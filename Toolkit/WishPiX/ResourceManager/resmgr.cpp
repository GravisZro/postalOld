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
// ResMgr.cpp
//
// Resource manager class which allows multiple access to shared
// resources to save load time and memory.  
//
// History:
//		12/09/96 BRH	Started this class using STL containers to
//							keep track of resources that have been loaded.
//
//		12/17/96 BRH	Added additional resource types for Get and 
//							Release.  Also added statistics tracking and
//							reporting.  The statistics report can be used
//							to generate the SAK files.
//
//		12/18/96 BRH	Added CreateSak() function to read in the 
//							text script file generated by Statistics()
//							and create a SAK file.  Also added OpenSak()
//							function to read in the SAK file, create the
//							internal directory and have an open RFile 
//							available from which the resources will be read.
//							Once a SAK file has been opened, it is assumed
//							that all resource names refer to the SAK file, 
//							so accessing resources that are not in the SAK
//							file will be an error, it will not attempt to
//							read the resource from a disk file.  CloseSak()
//							will close the SAK file and resource names will
//							go back to looking at disk files.
//
//		12/19/96 BRH	Added RFont as one of the supported resources.
//							Also removed cout that was using during testing
//							with the console app.
//
//		01/06/97 BRH	Added generic void type of resource which loads
//							a buffer of bytes which can then be plugged into
//							an RFile as a memory file as one example.
//							Also now frees up all resources when the 
//							resource manager is destroyed.
//
//		01/10/97 BRH	Added a TRACE message to FreeAllResources that
//							informs the user if they have not released
//							all of the resources before the resource manager
//							object was destroyed.  It will still free the
//							resources as it did before but they will get a
//							list of the resources they should have freed.
//							Changed CreateSak to copy whatever type of
//							file using the CResVoid object rather than
//							specific loads and saves for each type of
//							resource since the byte-by-byte copy will work
//							for every type of resource.  This eliminates
//							the switch statement and doesn't require
//							the addition of a load and save for each new
//							type of resource added to the resource manager.
//
//		01/29/97	JMI	Added RSample as one of the supported resources.
//
//		01/30/97 BRH	Fixed a problem in the Get functions where if
//							an error occurred such as "file not found" when
//							doing a get, the filename was added to the 
//							resource manager but no resource so the next time
//							someone did a Get of that filename, it would
//							return them a nullptr resource.  Now if there is
//							an error on the first get, the resource name
//							is removed from the list.
//
//		02/03/97 BRH	Added a function FromSystempath() that takes
//							a resource name and converts it into a system
//							pathname when loading individual files.  It is
//							similar to the FromSak() function.  The Get
//							functions now use this FromSystempath funciton
//							to prepend the base path to the resource name
//							in order to load files.  Also added more
//							checking to SetBasePath function to make sure
//							its in the correct system path format.
//
//		02/03/97 BRH	Added RAttributeMap as a supported type.
//
//		02/04/97	JMI	Changed Get() for RImage to just use Load().
//							Was using LoadDib() for filenames with .bmp
//							extensions.  Now Load() supports both .IMG's and
//							*.BMP's.
//
//		02/04/97	JMI	Added rspGetResource(), rspReleaseResource(),
//							and helper templates (ResMgr_CreateResource,
//							ResMgr_DestroyResource, ResMgr_LoadResource,
//							and ResMgr_SaveResource).  Also added member
//							to a member version of Get() in RResMgr that
//							supports just the kind of functionality that
//							the template functions need.
//							To make this generic resource interface 
//							possible, CResourceBlock had to be modified to 
//							include pointers to Create, Destroy, Load, and
//							Save (aslo, m_usType was no longer necessary).
//							Dropped support for particular types and got rid
//							of the void resource.
//
//		03/24/97 BRH	Added checking for duplicate files when creating
//							a SAK file, in addition to the checking done
//							when creating the sak script files.  This will
//							allow two or more script files to be concatinated
//							together and fed in to create a sak file without
//							duplicates.  
//
//		03/25/97 BRH	Added calls to NormalizeResName() in Get and
//							CreateSak so that that resource names that are
//							stored in the header, and the resource names
//							requested will match regardless of case or
//							which / or \ slash character was used in the
//							pathname.
//
//		05/08/97	JMI	Added constructor arguments to pair<> 
//							declarations since there appears to be no default
//							constructor for pair in the STL that comes with
//							VC <= 4.1.
//
//		05/30/97	JMI	Added RESMGR_VERBOSE.  Which, when defined, 
//							causes the resmgr to output TRACEs when a file
//							is not found.  Although we all love this feature
//							for debugging purposes, it gets bad when we're
//							loading a file only if it exists.
//
//		06/11/97 BRH	Added ReleaseAndPurge(void*) to purge a single given
//							resource who's reference count is zero.  If it
//							is not zero, then it effectively acts as
//							a Release() call.
//
//		06/29/97 MJR	Minor changes for mac compatibility.  Also fixed
//							bug in mac-specific code (string truncation was
//							incorrect).
//
//		07/03/97 MJR	MAJOR changes for mac compatibility.
//
//		07/17/97	JMI	Added rspGetResourceInstance(), GetInstance(),
//							and rspReleaseResourceInstance().
//
//		07/18/97	JMI	Forgot to NormalizeResName() in GetInstance().
//
//		08/28/97 BRH	Added a flag and a trace to show uncached loads
//							if the flag is set it will print a trace of 
//							any resources that have to loaded from a disk.
//							This can be used in a game to make sure that
//							things aren't loading during gameplay if they
//							were meant to be preloaded.
//
//		09/03/97	JMI	The check for failure in GetInstance() that
//							deallocated an object whose file could not be
//							opened or loaded may not have caught cases where
//							the Load() for the particular type returned other
//							than -1.
//
//		09/03/97	JMI	GetInstance(), in the case of failure, was 
//							attempting to delete the allocated instance with
//							delete as a void* instead of using the provided
//							destroy func.
//
//		09/04/97	JMI	GetRes() was clearing pfnDestroy in the case
//							that GetInstance() succeeded or failed.  It
//							should've just been in the success case.  Fixed.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include "resmgr.h"


