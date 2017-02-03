////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 RWS Inc, All Rights Reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of version 2 of the GNU General Public License as published by
// the Free Software Foundation
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//////////////////////////////////////////////////////////////////////
//
//	IMAGE.CPP
//   
//	Created on 		4/20/95	MR 
// Implemented on 4/21/95	JW
// Changes Added	4/25/95	PPL	Change from using halloc to malloc
//											since we are in Win32.
// 05/08/95	JMI	Took out CDib dependency and added internal method
//						for loading BMPs.
//
//	05/09/95	JMI	No longer converting from RGBQUAD type palette to
//						555 when loading DIBs since RGBQUAD is the DIB
//						palette.
//
//	05/10/95	JMI	CPal now contains the ability to convert between
//						an RGBQUAD (DIB-type (FCC_pdib)) and a 555
//						(FCC_p555).
//
//	05/10/95	JMI	Added member to store whether it's okay to free
//						the palette data (i.e., we allocated it in CPal).
//
//	05/11/95	JMI	Took out fstream stuff that was including Windows.h
//						causing this module to take weeks to compile.
//
//	09/26/95 BRH	Completely changed the CImage class to a new
//						version that is compatible with Jeff's Blit modules
//						and includes expandability for future image types.
//						This is the new standard image which each new image
//						type will be required to understand and be able to 
//						convert to/from.  This CImage will complete the missing
//						link between the app level image format and Jeff's
//						Blit library.  There is no CPalImage anymore, but the
//						more general CImage may have a valid CPal pointer or
//						nullptr if  there is no palette.
//
// 11/03/95 BRH	Made a few minor changes to the TRACE messages.
//						Implemented the CImage and CPal Load and Save 
//						functions which will be able to save any standard
//						image and palette.  Currently there are no image
//						types that it cannot save, but an image type that 
//						used a pSpecial to point to data containing other
//						pointers would not be saved correctly with the
//						standard CImage::Load.  If you have a special case
//						like that, you can call write your own save that calls
//						CImage::Load to save the standard part of the image 
//						and then append your own special data at the end.
//
//	11/15/95	BRH	Added CPal::DetachData() so that the pData can be
//						detached from the palette.  This makes the new
//						way of converting palettes easier.  Previously when
//						a CImage converted a palette it created a whole new
//						CPal object, but now it instead keeps the original
//						CPal object and just changes the palette's pData.
//						See note in imagecon.cpp history for more information.
//
// 11/20/95 BRH	Added CPal::Convert() so that palettes can be converted
//						using the existing CImage::Convert routines.  The Pal
//						conversion creates a temporary palettized image and
//						runs the conversion.  Also added CImage::SetPalette()
//						and a mechanism by which a CImage can keep track of
//						palettes that it allocated and those that were set
//						by the user.  Also changed pMem from public to private
//						and added a GetMemory access function to effectively 
//						make the memory pointer read only.
//
//	01/16/96	JMI	Altered Convert to utilize dynamically linked convertors.
//						Updated aPalEntrySizes.
//
//	01/16/96 BRH	Added pSpecialMem to solve a problem that came up when
//						images that included pSpecial data were loaded from a file.
//						Before the user was always responsible for freeing the 
//						pSpecial data but in this case, if a user simply loads
//						a specific image type that happened to have pSpecial data,
//						it would be a burden to them to have to remember to free
//						the special data before the desturctor was called for the image.
//						so the destructor will take care of pSpecial if it was the one
//						that created it (as a result of loading from a file).
//
//						Also added GetPitch(width, depth) to provide an easy calculation
//						of 128-bit aligned pitches.  Some of the conversion functions
//						were not recalculating the pitch of the new image type so this
//						function call will be added to the conversion functions.
//
// 01/23/96 BRH	Adding Operator= function to copy images and palettes which
//						will be used in the new SaveDIB function.  SaveDIB is the
//						counterpart to LoadDIB.  This will allow us to export images
//						back into a common .BMP file for further editing with standard
//						tools.  SaveDIB will use the code from DIB's save and will
//						help CImage start to take the place of DIB in any RSPIX programs.	 
//
// 02/27/96 BRH	Added call to IMAGELINKINSTANTIATE macro to set up the
//						CImageSpecialFunc class's arrays of special functions
//						This version of CImage uses the CImageSpecialFunc class to
//						set up functions for special image types for conversion, 
//						load/save and alloc/delete.  The CImageSpecialFunc class
//						is based on the DYNALINK that was used in the last version
//						for ConvertTo and ConvertFrom functions.  Once we determined
//						that we also needed special version of load, save, alloc, 
//						and delete we decided to make a class similar to DYNALINK that
//						would take care of all of these special functions at once
//						rather than setting up 6 different DYNALINK arrays.
//
// 03/04/96 BRH	Added calls to the special functions.  The ConvertTo
//						and ConvertFrom have been changed from DYNALINK to
//						CImageSpecialFunc but the macros to call them remained
//						the same.  Macro calls to get the function pointers for
//						special image save and load were added to the Image's
//						Save and Load functions.  First the standard image
//						information is written and then the special save for that
//						image type is called if it exists to write the special
//						data to the open CNFile.  I also added calls to special
//						allocation functions in CreateData and a call to the special
//						delete function in the destructor.
//
// 03/18/96	JMI	Added SaveDib() ripped nearly verbatim from DIB.CPP.
//
// 04/30/96	BRH	Added some new fields to the header of the CImage, thus
//						invalidating the previous .IMG files but at this point they
//						weren't in wide use.  The Blit routines required a few
//						additional pieces of information so that they could avoid
//						recreating the buffer as they needed it.  I added 
//						lBufferWidth, lBufferHeight, lXPos, lYPos to accomodate 
//						the Blit functions.  This way a buffer that is larger
//						than the image can be created and the image can be
//						located at a specific position in that buffer. Note that
//						the whole buffer is not saved, just the image but the
//						size of the buffer is saved so that upon load, the buffer
//						can be recreated at its desired size.  
//						I also added ulDestinationType to accomodate image formats
//						that may be saved as one type but are converted to another
//						type once they are loaded.  For example, if you are using
//						sprites and there is no save funciton for sprites, you can
//						save the file with BMP8 type and a destination type of
//						FAST_SPRITE so that when the file is loaded, it is
//						read as a BMP8 format but automatically converted to
//						the fast sprite type.  Then to avoid debugging problems
//						when the CImage header is changed, I added a version number
//						as a #define and write that to the file.  Then when a .IMG file
//						is loaded, the version number is checked with the current
//						version of CImage and if they are different it will return
//						an error.
//
// 05/16/96 BRH	Changed from using malloc to calloc so that image buffers
//						will be cleared to zero before being used.
//
//	07/12/96	JMI	Changed CreateImage so it updates the lPitch member once
//						calculated (if calculated (may be passed in)).
//
// 08/02/96 BRH	Fixed Load so it is in sync with Save.  The Save function
//						had been updated as described above in 4/30/96 comment with the
//						new fields added, but the Load function did not yet read those
//						fields.
//
// 08/04/95 MJR	Added function prototypes for sCreateMem(), sCreateAlignedMem(),
//						and sDestroyMem(), and also made them static, thereby brining
//						them into compliance with ANSI Section 5.75.53 Subsection 5a
//						and Section 8.27 Subsection 2b.  More importantly, it avoided
//						CodeWarrior compiler warnings.
//
//	09/10/96	JMI	Modified LoadDib() to be able to calculate proper pitches for
//						bit depths that don't have 8 as a factor.  Same for 
//						SaveDib().  LoadDib() was not reporting an error for 16 bit 
//						BMPs even though it was setting the ulType field to 
//						NOT_SUPPORTED.  Also, SaveDib() was improperly setting the 
//						size field of the DIB file header to the lPitch * lHeight 
//						when it should have been the lDibPitch * lHeight.  Also, 
//						WIDTHU8 and WIDTH128 macros were not 'order-of-operations'
//						safe macros.  Added parenthesis surrounding arguments for 
//						that extra sense of comfort we've come to know and love.  We
//						deserve that kind of protection.
//
//	10/09/96	JMI	Load() was converting to ulDestinationType even
//						if it was NOT_SUPPORTED.  ReadPixelData() was using
//						a comparison that was not compatable with the one
//						used in WritePixelData() for the same data set.
//						Fixed.
//
//	10/10/96	JMI	CreateImage() was computing lPitch wrong.  Fixed.
//
//	10/15/96 MJR	Added GetEntries() to CPal as a standardized method of
//						accessing the palette color info regardless of format or type.
//
//	10/16/96	JMI	Minor fix in CPal::GetEntries() to ASSERTs on undefined vars.
//
//	10/18/96 MJR	Added SetEntries() to CPal as a standardized method of
//						accessing the palette color info regardless of format or type.
//						Added CreatePalette() to CPal as a way to create a palette
//						without having to know much (if anything) about it's format.
//						Fixed GetEntries() to properly handle 555 and 565 palettes.
//						Fixed GetEntries() to properly deal with palette's whose starting
//						index is not 0 (very rare, but a bug nonetheless).
//
//	10/21/96 BRH	Changed all loads and saves so that optional data
//						does not need to be present to save an Image or
//						palette.  Now you can save a palette with no
//						color data, just the header if you want.  Each
//						optional section is flaged in the file.  Also, I
//						modified the load and save routines to check
//						only for critical errors like wrong magic_number or
//						wrong version.  Then the error codee is checked
//						after all of the reads or writes have been done
//						before it reports an error.  This cut out most of
//						the if/else pyramid code and made it much easier 
//						to read.  
//
//	10/24/96 BRH	Changed WritePixelData and ReadPixelData functions
//						to read and write their data according to the bit
//						depth rather than treating it as a block of bytes.
//						This was done so that the data will be properly
//						byte swapped when transfering it between the Mac and PC.
//						For example, 16 bit image formats are now written 
//						to the CNFile as a number of uint16_ts rather than
//						twice as many uint8_ts.  This same functionality
//						needs to be added to load and save for DIBs which
//						will be in the next version.
//
//	10/30/96	JMI	Pulled CPal stuff out of here and put it into pal.cpp.
//						Pulled astrImageTypeNames out of imagetyp.h and put it
//						here as ms_astrTypeNames static member.
//						Attempted to reword comment summary to reflect these
//						changes.
//						Note: I'm not positive but I think the references to
//						imageafp.h are out of date.
//
//	10/30/96	JMI	Changed:
//						Old label:		New label:
//						=========		=========
//						CNFile			RFile
//						CImage			RImage
//						CPal				RPal
//						uint32_t ulType	RImage::Type ulType
//
//						The thing that annoys me the most about using actual enums
//						instead of uint32_ts is that you have to copy it into a dummy
//						uint32_t to use RFile on it.  This isn't very bad, but it's
//						annoying.
//
//	10/31/96	JMI	Changed all members to be preceded by m_ (e.g., sDepth
//						m_sDepth).  Changed all position members (i.e., lWidth,
//						lHeight, lBufferWidth, lBufferHeight, lXPos, & lYPos) to
//						be int16_ts (i.e., m_sWidth, m_sHeight, m_sBufferWidth,
//						m_sBufferHeight, m_sXPos, m_sYPos) and functions associated
//						with these members reflect this change (e.g., int32_t GetWidth()
//						is now int16_t GetWidth()).  Changed ulType to m_type and 
//						ulDestinationType to m_typeDestination.  Increased file
//						version to 5 since members converted to int16_t will affect
//						the file format.
//
//	11/22/96	JMI	DestroyData() now destroys m_pSpecialMem as well.
//
//	11/25/96	JMI	Because DestroyData() had explicit returns other than the
//						one at the end, the fix above was rarely being processed.
//						Fixed.  Now DestroyData() contains only one return.
//
//	11/26/96	JMI	Now DestroyData() sets pSpecialMem to nullptr.
//
//	11/27/96 BRH	Changed names of the buffer and image width around
//						to make the usage more clear and to make sure that
//						code using the previous method would not quit working
//						due to this change.  Previously the widht and height
//						referred to the visible picture which may be stored
//						in a larger buffer.  The BufferWidth and BufferHeight
//						referred to the size of the memory in which the picture
//						was being stored.  Now we will change the width and height
//						to mean the entire memory area and the size of the picture
//						will be referred to by WinWidth and WinHeight.  I am 
//						rearranging the order of the variables in the class so
//						that I won't have to change the version of the file
//						format and I will also change the order that these values
//						are being written and read from the files so that old
//						image files will continue to work.
//
//	12/04/96	JMI	Added patch to make old .IMG files that had m_sBufferWidth
//						and m_sBufferHeight set to 0.  For a more detailed
//						explanation, search for "Begin load patch".
//
//	12/11/96	JMI	Now calls RImageFile::Load(...) to load images.
//						RImageFile::Load(...) has the advantage of potentially
//						supporting older formats.  This required that extended
//						support functions for load and save be aware of the file
//						version number, so LOADFUNC and SAVEFUNC now take a
//						ulVersion.
//
//	12/11/96	JMI	Changed LOADFUNC and SAVEFUNC back to NOT taking a ulVersion
//						because the BLiT libs already use these extensions without
//						the ulVersion.  This change is temporary until such a change
//						can be made to BLiT.
//
//	12/18/96 BRH	Added overloaded versions of LoadDib and SaveDib that
//						take an RFile* and can load or save to that rather than
//						just the filename versions.  This was first used
//						in the resource manager but is also otherwise
//						generally useful.
//
//	12/21/96	JMI	LoadDib() is now able to load itself from a larger file.
//						It was using a Seek() to a non-relative file address.  Now
//						it simply adds the value of an initial Tell() to the address
//						to Seek().
//
//	02/04/97	JMI	Added BMP_MAGIC_NUMBER for in detecting a .BMP formatted file.
//						Made LoadDib() a private member.  Now Load() will load *.BMP
//						or *.IMG formatted files.
//
//	04/16/97	JMI	Ammendded operator= overload.
//
// 05/21/97 JRD	Fixed a bug in the default pitch generation of CreateImage.
//
//	06/08/97 MJR	Removed unecessary (some would say incorrect) TRACE messages
//						and associated error returns from DestroyData() and
//						DestroyPalette().  They were complaining if you called
//						called them when the data had been allocated by the user.
//						Instead, they now silently deal with it.
//
//	Image.h contains the class RImage.  Pal.h contains RPal.  The RImage is used
// to store generic images either with or without palettes.  RPal can also be 
// used alone to store palettes.
//
// The image format sets a standard image format as a middle ground.
// If you wish to create a different image format, such as compressed
// images, then you would supply a convert function that would be able
// to convert from this standard RImage format to your compressed 
// format and a conversion function to convert from your compressed
// format to this standard format.  You can choose to change the pData
// pointer to point to the data in your new format, or you could
// choose to keep the standard form in pData and allocate memory for 
// your compressed version and use pSpecial to keep track of it.
//
// If you create a new image format, you should register your
// enumerated image type in image.h in the eImageTypes enum and add 
// your Convert() function pointer to the array of convert functions
// in imageafp.h.  The array of convert function pointers is indexed
// by the image type.
// Then you may add your conversion function either in your own
// file or you could add its prototype to imagecon.h and the
// code to imagecon.cpp.
//
//////////////////////////////////////////////////////////////////////