//////////////////////////////////////////////////////////////////////
// Macros.
//////////////////////////////////////////////////////////////////////

// Size of buffer used to transfer resource data from original file
// to SAK when creating SAK files.
#define TRANSFER_BUF_SIZE	4096	// Bytes.

// Endian nature the SAK file uses.
#define SAK_FILE_ENDIAN		RFile::LittleEndian


//////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////

RResMgr::RResMgr(void)
{
	m_bTraceUncachedLoads = false;
}

//////////////////////////////////////////////////////////////////////
//
// Destructor
//
//////////////////////////////////////////////////////////////////////

RResMgr::~RResMgr(void)
{
	// Closes the SAK file if one was open.
	CloseSak();
	// Free resources even if they are in use
	FreeAllResources();
}

//////////////////////////////////////////////////////////////////////
//
// Get
//
// Description:
//		This gets a resource that either already exists or is in the 
//		open SAK or on disk, if no SAK.  It creates an object instance
//		(if it does not exist) via fnCreate and loads the given file 
//		via fnLoad and then returns the resource instance to you.
//
// Parameters:
//		See below.
//		
//
// Returns:
//		SUCCESS if the resource was set to hRes
//		negative error code otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RResMgr::Get(									// Returns 0 on success.
	RString strFilename,								// In:  Resource name
	void** hRes,										// Out: Pointer to resource returned here
	RFile::Endian	endian,							// In:  Endian nature of resource file
	GenericCreateResFunc* pfnCreate,				// In:  Pointer to "create" function object
	GenericDestroyResFunc* pfnDestroy,			// In:  Pointer to "destroy" function object
	GenericLoadResFunc* pfnLoad)					// In:  Pointer to "load" function object
	{
	int16_t sReturn = SUCCESS;

	// Map iterator (one of the best things about STL is how readable it is)
	pair<resclassMap::iterator, bool> p(m_map.begin(), false);

	// Create a temporary resource block.  Be carefull to set ONLY THE NAME
	// at this point.  Any other values should be set only if it turns out
	// that we need to create and load the requested resource!
	CResourceBlock resBlock;
	NormalizeResName(&strFilename);
	resBlock.m_strFilename	= strFilename;

	// p is a pair of <iterator, bool> where p.first (iterator) is
	// referencing the map either where the strFilename was found
	// or where it was inserted if it did not previously exist.
	p = m_map.insert(resclassMap::value_type (strFilename, resBlock));

	// If the requested resource does not already exist, create the resource now and load it
	if ((*(p.first)).second.m_vpRes == nullptr)
		{
		sReturn = GetInstance(	// Returns 0 on success.
				strFilename,		// In:  Resource name
				hRes,					// Out: Pointer to resource returned here
				endian,				// In:  Endian nature of resource file
				pfnCreate,			// In:  Pointer to "create" function object
				pfnDestroy,			// In:  Pointer to "destroy" function object
				pfnLoad);			// In:  Pointer to "load" function object
		if (sReturn == 0)		
			{
			// Fill in the resource block.
			(*(p.first)).second.m_vpRes = *hRes;
			(*(p.first)).second.m_pfnDestroy	= pfnDestroy;

			// Clear pointer so that the object won't be deleted on exit from this function.
			// The responsibility for deleting the object now lies with the resource block.
			pfnDestroy = 0;

			// Add to the access order list
			m_accessList.push_back(strFilename);	
#ifdef _DEBUG
			if (m_bTraceUncachedLoads)
				TRACE("RResMgr::Get - Loading uncached resource %s\n", (char *) strFilename);
#endif //_DEBUG
			}
		else
			{
			#ifdef RESMGR_VERBOSE
				TRACE("RResMgr::Get - Break Yo Selfen hosen!  GetInstance() failed.\n");
			#endif // RESMGR_VERBOSE
			}
		}

	if (sReturn == SUCCESS)
		{
		(*(p.first)).second.m_sRefCount++;
		(*(p.first)).second.m_sAccessCount++;
		*hRes = (*(p.first)).second.m_vpRes;
		// Add a mapping indexed by pointer to make it easy to find the
		// name of this resource later by using the resource pointer.
		m_ptrMap[*hRes] = strFilename;
		}
	else
		{
		*hRes = nullptr;
		// In this case, m_vpRes is also nullptr, so we don't have to worry about a 
		// double delete.
		m_map.erase(strFilename);
		}

	// Delete the create and load function objects, and POSSIBLY the destroy function,
	// if its pointer hasn't been cleared to 0 (which indicates that responsibility for
	// deleting it no longer lies with this function, but with a newly created ResourceBlock.
	delete pfnCreate;
	delete pfnDestroy;	// Might be 0 (which is safe for delete)!  See comments above!
	delete pfnLoad;
	
	return sReturn;
	}

//////////////////////////////////////////////////////////////////////
//
// GetInstance
//
// Description:
//		This gets a resource that either already exists or is in the 
//		open SAK or on disk, if no SAK.  It creates an object instance
//		(if it does not exist) via fnCreate and loads the given file 
//		via fnLoad and then returns the resource instance to you.
//
// Parameters:
//		See below.
//		
//
// Returns:
//		SUCCESS if the resource was set to hRes
//		negative error code otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RResMgr::GetInstance(						// Returns 0 on success.
	RString strFilename,								// In:  Resource name
	void** hRes,										// Out: Pointer to resource returned here
	RFile::Endian	endian,							// In:  Endian nature of resource file
	GenericCreateResFunc* pfnCreate,				// In:  Pointer to "create" function object
	GenericDestroyResFunc* pfnDestroy,			// In:  Pointer to "destroy" function object
	GenericLoadResFunc* pfnLoad)					// In:  Pointer to "load" function object
	{
	int16_t	sReturn	= SUCCESS;	// Assume success for return.
	// Assume failure for safety.
	*hRes	= nullptr;

	NormalizeResName(&strFilename);

	// Create resource using the specified "create" function
	void* pvInstance = nullptr;
	if ((*pfnCreate)(&pvInstance) == 0)
		{
		RFile		fileNoSak;
		RFile*	pfileSrc	= nullptr;
		// If a SAK file is in use, load it from that, otherwise
		// load it from the disk file.
		if (m_rfSak.IsOpen())
			pfileSrc	= FromSak(strFilename);

		// If SAK file fails try loading from disk.
		if (!pfileSrc)
			{
			if (fileNoSak.Open(/*FromSystempath(strFilename)*/strFilename, "rb", endian ) == 0)
				pfileSrc	= &fileNoSak;
			else
				{
				#ifdef RESMGR_VERBOSE
					TRACE("RResMgr::GetInstance - Break Yo Selfen hosen!  Could not open resource file.\n");
				#endif // RESMGR_VERBOSE
				}
			}

		if (pfileSrc)
			{
			// Set endian for this file.
			pfileSrc->SetEndian(endian);

			// Let resource load itself from the file
			sReturn	= (*pfnLoad)(pvInstance, pfileSrc);

			// Restore endian to SAK file mode (although this may or may not be
			// the SAK file (it can't hurt (can it?) ) ).
			pfileSrc->SetEndian(SAK_FILE_ENDIAN);

			// If our local RFile is open, close it
			if (fileNoSak.IsOpen())
				fileNoSak.Close();
			}
		else
			{
			#ifdef RESMGR_VERBOSE
				TRACE("RResMgr::GetInstance - Break Yo Selfen hosen! Could not get RFile* to resource data.\n");
			#endif // RESMGR_VERBOSE
			sReturn	= FAILURE;
			}

		// If we fail after allocation . . .
		if (sReturn != SUCCESS)
			{
			// Delete the object.
			(*pfnDestroy)(pvInstance);
			}
		else	// Otherwise, we've succeeded.
			{
			// Return the instance.
			*hRes	= pvInstance;
			}
		}
	else
		{
		TRACE("RResMgr::GetInstance - Break Yo Selfen hosen! Error allocating new resource\n");
		sReturn = FAILURE;
		}

	return sReturn;
	}