#include "Image.h"
#include "ImageFile.h"

#include <cstdio>
#include <cstdlib> // For malloc, etc.
#include <cstring> // For memcpy...
#include <cstdalign>

#include <utility>

//////////////////////////////////////////////////////////////////////
// Instantiate class statics.
//////////////////////////////////////////////////////////////////////

// This array of type names should correspond to the list of
// enumerated types eImageTypes in image.h.  Whenever you add an image
// type and an enum, you need to also insert that name into the 
// corresponding place in this array.  
// Note that this uses END_OF_TYPES enum item to size the array.

char const * const RImage::ms_astrTypeNames[END_OF_TYPES] = 
{
  "Same Type",
  "BMP8",
  "SYSTEM8",
  "SCREEN8_555",
  "SCREEN8_565",
  "SCREEN8_888",
  "BMP24",
  "SCREEN16_555",
  "SCREEN16_565",
  "SCREEN24_RGB",
  "SCREEN32_ARGB",
  "FSPR1",
  "FSPR8",
  "FSPR16",
  "FSPR32",
  "ROTBUF",
  "SPECIAL",
  "FLX8_888",
  "IMAGE_STUB",
  "BMP8RLE",
  "BMP1",				// Added 09/04/96	JMI.
};

//////////////////////////////////////////////////////////////////////
// Instantiate Dynamic Arrays
//////////////////////////////////////////////////////////////////////