//////////////////////////////////////////////////////////////////////
//
// Release
//
// Description:
//		Release function for resources.
//
// Parameters:
//		pRes = pointer to resource obtained earlier by Get()
//
// Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

void RResMgr::Release(void* pVoid)
{
	m_map[m_ptrMap[pVoid]].m_sRefCount--;
}

//////////////////////////////////////////////////////////////////////
//
// ReleaseAndPurge
//
// Description:
//		Releases the given resource and then purges it if its reference
//		count is zero.  If it is not zero then it is simply released.
//
// Parameters:
//		pRes = pointer to resource obtained earlier by Get()
//
// Returns:
//		true if it was purged, false if it was only released.
//
//////////////////////////////////////////////////////////////////////

bool RResMgr::ReleaseAndPurge(void* pVoid)
{
	bool bPurged = false;

	m_map[m_ptrMap[pVoid]].m_sRefCount--;

	// If nobody else is using this, then purge it.
	if (m_map[m_ptrMap[pVoid]].m_sRefCount < 1)
	{
		m_map[m_ptrMap[pVoid]].m_sRefCount = 0;
		m_map[m_ptrMap[pVoid]].FreeResource();
		bPurged = true;
		m_map.erase(m_ptrMap[pVoid]);
	}

	return bPurged;
}

//////////////////////////////////////////////////////////////////////
//
// Purge
//
// Description:
//		Deallocates resources with a zero reference count.  
//
// Parameters:
//		none
//
// Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

void RResMgr::Purge(void)
{
	resclassMap::iterator i;
	resclassMap::iterator del;

	i = m_map.begin();

	while (i != m_map.end())
	{
		del = i;
		i++;
		if ((*del).second.m_sRefCount <= 0)
		{
			(*del).second.FreeResource();
			(*del).second.m_sRefCount = 0;
			m_map.erase(del);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//
// FreeAllResources
//
// Description:
//		Deallocates all resources, much like Purge but it deallocates
//		resources even if their reference count is not zero.  This would
//		normally be bad because you are taking away resources that
//		are in use, but this function is to only be used by the
//		destructor so if the resource manager is being destroyed, it
//		cleans up all of the allocated resources.
//
//		It will report in a TRACE message all resources that haven't 
//		been released yet but should have been by this time.
//
// Parameters:
//		none
//
// Returns:
//		none
//
//////////////////////////////////////////////////////////////////////

void RResMgr::FreeAllResources(void)
{
	resclassMap::iterator i;
	
	for (i = m_map.begin(); i != m_map.end(); i++)
	{
		if ((*i).second.m_sRefCount > 0)
			TRACE("RResMgr::FreeAllResources - %s ref count = %d - Releasing anyway\n",
			      (char*) (*i).second.m_strFilename, (*i).second.m_sRefCount);
//			      (char*) (*i).second.m_strFilename.c_str(), (*i).second.m_sRefCount);
		(*i).second.FreeResource();
		(*i).second.m_sRefCount = 0;
	}	
	m_map.erase(m_map.begin(), m_map.end());
}


//////////////////////////////////////////////////////////////////////
//
// Statistics
//
// Description:
//		This function will output a text file giving stastics for
//		each file that was accessed.
//
// Parameters:
//		strStatFile = output filename that will be created.
//
// Returns:
//		SUCCESS if the file was created
//		FAILURE otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RResMgr::Statistics(RString strStatFile)
{
#if PLATFORM_UNIX   // avoid STL on Unix.  --ryan.
    fprintf(stderr, "STUBBED: %s:%d\n", __FILE__, __LINE__);
    return FAILURE;
#else
	int16_t sReturn = SUCCESS;
	ofstream txtout;
	resclassMap::iterator i;
	
	m_duplicateSet.erase(m_duplicateSet.begin(), m_duplicateSet.end());
	txtout.open((char*) strStatFile);
//	txtout.open((char*) strStatFile.c_str());
	if (txtout.is_open())
	{
		txtout << ";\n";
		txtout << "; Resource manager statistics file" << endl;
		txtout << ";" << endl;
		txtout << "; List of files in access order\n";
		txtout << ";------------------------------------------------------------------\n";

		accessVector::iterator j;
		pair <dupSet::iterator, bool> p(m_duplicateSet.begin(), false);
		for (j = m_accessList.begin(); j != m_accessList.end(); j++)
		{
			p = m_duplicateSet.insert((*j));
			// If its not in the set, then its ok to print it out 
			if (p.second)
				txtout << *j << endl;
		}


		txtout << "\n\n; Statistics on each file\n;\n";
		txtout << ";Access\n";
		txtout << ";Count	Filename" << endl;
		txtout << ";------------------------------------------------------------------\n";

		for (i = m_map.begin(); i != m_map.end(); i++)
		{
			txtout << "; " << (*i).second.m_sAccessCount << "\t" << (*i).first << endl;
		}		
		txtout.close();
	}
	else
	{
		TRACE("RResMgr::Statistics - Break Yo Self! Error - unable to open stat file %s\n",
		      (char*) strStatFile);
//		      (char*) strStatFile.c_str());
		sReturn = FAILURE;
	}
	return sReturn;
#endif
}


//////////////////////////////////////////////////////////////////////
//
// CreateSak
//
// Description:
//		Creates a SAK file from the script file that is written out
//		by the Statistics() function.  The SAK file is a single file
//		that contains several resources.  It has a header that contains
//		a directory of resource names that map to offsets within the
//		SAK file.  
//
// Parameters:
//		strScriptFile = filname of script file created by Statistics()
//		strSakFile = filename of SAK file to be created
//
// Returns:
//		SUCCESS if the SAK file was created
//		FAILURE otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RResMgr::CreateSak(RString strScriptFile, RString strSakFile)
{
#if PLATFORM_UNIX   // avoid STL on Unix.  --ryan.
    return FAILURE;
#else
	int16_t sReturn = SUCCESS;
	ifstream script;
	RString line;
	RString resname;
//	uint16_t usType;
	char buffer[256];
	RFile sak;

	script.open((char*) strScriptFile);
//	script.open((char*) strScriptFile.c_str());
	sak.Open((char*) strSakFile, "wb", SAK_FILE_ENDIAN);
//	sak.Open((char*) strSakFile.c_str(), "wb", SAK_FILE_ENDIAN);
	if (script.is_open() && sak.IsOpen())
	{
		// Load in all of the resource names to be added to SAK file
		while (!script.eof())
		{
			script.getline(buffer, 256);
			resname = buffer;
			if (resname[(int32_t) 0] != ';' && resname[(int32_t) 0] != ' ' && resname.GetLen() > 0)
			{
				NormalizeResName(&resname);
				m_LoadList.push_back(resname);
				m_DirectoryMap.insert(dirMap::value_type(resname, 0));
			}
/*
			script >> line;
			if (line[(int32_t) 0] == ';' || line.GetLen() == 0)
				script.getline(buffer, 256);
			else
			{
				resname = line;
				script.getline(buffer, 256);
				NormalizeResName(&resname);
				m_LoadList.push_back(resname);
//				m_TypeList.push_back(usType);
				m_DirectoryMap.insert(dirMap::value_type(resname, 0));
			}	
*/
		}

		// Write SAK header as a placeholder
		WriteSakHeader(&sak);

		// Open each type of resource and write it to the SAK file
		// keeping track of its offset in the file
		accessVector::iterator iFilename;
//		typeVector::iterator iType = m_TypeList.begin();
		RFile	fileRes;
		uint8_t	au8Transfer[TRANSFER_BUF_SIZE];
		int32_t	lNumBytes;
		pair <dupSet::iterator, bool> p(m_duplicateSet.begin(), false);
		m_duplicateSet.erase(m_duplicateSet.begin(), m_duplicateSet.end());

		for (iFilename = m_LoadList.begin(); iFilename != m_LoadList.end() && sReturn == SUCCESS; iFilename++) //, iType++)
		{
			// Insert this filename into the set.  If its already in there, p.second
			// will be false.
			p = m_duplicateSet.insert((*iFilename));

			// If the file hasn't been included in the sak yet, then its OK to include it now.
			if (p.second)
			{
				// Set offset position in directory map for this file
				m_DirectoryMap[(*iFilename)] = sak.Tell();

				// Open disk file . . .
//				if (fileRes.Open( FromSystempath((char*) (*iFilename).c_str() ), "rb", SAK_FILE_ENDIAN) == 0)
				if (fileRes.Open( FromSystempath((char*) (*iFilename) ), "rb", SAK_FILE_ENDIAN) == 0)
				{
					do
					{
						// Read chunk.
						lNumBytes	= fileRes.Read(au8Transfer, sizeof(au8Transfer) );
						// If we got anything . . .
						if (lNumBytes > 0)
						{
							// Write chunk.
							sak.Write(au8Transfer, lNumBytes);
						}

					} while ( (fileRes.IsEOF() == FALSE) && (fileRes.Error() == FALSE) && (sak.Error() == FALSE) );

					fileRes.Close();
				}
				else
				{
					TRACE("CreateSak(): fileRes.Open() failed.\n");
					sReturn	= FAILURE;
				}
			}
		}

		// Go back to the beginning of the SAK file and rewrite
		// the header so that the proper offset values in the 
		// directory will be written.

		sak.Seek(0, SEEK_SET);
		WriteSakHeader(&sak);

		sak.Close();

		// Free the memory for the maps and lists
		m_LoadList.erase(m_LoadList.begin(), m_LoadList.end());
//		m_TypeList.erase(m_TypeList.begin(), m_TypeList.end());
		m_DirectoryMap.erase(m_DirectoryMap.begin(), m_DirectoryMap.end());
	}
	else
	{
		TRACE("RResMgr::CreateSak - Break Yo Self! Error opening script file %s or sak file %s", 
		      (char*) strScriptFile, (char*) strSakFile);
//		      (char*) strScriptFile.c_str(), (char*) strSakFile.c_str());
		sReturn = FAILURE;
	}
	return sReturn;
#endif
}

//////////////////////////////////////////////////////////////////////
//
// WriteSakHeader
//
// Description:
//		Write the header and directory for the SAK file at the
//		current location in the given RFile.  This will be called
//		twice, one to write the header as a placeholder for the file
//		and then after the data has been written to the file, it will
//		be called again to write the directory at the beginning of 
//		the file which will now contain the correct offsets as well
//		as the resource names.
//
// Parameters:
//		RFile* prf = pointer to open RFile
//
// Returns:
//		SUCCESS if the header was written correctly
//		FAILURE otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RResMgr::WriteSakHeader(RFile* prf)
{
	int16_t sReturn = SUCCESS;
   uint32_t ulFileType = SAK_MAGIC_NUMBER;
	uint32_t ulCurrentVersion = SAK_CURRENT_VERSION;
	uint16_t usNumPairs = m_DirectoryMap.size();
	dirMap::iterator m;

	if (prf && prf->IsOpen())
	{
		prf->ClearError();
		prf->Write(&ulFileType);
		prf->Write(&ulCurrentVersion);
		prf->Write(&usNumPairs);
		for (m = m_DirectoryMap.begin(); m != m_DirectoryMap.end(); m++)
		{
			// Write resource name
			prf->Write((char*) (*m).first);
//			prf->Write((char*) (*m).first.c_str());
			// Write offset
			prf->Write((*m).second);	
		}
	}
	else
	{
		TRACE("RResMgr::WriteSakHeader - Break Yo Self! Error writing to SAK header\n");
		sReturn = FAILURE;
	}

	return sReturn;
}

//////////////////////////////////////////////////////////////////////
//
// OpenSak
//
// Description:
//		Open a SAK file and read in its directory from the header.  
//		While a SAK file is opened, it is assumed that all resource
//		names refer to resources within the current SAK file.  If
//		a resource is not found in the SAK file, the resource manager
//		will not attempt to load it from its individual disk file.
//
// Parameters:
//		strSakFile = filename of SAK file to be opened
//
// Returns:
//		SUCCESS if the SAK file was opened and the header was read
//		FAILURE otherwise
//
//////////////////////////////////////////////////////////////////////

int16_t RResMgr::OpenSak(RString strSakFile)
{
  uint32_t ulFileType;
  uint32_t ulFileVersion;
  uint16_t usNumPairs;
  uint16_t i;
  char char_buffer[256];
  int32_t lOffset;
  RString strFilename;

  if (m_rfSak.IsOpen() == TRUE)
  {
    TRACE("RResMgr::OpenSak - The currently open SAK file is being closed before loading the new SAK file\n");
    CloseSak();
  }

  try
  {
    if (m_rfSak.Open((char*) strSakFile, "rb", SAK_FILE_ENDIAN) != SUCCESS)
      throw std::string("RResMgr::OpenSak - Error opening sak file ").append((char*)strSakFile);

    m_rfSak.ClearError();

    m_rfSak.Read(&ulFileType);
    if (ulFileType != SAK_MAGIC_NUMBER)
      throw std::string("RResMgr::OpenSak - Not a valid SAK file, magic_number should be 'SAK '");

    m_rfSak.Read(&ulFileVersion);
    if (ulFileVersion != SAK_CURRENT_VERSION)
      throw std::string("RResMgr::OpenSak - This file is version ").append(std::to_string(ulFileVersion)).append("and the current SAK version is ").append(std::to_string(SAK_CURRENT_VERSION));

    m_rfSak.Read(&usNumPairs);
    for (i = 0; i < usNumPairs; i++)
    {
      // Read the filename
      m_rfSak.Read(char_buffer);
      strFilename = char_buffer;
//      std::cout << "found: " << char_buffer << std::endl;
      // Read the offset
      m_rfSak.Read(&lOffset);
      m_SakDirectory.insert(dirMap::value_type (strFilename, lOffset));
      m_SakDirOffset.insert(lOffset);
    }
    // Insert end of SAK file into offset Set container so there is
    // always a next offset to look up.
    m_rfSak.Seek(0, SEEK_END);
    lOffset = m_rfSak.Tell();
    m_SakDirOffset.insert(lOffset);
  }
  catch(std::string message)
  {
    TRACE(message.c_str(), "\n");
    return FAILURE;
  }

  return SUCCESS;
}

//////////////////////////////////////////////////////////////////////
//
// SetBasePath
//
// Description:
//		Sets the base pathname for this instance of the resource
//		manager.  The base path is used when loading resources from
//		their individual files rather than from the SAK file.  This 
//		function checks the basepath passed in (which should be a 
//		system path rather than an RSP path) and makes sure that
//		the separator is correct.
//
//	Parameters:
//		strBasepath = the base path you wish to set - should already
//						  be a system path.  Call rspPathToSystem to convert
//						  an RSP path to a system path before calling this
//						  function.
//
// Returns:
//		none - ASSERTS if the path is too long (> RSP_MAX_PATH)
//
//////////////////////////////////////////////////////////////////////

void RResMgr::SetBasePath(RString strBasepath)
{
	int16_t sLast = strBasepath.GetLen();
	if (sLast > 0)
		sLast--;

	// If base path doesn't end with a slash, add one
	if (strBasepath[(int32_t) sLast] != RSP_SYSTEM_PATH_SEPARATOR)
		strBasepath += RSP_SYSTEM_PATH_SEPARATOR;
	// Make sure it is int16_t enough to work with rspix functions
	ASSERT(strBasepath.GetLen() < RSP_MAX_PATH);
	m_strBasepath = strBasepath;
}

//////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////