int16_t	ConvertNoSupport(RImage* pImage);

IMAGELINKINSTANTIATE();

IMAGELINKLATE(NOT_SUPPORTED, ConvertNoSupport, nullptr, nullptr, nullptr, nullptr, nullptr);

//////////////////////////////////////////////////////////////////////
//
//	sCreateMem
// (static)
//
//	Description:	
//		To allocate memory for the data buffers of RPal
//
//	Parameters:	
//		hMem		handle used for the buffer
//		ulSize	size of buffer to allocate in bytes
//
//	Returns:    
//		0		Success
//		-1		Buffer has already been allocated
//		-2		Buffer could not be allocated
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::sCreateMem(void **hMem,uint32_t ulSize)
{
  //	Make sure the data
  //	hasn't already been allocated
  if (*hMem)
  {
    TRACE("RPal::AllocMem() called by CreateData() -- A buffer has already been allocated");
    // Image allocated already
    return ((int16_t)-1);
  }
  else
  {
    if (ulSize > 0)
    {
      if ((*hMem = calloc(ulSize, 1)) == nullptr)
      {
        TRACE("RPal::AllocMem() called by CreateData() -- The buffer could not be allocated");
        // Image buffer couldn't be allocated
        return ((int16_t)-2);
      }
      else
      {
        // Success
        return ((int16_t)0);
      }
    }
    else
    {
      TRACE("RPal::AllocMem() called by CreateData() - Warning attempting to allocate 0 bytes, quit screwing around");
      *hMem = nullptr;
      return 0;
    }
  }
}

//////////////////////////////////////////////////////////////////////
//
//	sCreateAlignedMem
// (static)
//
//	Description:	
//		To allocate memory and return a pointer aligned to 128-bits
//		for optimum blit speed.  This is the function used by
//		RImage when it creates memory for the image buffers.
//
//	Parameters:	
//		hMem		handle used for deallocating memory later
//		hData		handle for the aligned image buffer
//		ulSize	size of buffer to allocate in bytes
//
//	Returns:    
//		0		Success
//		-1		Buffer has already been allocated
//		-2		Buffer could not be allocated
//
//////////////////////////////////////////////////////////////////////
#ifdef UNUSED_FUNCTIONS
int16_t RImage::sCreateAlignedMem(void **hMem, void **hData, uint32_t ulSize)
{
  // Make sure the data hasn't already been allocated
  if (*hMem)
  {
    TRACE("RImage::AllocMem called by CreateData() - buffer has already been allocatd");
    // buffer already exists
    return FAILURE;
  }
  else
  {
    if (ulSize > 0)
    {
      // allocate an extra 15 bytes so that the data ponter can be aligned
      // to the nearest 128-bit boundry for Blit speed reasons
      if ((*hMem = calloc(ulSize + 15, 1)) == nullptr)
      {
        TRACE("RImage::AllocMem() called by CreateData() - buffer could not be allocated");
        // calloc failed
        return FAILURE;
      }
      else
      {
        // Set Data buffer to 128-bit alignment
        *hData = (void*) (( *hMem + (void*)0x0f) & 0xfffffff0);
        // success
        return SUCCESS;
      }
    }
    else
    {
      TRACE("RImage::AllocMem() called by CreateData() - Warning attempted to create a buffer of 0 bytes, quit screwing around");
      *hMem = nullptr;
      return SUCCESS;
    }
  }
}
#endif

//////////////////////////////////////////////////////////////////////
//
//	sDestroyMem
// (static)
//
//	Description:
//		To free the data buffers of RPal and RImage that were created 
//		using either sCreateMem() or sCreateAlignedMem()
//
//	Parameters:	
//		hMem		handle to the memory used by the buffer
//
//	Returns:    
//		0		Success
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::sDestroyMem(void **hMem)
{   
  // Make sure the memory
  // hasn't already been freed
  if (*hMem)
  {
    free(*hMem);
    *hMem = nullptr;
  }

  // Always return success because
  // the memory has been freed
  return SUCCESS;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////// RImage //////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
//	RImage Member Functions
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// Init
//
// Sets up the array of conversion handlers for this class
//
//////////////////////////////////////////////////////////////////////

void RImage::Init()
{

}

//////////////////////////////////////////////////////////////////////
//
// RImage::DestroyDetachedData
//
// Description:
//		static member function that deallocates memory that was
//		originally created using RImage::CreateData() and then
//		detached from the RImage using DetachData().  This function
//		will use the correct method to free the memory.
//
//	Parameters:
//		hMem	handle to the memory to be freed
//
// Returns:
//		SUCCESS if the memory was successfully freed
//		FAILURE if either the handle or the pointer to memory was
//				  nullptr already
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::DestroyDetachedData(void** hMem)
{
  if (hMem)
  {
    if (*hMem)
      return sDestroyMem(hMem);
    TRACE("Image::DestroyDetachedData - Attempted to free a nullptr pointer.");
  }
  else
    TRACE("Image::DestroyDetachedData - Attempted to free a nullptr handle.");

  return FAILURE;
}

//////////////////////////////////////////////////////////////////////
//
// Constructor
//
// Description:
//		Default constructor for the RImage class.  Initializes the
//		member variables but does not create or load any data.
//
// Parameters:
//		none
//
// Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

RImage::RImage()
{            
  // Initialize member variables to zero
  InitMembers();
}	

//////////////////////////////////////////////////////////////////////
//
// Constructor
//
// Description:
//		Initializes class members and creates data of the given size
//
// Parameters:
//		ulNewSize	size of data to be created
//
//	Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

RImage::RImage(uint32_t ulNewSize)
{
  // Initialize member variables to zero
  InitMembers();

  CreateData(ulNewSize);
}

//////////////////////////////////////////////////////////////////////
//
// Constructor
//
// Description:
//		Initializes member variables and attempts to load the given
//		BMP from a file.
//
// Parameters:
//		pszFilename	Filename to load
//
// Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

RImage::RImage(char const * pszFilename)
{
  // Initialize member variables to zero
  InitMembers();

  LoadDib(pszFilename);
}

//////////////////////////////////////////////////////////////////////
//
// Destructor
//
// Description:
//		Deallocates buffer memory, and palette if any
//
// Parameters:
//		none
//
// Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

RImage::~RImage()
{
  // Free Image data
  // if (m_pMem) // WILL NEVER ALLOW DeleteSpecial
  DestroyData();

  if (m_pPalMem)
    delete(m_pPalMem);
}

//////////////////////////////////////////////////////////////////////
//
// RImage::Init
//
// Description:
//		Initialize all members.  Calling this when m_pMem* is set is 
//		not a good idea.
//
// Parameters:
//		None.
//
//	Affects:
//		All members.
//
// Returns:
//		Nothing.
//
//////////////////////////////////////////////////////////////////////

void RImage::InitMembers(void)
{
  // Initialize member variables to zero
  m_type				= NOT_SUPPORTED;
  m_typeDestination	= NOT_SUPPORTED;
  m_ulSize				= 0;
  m_sWidth				= 0;
  m_sHeight			= 0;
  m_sWinWidth			= 0;
  m_sWinHeight		= 0;
  m_sWinX				= 0;
  m_sWinY				= 0;
  m_lPitch				= 0;
  m_sDepth				= 0;
  m_pMem				= nullptr;
  m_pData				= nullptr;
  m_pPalette			= nullptr;
  m_pPalMem			= nullptr;
  m_pSpecial			= nullptr;
  m_pSpecialMem		= nullptr;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::CreateData
//
// Description:
//		Creates an 128-bit aligned buffer for the image data
//
// Parameters:
//		ulNewSize	Size in bytes of the buffer
//
// Returns:
//		SUCCESS if the memory was alocated successfully 
//		FAILURE if memory could not be allocted
//
//////////////////////////////////////////////////////////////////////

int16_t	RImage::CreateData(uint32_t ulNewSize)
{
  if (m_pMem)
  {
    TRACE("RImage::CreateData - Attempting to create data when pMem is still pointing to memory");
    return FAILURE;
  }

  if (m_pData && !m_pMem)
    TRACE("RImage::CreateData - Warning: pData is pointing to data");

  ALLOCFUNC caf = GETALLOCFUNC(m_type);
  if (caf && (*caf)(this) != SUCCESS)
    TRACE("RImage::CreateData - Error creating data for special type %d", m_type);

  m_ulSize = ulNewSize;
#ifdef ALIGNED_MEMORY
  return sCreateAlignedMem((void**) &m_pMem, (void**) &m_pData, ulNewSize);
#endif
  m_pMem = m_pData = new uint8_t[ulNewSize];
  return m_pMem != nullptr ? SUCCESS : FAILURE;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::CreateData
//
// Description:
//		Create IMAGE's data utilizing passed in fields.
//		Calls CreateData(uint32_t) to do the allocation.
//
// Parameters:
//		As described below.
//
// Returns:
//		Return value from CreateData(uint32_t).
//		SUCCESS if the memory was alocated successfully 
//		FAILURE if memory could not be allocted
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::CreateImage(		// Returns 0 if successful.
                                 int16_t	sWidth,					// Width of new buffer.
                                 int16_t	sHeight,					// Height of new buffer.
                                 Type	type,						// Type of new buffer.
                                 int32_t	lPitch	/*= 0L*/,	// Pitch of new buffer or -1 to calculate.
                                 int16_t	sDepth	/*= 8*/)		// Color depth of new buffer.
{
  int16_t	sRes	= SUCCESS;	// Assume success.

  // Fill in fields.
  m_sWidth = m_sWinWidth	 = sWidth;
  m_sHeight = m_sWinHeight = sHeight;
  m_type						 = type;
  m_sDepth						 = sDepth;
  m_sWinX = m_sWinY			 = 0;
  // If no pitch specified . . .
  if (lPitch == 0L)
  {
    lPitch	= GetPitch(sWidth, sDepth);
  }

  // Update member lPitch.
  m_lPitch			= lPitch;
  m_ulSize			= lPitch * (int32_t)sHeight;
  if (m_ulSize > 0)
  {
    sRes	= CreateData(m_ulSize);
  }

  return sRes;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::DetachData
//
// Description:
//		This function detaches the image buffer from the RImage
//		and returns the pointer to the original buffer to you.  
//		You can then call RImage::CreateData again to create a new
//		buffer.  You are responsible for keeping track of the detached
//		buffer and deallocating its memory when you are finished with it.
//		You should deallocate the memory by calling
//		RImage::DestroyDetachedData() so it will be freed using a 
//		deallocation function that is compatible with the way the
//		data was allocated.  
//
//		This function is useful when doing image conversion where you 
//		want to create a new buffer for the new data and get rid of
//		the old buffer when you are done with the conversion.  This
//		version of DetachData returns pointer to the memory buffer,
//		and not the pointer to the data (the aligned pointer).  If you
//		need to use the data for a conversion for example, then you
//		should first save a pointer to the data pImage->pData before
//		calling DetachData, or you can call the other version of
//		DetachData where you supply a handle to the memory and to
//		the data.
//
//	Parameters:
//		none
//
// Returns:
//		RImage.pMem, the pointer to the allocated memory
//
//////////////////////////////////////////////////////////////////////

void* RImage::DetachData(void)
{
  void* pDetachment = m_pMem;
  m_pMem = m_pData = nullptr;
  return pDetachment;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::DetachData
//
// Description:
//		This function detaches the image buffer from the RImage
//		and returns the pointer to the original buffer to you.  
//		You can then call RImage::CreateData again to create a new
//		buffer.  You are responsible for keeping track of the detached
//		buffer and deallocating its memory when you are finished with it.
//		You should deallocate the memory by calling
//		RImage::DestroyDetachedData() so it will be freed using a 
//		deallocation function that is compatible with the way the
//		data was allocated.  
//
//		This function is useful when doing image conversion where you 
//		want to create a new buffer for the new data and get rid of
//		the old buffer when you are done with the conversion.  This
//		version of DetachData takes two handles to memory and 
//		gives hMem the pointer to the allocated memory and hData
//		the pointer to the aligned memory where the data begins.
//		Alternatively you could save a pointer to the image data
//		RImage.pData and then call DetachData() which returns 
//		a pointer to the memory buffer.
//
//	Parameters:
//		none
//
// Returns:
//		FAILURE if either handle passed in was nullptr
//		SUCCESS otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::DetachData(void** hMem, void** hData)
{
  if (hMem && hData)
  {
    *hMem = m_pMem;
    *hData = m_pData;
    m_pMem = m_pData = nullptr;
    return SUCCESS;
  }
  return FAILURE;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::DestroyData
//
// Description:
//		Deallocated memory created by CreateData()
//
// Parameters:
//		none
//
// Returns:
//		FAILURE if DestoryData is called with data that was set by
//		        the user using SetData().  
//		SUCCESS after freeing the data created by CreateData()
//
//////////////////////////////////////////////////////////////////////

int16_t	RImage::DestroyData()
{
  int16_t	sRes	= 0;	// Assume success.

  // Only if the data was not supplied by the user.
  if (m_pMem)
  {
    m_pData = nullptr;
    sRes	= sDestroyMem((void**) &m_pMem);
    m_pMem = nullptr;
  }

  if (m_pSpecialMem)
  {
    // If there is a special delete function for this image type
    // call it so that it can clean up its pSpecial Memory
    DELETEFUNC cdf = GETDELETEFUNC(this->m_type);
    if (cdf)
    {
      if ((*cdf)(this) == SUCCESS)
      {
        m_pSpecialMem	= nullptr;
      }
    }
    else
    {
      // Else do the best you can
      free(m_pSpecialMem);

      m_pSpecialMem	= nullptr;
    }
  }

  return sRes;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::SetData
//
// Description:
//		Allows the user to set the image data pointer to a buffer
//		that was created by the user.  The user will be responsible
//		for deallocating this buffer, they cannot use DestroyData().
//
// Parameters:
//		pUserData = pointer to an image buffer
//
// Returns:
//		SUCCESS if the memory was set
//		FAILURE if the image already had a buffer
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::SetData(void* pUserData)
{
  if (m_pMem)
  {
    TRACE("Image::SetData - Attempted to set your own data pointer while there was memory allocated");
    return FAILURE;
  }
  m_pData = (uint8_t*) pUserData;
  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// SetPalette
//
// Description:
//		Sets a user palette to the image's pPalette pointer.  This is
//		the safe way of setting your own palette rather than just
//		setting pImage->pPalette to your palette.  This will warn you
//		if the image is already pointing to a palette allocated by
//		the image by a function like CreatePalette().
//
// Parameters:
//		pPal = pointer to your palette
//
// Returns:
//		SUCCESS - in all cases for now anyway, since it will always
//					 set the palette even if it has to delete the old one
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::SetPalette(RPal* pPal)
{
  if (m_pPalMem)
  {
    TRACE("RImage::SetPalette - Warning: m_pPalette points to an Image-allocated palette");
    TRACE("                     The previous palette will be deleted and your palette will be set");
    delete(m_pPalMem);
    m_pPalMem = nullptr;
  }
  m_pPalette = pPal;
  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::CreatePalette
//
// Description:
//		Creates a RPal object but doesn't allocate any palette buffer
//
// Parameters:
//		none
//
// Returns:
//		SUCCESS if the palette was created
//		FAILURE if memory could not be allocated for the palette
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::CreatePalette(void)
{
  m_pPalMem = m_pPalette = new RPal();
  return m_pPalette == nullptr ? FAILURE : SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::CreatePalette
//
// Description:
//		Creates a RPal object and allocates a pallete of the given
//		size
//
// Parameters:
//		ulSize = size in bytes of the palette
//
// Returns:
//		SUCCESS if the palette was created
//		FAILURE if memory could not be allocated for the palette
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::CreatePalette(uint32_t ulSize)
{
  if (CreatePalette() == SUCCESS)
    return m_pPalette->CreateData(ulSize);
  else
    return FAILURE;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::DetachPalette
//
// Description:
//		Removes the RPal from the image and returns the pointer to you.
//		Now you can call CreatePalette again for the image to create
//		a new palette and you are responsible for the palette returned
// 	to you.  You should use delete to deallocate the palette when
//		you are done with it.  This function is useful when doing
//		palette conversions on an image where you want to create a
//		new palette for the image and only want to use the old one
//		during the conversion and then delete it when you are done.
//
// Parameters:
//		none
//
// Returns:
//		RPal* = pointer to the RPal object
//		nullptr if  there is no palette for this image
//
//////////////////////////////////////////////////////////////////////

RPal* RImage::DetachPalette(void)
{
  RPal* pDetachment = m_pPalette;
  m_pPalette = m_pPalMem = nullptr;
  return pDetachment;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::DestroyPalette
//
// Description:
//		Deallocates the palette for the image that was created using
//		CreatePalette()
//
// Parameters:
//		none
//
//	Returns:
//		SUCCESS after the palette is deallocated
//		FAILURE if the palette could not be destroyed either because
//			     there was no palette or it was a user set palette
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::DestroyPalette(void)
{
  if (m_pPalMem)
  {
    delete m_pPalMem;
    m_pPalMem = m_pPalette = nullptr;
  }
  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// Equals overload.
// Note that this function could fail.
//////////////////////////////////////////////////////////////////////
RImage& RImage::operator=(const RImage& imSrc)
{
  // Easiest most likely to succeed way to get an image copy is to
  // write it to a file.
  RFile	file;
  // We know we'll probably need at least m_ulSize bytes so that's a
  // good start for the size of the mem file.
  // Allow it to grow byte let's say 1K at a time for reasonable
  // efficiency vs memory wastage.
  try
  {
    if (file.Open(imSrc.m_ulSize, 1024, RFile::LittleEndian) != SUCCESS)
      throw "operator=(): file.Open() failed.";

    // Save the source into the mem file . . .
    if (imSrc.Save(&file) != SUCCESS)
      throw "operator=(): imSrc.Save() to mem file failed.";

    // Go back to beginning.
    file.Seek(0, SEEK_SET);

    // Load the mem file into the dest (this) . . .
    if (Load(&file) != SUCCESS)
      throw "operator=(): Load() from mem file failed.";
    // Successful copy!
  }
  catch(const char* message)
  {
    TRACE(message);
  }

  return *this;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::Convert
//
// Description:
//		This function calls one of the convert functions from the 
//		array ms_afp stored in the CDynaLink template.  The array of 
//		functions is indexed by the type of image that you wish to 
//		convert to.
//		Each convert function will evaluate the current type of the
//		image and if it supports converting from the current type
//		to the new type, it will do the conversion and return the
//		new type.  If it is not supported then it will return
//		NOT_SUPPORTED.
//
// Parameters:
//		ulType = one of the enumerated types in imagetyp.h.  This
//				   is the type you wish to convert to.
//
// Returns:
//		ulType if successful
//		NOT_SUPPORTED if the conversion cannot be done with the
//				        current image type, or if there is no
//						  convert function for the type you specified
//
//////////////////////////////////////////////////////////////////////

RImage::Type RImage::Convert(Type type)
{
  Type	typeRes	= NOT_SUPPORTED;	// Assume sux.

  // If out of range . . .
  if (type >= END_OF_TYPES)
  {
    // Not supported.
  }
  else
  {
    // If current format is extended . . .
    if (m_type > SCREEN32_ARGB)
    {
      // Verify function exists . . .
      CONVFROMFUNC	cft	= GETFROMFUNC(m_type);
      if (cft)
      {
        // Convert to a standard type.
        typeRes	= (Type)(*cft)(this);
      }
      else
      {
        TRACE("Convert(): Type exists, but no current link to convert to "
              "standard type.  Check for proper module.");
        typeRes	= NOT_SUPPORTED;
      }
    }

    // If current format is standard . . .
    if (	m_type	>= BMP8
         &&	m_type	<= SCREEN32_ARGB)
    {
      // If current format is not the destination format . . .
      if (m_type	!= type)
      {
        // Verify function exists . . .
        CONVTOFUNC	ctt	= GETTOFUNC(type);
        if (ctt)
        {
          typeRes = (Type)(*ctt)(this);
        }
        else
        {
          TRACE("Convert(): Type exists, but no current link.  Check for "
                "proper module.");
          typeRes	= NOT_SUPPORTED;
        }
      }
      else
      {
        // Already in correct format (after standardization).
        typeRes	= type;
      }
    }
    else
    {
      TRACE("Convert(): Not in a standard format.");
      // Preserve current return value.
    }
  }

  return typeRes;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::LoadDib
//
// Description
//		This function will allow the user of RPalImage to
//		read in a Window's Dib.
//
// Parameters:	
//		pszFileName = pointer to the filename of the DIB
//			or
//		pcf = pointer to open RFile where a DIB is stored
//
// Returns:
//		  0: 		(SUCCESS) if successful
//		-20:		Palette read error
//		-19:		Pixel data read error
//		-18:		Can't allocate memory for DIB
//		-17:		colors important field not read
//		-16:		colors used field not read
//		-15:		vert pixels per meter field not read
//		-14:		horz pixels per meter field not read
//		-13:		size of image field not read
//		-12:		compression field not read
//		-11:		bit count field not read
//		-10:		planes field not read
//		-9:		height field not read
//		-8:		width field not read
//		-7:		size field not read
//		-6:		offset to bits field not read
//		-5:		reserved2 field not read
//		-4:		reserved1 field not read
//		-3:		size of header field not read or not a BMP file
//		-2:		type field not read
//		-1:		unable to open file
///	1:			if a colordepth of 1 or 4 was read
//		2:			if an unknown colordepth has been encountered
//		3:			if the dib is compressed, can not handle yet
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::LoadDib(char const * pszFilename)
{
  RFile cf;
  int16_t sReturn = SUCCESS;

  if (cf.Open(pszFilename, "rb", RFile::LittleEndian) == SUCCESS)
  {
    sReturn = LoadDib(&cf);
    cf.Close();
  }
  else
  {
    TRACE("RImage::LoadDib - could not open Dib file %s - well shucks!", pszFilename);
    sReturn = FAILURE;
  }

  return sReturn;
}

int16_t RImage::LoadDib(RFile* pcf)
{
  DIBHEADER		dh;
  DIBFILEHEADER dfh;
  int32_t lDibPitch;

  try
  {
    if (!pcf || !pcf->IsOpen())
      throw std::make_pair("RImage::LoadDib - RFile* does not refer to an open file", -1);

    // Get the beginning of the DIB portion of this file.
    // In some cases, this DIB file may be embedded in a larger
    // collection of files.
    int32_t	lDibFileStartPos = pcf->Tell();

    //  Read BITMAPFILEHEADER

    if (!pcf->Read(&(dfh.usType), 1))
      throw std::make_pair("RImage::LoadDib(): Unable to read type field of bitmap file header.", -2);

    if (dfh.usType != BMP_MAGIC_NUMBER)
      throw std::make_pair("RImage::LoadDib(): NOT a BITMAP file.", -3);

    if (pcf->Read(&(dfh.ulSize), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read size field of bitmap file header.", -3);

    if (pcf->Read(&(dfh.usReserved1), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read reserved1 field of bitmap file header.", -4);

    if (pcf->Read(&(dfh.usReserved2), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read reserved2 field of bitmap file header.", -5);

    if (pcf->Read(&(dfh.ulOffBits), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read offset to bits field of bitmap file header.", -6);

    // Read BITMAPINFOHEADER
    if (pcf->Read(&(dh.ulSize), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read size field of bitmap info header.", -7);

    if (pcf->Read(&(dh.lWidth), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read width field of bitmap info header.", -8);

    if (pcf->Read(&(dh.lHeight), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read height field of bitmap info header.", -9);

    if (pcf->Read(&(dh.usPlanes), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read planes field of bitmap info header.", -10);

    if (pcf->Read(&(dh.usBitCount), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read bit count field of bitmap info header.", -11);

    if (pcf->Read(&(dh.ulCompression), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read compression field of bitmap info header.", -12);

    if (pcf->Read(&(dh.ulSizeImage), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read size of image field of bitmap info header.", -13);

    if (pcf->Read(&(dh.lXPelsPerMeter), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read horz pixels per meter field of bitmap info header.", -14);

    if (pcf->Read(&(dh.lYPelsPerMeter), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read vert pixels per meter field of bitmap info header.", -15);

    if (pcf->Read(&(dh.ulClrUsed), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read colors used field of bitmap info header.", -16);

    if (pcf->Read(&(dh.ulClrImportant), 1) != TRUE)
      throw std::make_pair("RImage::LoadDib(): Unable to read colors important field of bitmap info header.", -17);

    m_sDepth = dh.usBitCount;
    m_sWidth = m_sWinWidth = (int16_t)dh.lWidth;
    m_sHeight = m_sWinHeight = (int16_t)dh.lHeight;
    m_sWinX = m_sWinY = 0;

    // Pre calc width in bits.
    int32_t lBitsWidth = dh.lWidth * (int32_t)dh.usBitCount;
    m_lPitch  = WIDTH128(((lBitsWidth + 7) & ~7) / 8);
    lDibPitch = WIDTHU8(((lBitsWidth + 7) & ~7) / 8);

    if (dh.ulCompression) // Compressed, use bitmap's header.
      m_ulSize = dh.ulSizeImage;
    else // not compressed . . .
      m_ulSize = m_lPitch * dh.lHeight;

    if (CreateData(m_ulSize) != SUCCESS)
      throw std::make_pair("RImage::LoadDib(): Unable to allocate DIB.", -18);

    if (dh.usBitCount <= 8)
    {
      // Read palette in one chunk since only on this platform.
      int16_t sNumColors = 1 << dh.usBitCount;
      CreatePalette(sNumColors * 4);
      m_pPalette->m_sNumEntries = sNumColors;
      m_pPalette->m_sPalEntrySize = 4;

      if (pcf->Read(m_pPalette->m_pData, 4 * sNumColors) != 4 * sNumColors)
        throw std::make_pair("RImage::LoadDib(): Unable to read palette.", -20);
    }

    if (pcf->Seek(lDibFileStartPos + dfh.ulOffBits, SEEK_SET) != SUCCESS)
      throw std::make_pair("RImage::LoadDib(): Unable to seek to bits.", -20);

    if (dh.ulCompression) // if compressed
    {
      if(pcf->Read(m_pData, m_ulSize) != (int32_t)m_ulSize) // Read in one kerchunk.
        throw std::make_pair("RImage::LoadDib(): Unable to read all the compressed bits.", -19);
    }
    else // If not compressed . . .
    {
      // If we read in the upside down way . . .
      // "Upside down" way.
      // Read the dib a line at a time and flip it upside down (which is really right side up)
      for (int32_t l = dh.lHeight - 1L; l >= 0L; l--)
        if (pcf->Read(m_pData + (l * m_lPitch), lDibPitch) != lDibPitch)
          throw std::make_pair("RImage::LoadDib(): Unable to read all the bits.", -19);
    }
  }
  catch (std::pair<const char*, int> data) // If any errors occurred . . .
  {
    // Free the allocated memory.
    DestroyPalette();
    DestroyData();

    TRACE(data.first);
    return data.second;
  }

  try
  {
    // Make all the image data members correspond to the loaded dib.
    if (dh.ulCompression)
    {
      // Handle compressed bitmap type.
      if (dh.usBitCount == 8) // RLE8
      {
        m_type = BMP8RLE;
        if(Convert(BMP8) != BMP8) // Convert to BMP8.
          throw std::make_pair("RImage::LoadDib(): Failed to convert BMP8RLE to BMP8.", 2);
      }
      else // Unsupported compressed colordepth.
        throw std::make_pair("RImage::LoadDib(): Unsupported compressed colordepth (only 8bpp compression) supported.", 3);
    }
    else
    {
      // The dib's data is raw, uncompressed.
      switch (dh.usBitCount)
      {
        case 1:
          // raw, 1 bit per pixel (monochrome).
          // (index of 0 == White and 1 == Black).
          m_type	= BMP1;
          m_pPalette->m_type = RPal::PDIB;
          break;

        case 4:
          // This color depth is really not supported by RImage!
          m_type = NOT_SUPPORTED;
          throw std::make_pair("RImage:LoadDib() encountered a dib of colordepth 4, not supported!", 1);

        case 8:
          // raw, 8 bits per pixel
          m_type	= BMP8;
          m_pPalette->m_type = RPal::PDIB;
          break;

        case 16:
          // This color depth is really not supported by RImage!
          m_type = NOT_SUPPORTED;
          throw std::make_pair("RImage:LoadDib() encountered a dib of colordepth 16, not supported!", 1);

        case 24:
          // raw, 24 bits per pixel
          m_type	= BMP24;
          break;

        case 32:
          // raw, 32 bits per pixel
          m_type	= BMP24;
          break;

        default:
          // unsupported colordepth
          m_type = NOT_SUPPORTED;
          throw std::make_pair("RPalImage:LoadDib() encountered an unsupported colordepth!", 2);
      }
    }

    if (m_type == BMP8RLE && // needs to be converted
        Convert(BMP8) != BMP8) // Convert to BMP8.
      throw std::make_pair("RImage::LoadDib(): Failed to convert BMP8RLE to BMP8.", 2);
  }
  catch (std::pair<const char*, int> data) // If any errors occurred . . .
  {
    TRACE(data.first);
    return data.second;
  }

  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// SaveDib
//
// Description:
//		Saves the Image in DIB format (.bmp) to the given file.
//
// Parameters:
//		pszFilename = name of BMP file to be saved
//			or
//		pcf = pointer to open RFile where BMP is to be saved
//
// Returns:
//		SUCCESS if the file was saved
//		negative error code on failure
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::SaveDib(char const * pszFilename)
{
  RFile cf;
  int16_t sReturn = SUCCESS;

  if (cf.Open(pszFilename, "wb", RFile::LittleEndian) == SUCCESS)
  {
    sReturn = SaveDib(&cf);
    cf.Close();
  }
  else
  {
    TRACE("RImage::SaveDib - Could not open file %s for saving - huh, ain't that somthing", pszFilename);
    sReturn = FAILURE;
  }

  return sReturn;
}

int16_t RImage::SaveDib(RFile* pcf)
{
  try
  {
    if (!pcf || !pcf->IsOpen())
      throw std::make_pair("RImage::SaveDib - pcf does not refer to an open RFile", -1);

    int32_t lDibPitch = WIDTHU8((((int32_t)m_sWidth * (int32_t)m_sDepth + 7L) & ~7L) / 8L);

    int32_t	ulColorData	= 0;
    if (m_pPalette)
      ulColorData	= m_pPalette->m_ulSize;

    DIBFILEHEADER dfh;

    dfh.ulOffBits		= 14 + 40 + ulColorData;
    dfh.ulSize			= (dfh.ulOffBits + lDibPitch * (int32_t)m_sHeight) / 4L;
    dfh.usReserved1	= 0;
    dfh.usReserved2	= 0;

    uint8_t	auc[2]	= { 'B', 'M' };

    //  Write BITMAPFILEHEADER
    if (pcf->Write(auc) == 1L && pcf->Write(auc + 1) == 1L)
      throw std::make_pair("RImage::SaveDib: Unable to write type field of bitmap file header.", -2);

    if (pcf->Write(&dfh.ulSize) == 1)
      throw std::make_pair("RImage::SaveDib: Unable to write size field of bitmap file header.", -3);

    if (pcf->Write(&dfh.usReserved1) == 1L)
      throw std::make_pair("RImage::SaveDib: Unable to write reserved1 field of bitmap file header.", -4);

    if (pcf->Write(&dfh.usReserved2) == 1L)
      throw std::make_pair("RImage::SaveDib: Unable to write reserved2 field of bitmap file header.", -5);

    if (pcf->Write(&dfh.ulOffBits) == 1L)
      throw std::make_pair("RImage::SaveDib: Unable to write offset to bits field of bitmap file header.", -6);


    DIBHEADER	dh;
    dh.ulSize			= 40L;
    dh.usPlanes			= 1;
    switch (m_type)
    {
      case BMP8RLE:
        dh.ulCompression	= 1;	// BI_RLE8.
        dh.ulSizeImage		= m_ulSize;
        break;
      default:
        dh.ulCompression	= 0L;
        // Can't use ulSize b/c our buffer
        // may have a pitch that does not match
        // a DIB pitch.
        dh.ulSizeImage		= lDibPitch * m_sHeight;
        break;
    }

    dh.lXPelsPerMeter	= 50L;
    dh.lYPelsPerMeter	= 50L;
    dh.ulClrUsed		= 0L;
    dh.ulClrImportant	= 0L;

    if (pcf->Write(&dh.ulSize) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write size field of bitmap info header.", -7);

    int32_t	lWidth	= (int32_t)m_sWidth;
    if (pcf->Write(&lWidth) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write width field of bitmap info header.", -8);

    int32_t	lHeight	= (int32_t)m_sHeight;
    if (pcf->Write(&lHeight) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write height field of bitmap info header.", -9);

    if (pcf->Write(&dh.usPlanes) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write planes field of bitmap info header.", -10);

    if (pcf->Write(&m_sDepth) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write bit count field of bitmap info header.", -11);

    if (pcf->Write(&dh.ulCompression) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write compression field of bitmap info header.", -12);

    if (pcf->Write(&dh.ulSizeImage) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write size of image field of bitmap info header.", -13);

    if (pcf->Write(&dh.lXPelsPerMeter) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write horz pixels per meter field of bitmap info header.", -14);

    if (pcf->Write(&dh.lYPelsPerMeter) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write vert pixels per meter field of bitmap info header.", -15);

    if (pcf->Write(&dh.ulClrUsed) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write colors used field of bitmap info header.", -16);

    if (pcf->Write(&dh.ulClrImportant) != TRUE)
      throw std::make_pair("RImage::SaveDib: Unable to write colors important field of bitmap info header.", -17);

    if (m_pPalette)
      if (pcf->Write(m_pPalette->m_pData, m_pPalette->m_ulSize) != (int32_t)m_pPalette->m_ulSize)
        throw std::make_pair("RImage::SaveDib: Unable to write palette.", -20);

    if (dh.ulCompression)
    {
      if (pcf->Write(m_pData, m_ulSize) != (int32_t)m_ulSize) // Write in one big kerchunk.
        throw std::make_pair("RImage::SaveDib: Unable to write all the compressed bits.", -19);
    }
    else // not compressed . . .
    {
      // Upside down way.
      // Write the dib a line at a time and flip it upside down (which is really right side up)
      lHeight	= (int32_t)m_sHeight;
      for (int32_t l = lHeight - 1L; l >= 0L; l--)
        if (pcf->Write(m_pData + (l * m_lPitch), lDibPitch) != lDibPitch)
          throw std::make_pair("RImage::SaveDib: Unable to write all the bits.", -19);
    }
  }
  catch (std::pair<const char*, int> data) // If any errors occurred . . .
  {
    TRACE(data.first);
    return data.second;
  }

  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::Save
//
// Description:
//		Writes out any image format except special cases of 
//		images that use pSpecial.  This function writes out
//		the image data and if you supply a size for the 
//		pSpecial buffer it will write that out also.  As long
//		as your pSpecial pointer doesn't point to data containing
//		other pointers, this will work.  If your pSpecial pointed
//		to another RPal for example, then it would not properly
//		save that data since RPal contains a pData pointer.  For
//		that special case you should write your own save function
//		for your special image type and first open a RFile and
//		call Save with that open RFile and a ulSpecialSize of 0
//		which will write the contents of the image except the
//		pSpecial buffer, the write your pSpecial data before
//		closing the RFile.
//
// Parameters:
//		pszFilename = filename of the image to be saved
//		ulSpecialSize = The size in bytes of the buffer pointed to
//						    by pSpecial.  
//
//	Returns:
//		SUCCESS if the file was saved successfully
//		FAILURE if there was an error - TRACE messages will help
//			     pinpoint the failure.
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::Save(char const * pszFilename) const
{
  RFile cf;
  int16_t sReturn = SUCCESS;

  if (cf.Open(pszFilename, "wb", RFile::LittleEndian) != SUCCESS)
  {
    TRACE("RImage::Save - could not open file for output");
    return FAILURE;
  }

  sReturn = Save(&cf);

  cf.Close();

  return sReturn;
}

//////////////////////////////////////////////////////////////////////
//
// RImage::Save
//
// Description:
//		Writes out any image format except special cases of 
//		images that use pSpecial.  This function writes out
//		the image data and if you supply a size for the 
//		pSpecial buffer it will write that out also.  As long
//		as your pSpecial pointer doesn't point to data containing
//		other pointers, this will work.  If your pSpecial pointed
//		to another RPal for example, then it would not properly
//		save that data since RPal contains a pData pointer.  For
//		that special case you should write your own save function
//		for your special image type and first open a RFile and
//		call Save with that open RFile and a ulSpecialSize of 0
//		which will write the contents of the image except the
//		pSpecial buffer, the write your pSpecial data before
//		closing the RFile.
//
//		This function assumes the the RFile object referrs to
//		an open file and will add to the file in the current location.
//		It will not close the file when it is finished so that you
//		could potentially add your own data following the image data.
//
// Parameters:
//		RFile* pcf = the open RFile to which to write the data
//
//	Returns:
//		SUCCESS if the file was saved successfully
//		FAILURE if there was an error - TRACE messages will help
//			     pinpoint the failure.
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::Save(RFile* pcf) const
{
  uint32_t ulFileType = IMAGE_MAGIC_NUMBER;
  uint32_t	ulCurrentVersion = IMAGE_CURRENT_VERSION;

  if (!pcf || !pcf->IsOpen())
  {
    TRACE("RImage::Save - RFile pointer does not refer to an open file");
    return FAILURE;
  }

  pcf->ClearError();
  pcf->Write(&ulFileType);
  pcf->Write(&ulCurrentVersion);
  pcf->Write(reinterpret_cast<const uint32_t*>(&m_type));
  pcf->Write(reinterpret_cast<const uint32_t*>(&m_typeDestination));
  pcf->Write(&m_ulSize);
  pcf->Write(&m_sWinWidth);
  pcf->Write(&m_sWinHeight);
  pcf->Write(&m_sWidth);
  pcf->Write(&m_sHeight);
  pcf->Write(&m_sWinX);
  pcf->Write(&m_sWinY);
  pcf->Write(&m_lPitch);
  pcf->Write(&m_sDepth);

  if (m_pData)
  {
    uint16_t usFlag = 1;
    pcf->Write(&usFlag);
    WritePixelData(pcf);
  }
  else
  {
    uint16_t usFlag = 0;
    pcf->Write(&usFlag);
  }

  if (m_pPalette)
  {
    uint16_t usOne = 1;
    pcf->Write(&usOne);
    m_pPalette->Save(pcf);
  }
  else
  {
    uint16_t usZero = 0;
    pcf->Write(&usZero);
  }

  // Call the special Save function for this type if any
  SAVEFUNC csf = GETSAVEFUNC(m_type);
  if (csf)
    return (*csf)(const_cast<RImage*>(this), pcf/*, ulCurrentVersion*/);
  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// WritePixelData
//
// Description:
//		Private function called by Save to write the data in one of
//		two ways.  If the buffer is larger than the image, then it
//		must write the image line by line starting at the lXPos, lYPos
//		position in the buffer.  If a larger buffer is not being used, 
//		then it writes the image data in one chunk.
//
// Parameters:
//		pcf = pointer to an open RFile where the data will be written
//
// Returns:
//		SUCCESS if the pixel data was written correctly
//		FAILURE otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::WritePixelData(RFile* pcf) const
{
  uint8_t* pLineData = nullptr;

  int32_t divide;
  switch(m_sDepth)
  {
    default:
      divide = 1;
      break;
    case 16:
      divide = 2;
      break;
    case 24:
      divide = 3;
      break;
    case 32:
      divide = 4;
      break;
  }

  if (m_sWidth <= m_sWinWidth &&
      m_sHeight <= m_sWinHeight)
  {
    if (pcf->Write((uint16_t*) m_pData, m_ulSize/divide) != (int32_t) m_ulSize/divide)
    {
      TRACE("RImage::WritePixelData - Error writing %d-bit pixel data", 8 * divide);
      return FAILURE;
    }
  }
  else
  {
    int32_t	lYPos		= (int32_t)m_sWinY;
    int32_t	lXPos		= (int32_t)m_sWinX;
    int32_t	lHeight	= (int32_t)m_sWinHeight;
    int32_t	lDepth	= (int32_t)m_sDepth;

    int32_t l;
    int32_t lBytesPerLine = (((int32_t)m_sWinWidth * lDepth) / 8);
    if (m_sDepth < 8 && (((int32_t)m_sWinWidth * lDepth) % 8) > 0)
      lBytesPerLine++;

    for (l = lYPos; l < lYPos + lHeight; l++)
    {
      pLineData = m_pData + (lYPos * m_lPitch) + ((lXPos * lDepth) / 8);
      if (pcf->Write((RPixel24*) pLineData, lBytesPerLine/divide) != lBytesPerLine/divide)
      {
        TRACE("RImage::WritePixelData - Error writing %d-bit line %d of image", 8 * divide, l-lYPos);
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// Load
//
// Description:
//		Loads any standard image format including any straight
//		forward pSpecial data that was saved.  If there is a special
//		Save function for pSpecial data that contained pointers to
//		other data, then you will have to use the special load function
//		as well.
//
// Parameters:
//		pszFilename = filename of the IMG file to load
//
// Returns:
//		SUCCESS if the file was loaded correctly
//		FAILURE if there was an error - TRACE messages will help
//				  pinpoint the error.
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::Load(char const * pszFilename)
{
  RFile cf;
  int16_t sReturn = SUCCESS;

  if (cf.Open(pszFilename, "rb", RFile::LittleEndian) != SUCCESS)
  {
    TRACE("RImage::Load - could not open file for input");
    return FAILURE;
  }

  sReturn = Load(&cf);

  cf.Close();

  return sReturn;
}

int16_t RImage::Load(RFile* pcf)
{	
  int16_t sReturn = SUCCESS;

  if (pcf && pcf->IsOpen())
  {
    // Load image dependent on version.
    sReturn = RImageFile::Load(this, pcf);
  }
  else
  {
    TRACE("RImage::Load - RFile pointer does not refer to an open file");
    sReturn = FAILURE;
  }

  // If the file was loaded in as a different type than the
  // desired destination type, then convert it.
  if (m_type != m_typeDestination && m_typeDestination != NOT_SUPPORTED &&
      Convert(m_typeDestination) != m_typeDestination)
    TRACE("RImage::Load - Convert to Destination type %d failed", m_typeDestination);

  return sReturn;
}

//////////////////////////////////////////////////////////////////////
//
// ReadPixelData
//
// Description:
//		Private function called by Load to read the data in one of
//		two ways.  If the buffer is larger than the image, then it
//		must read the image line by line and put it at the lXPos, lYPos
//		position in the buffer.  If a larger buffer is not being used,
//		then it reads the image data in one chunk.
//
// Parameters:
//		pcf = pointer to an open RFile where the data will be read
//
// Returns:
//		SUCCESS if the pixel data was read correctly
//		FAILURE otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RImage::ReadPixelData(RFile* pcf)
{
  uint8_t* pLineData = nullptr;
  int32_t divide;
  switch(m_sDepth)
  {
    default:
      divide = 1;
      break;
    case 16:
      divide = 2;
      break;
    case 24:
      divide = 3;
      break;
    case 32:
      divide = 4;
      break;
  }

  if (m_sWidth <= m_sWinWidth &&
      m_sHeight <= m_sWinHeight)
  {
    if (pcf->Read((uint16_t*) m_pData, m_ulSize/divide) != (int32_t) m_ulSize/divide)
    {
      TRACE("RImage::ReadPixelData - Error reading %d-bit pixel data", 8 * divide);
      return FAILURE;
    }
  }
  else
  {
    int32_t	lYPos		= (int32_t)m_sWinY;
    int32_t	lXPos		= (int32_t)m_sWinX;
    int32_t	lHeight	= (int32_t)m_sWinHeight;
    int32_t	lDepth	= (int32_t)m_sDepth;

    int32_t lBytesPerLine = ((int32_t)m_sWinWidth * lDepth) / 8;
    if (m_sDepth < 8 && (((int32_t)m_sWinWidth * lDepth) % 8) > 0)
      lBytesPerLine++;

    for (int32_t l = lYPos; l < lYPos + lHeight; l++)
    {
      pLineData = m_pData + (lYPos*m_lPitch) + ((lXPos*lDepth)/8);
      if (pcf->Read((RPixel24*) pLineData, lBytesPerLine/divide) != lBytesPerLine/divide)
      {
        TRACE("RImage::ReadPixelData - Error reading %d-bit line %d of image", 8 * divide, l-lYPos);
        return FAILURE;
      }
    }
  }

  return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
