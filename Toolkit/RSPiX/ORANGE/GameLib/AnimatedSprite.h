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
//	ANIMSPRT.H
//   
//	Created on 		3/15/95 BRH 
// Implemented on 3/21/95 BRH
//
// The RAnimSprite class is derived from the Sprite class.  It adds
//	animation capabilities to the basic sprite.
//
//	History:
//
//		10/31/96 BRH	Changed names from CAnimSprite to RAnimSprite for
//							the new RSPiX naming convention.
//
//		11/01/96	JMI	Changed:
//							Old label:		New label:
//							=========		=========
//							CImage			RImage
//
//////////////////////////////////////////////////////////////////////
#ifndef animsprt_h
#define animsprt_h

#include <BLUE/System.h>
// Orange includes
#include <ORANGE/GameLib/Sprite.h>
#include <ORANGE/File/RFile.h>

#ifdef _MSC_VER
	#pragma warning (disable:4200) // disable warning about zero-length arrays
#endif

// These masks are used with the animation flags
#define ANIMSPRITE_FLAG_LOOP		0x0001	// 1=loop anim, 0=stop at end
#define ANIMSPRITE_FLAG_NOSKIP	0x0002	// 1=can't skip frames, 0=can
#define ANIMSPRITE_FLAG_OFFSET	0x0004	// 1=add anim offsets to sprite
#define ANIMSPRITE_FLAG_ROTATION	0x0008	// 1=modify rotation of sprite
#define ANIMSPRITE_FLAG_SCALE		0x0010	// 1=modify sprite's scale
#define ANIMSPRITE_FLAG_END		0x0100	// 1=ending frame, 0=other
#define ANIMSPRITE_FLAG_KEY		0x0200	// 1=key frame, 0=other

#define ANIMSPRITE_LAST_FRAME		-1
#define ANIMSPRITE_FIRST_FRAME	-2
#define ANIMSPRITE_WAITING			-3

#define ANIMSPRITE_CURRENT_VERSION	0
#define ANIMSPRITE_MAGIC_NUMBER				0x4d494e41	//"ANIM" in the file

// Error codes
#define ANIM_INVALID_FRAME			1			// Current anim does not contain 

// A FRAME contains offsets that describe how the hotspot should be moved
// in order to get to the position at which the specified IMAGE should be
// displayed.  The "hold" value tells how many milliseconds this FRAME should
// be held for before moving on to the next frame.  The sPicIndex is stored
// in the animation file to associate this frame with one of the animation's
// pictures.  Once the pictures are loaded, the pImage pointer is set to the
// image for this frame.  This way multiple frames can use the same picture.
struct FRAME {
	int16_t		sOffsetX;	// amount to add to sprite's X position for this frame
	int16_t		sOffsetY;	// amount to add to sprite's Y position for this frame
	int16_t		sOffsetZ;	// amount to add to sprite's Z position for this frame
	int16_t		sRotDeg;		// Rotation in Degrees
   int32_t		lScaleWidth;// Scaled width for this frame
   int32_t		lScaleHeight;//Scaled height for this frame
	uint32_t		ulFlags;		// Flags for this frame
	int16_t		sHold;		// number of milliseconds to hold this frame
	int16_t		sPicIndex;	// Animation's picture index for this frame (used to load)
	RImage*	pImage;		// pointer to the image for this frame
};
										

class RAnimSprite : public RSprite
{
	private:
		int16_t		m_sVersion;			// Animation version number (to mark .ani files)
		int16_t		m_sAllocatedPics;	 // Keep track of number allocated (user can't change)
	public:							// so Load can check the header format
		int16_t		m_sNumFrames;		// Number of frames in this animation
		int16_t		m_sNumPictures;	// Number of RImages in this animation
		int16_t		m_sLoopToFrame;	// -1 if no loop, or frame number to start loop
		uint32_t		m_ulAnimFlags;		// Animation flags and status
      int32_t		m_lTimer;			// timer used for animating
		int16_t		m_sCurrFrame;		// current frame in the animation
		FRAME*	m_aFrames;			// Array of frame structures
		RImage**	m_apPictures;		// Array of RImage*

	public:
		RAnimSprite();
		~RAnimSprite();

		// Load an animation from a .ani file with this filename
		int16_t Load(char* pszFilename);

		// Load an animation from an open RFile
		int16_t Load(RFile* pcf);

		// Save this animation to a .ani file and give it this filename
		int16_t Save(char* pszFilename);

		// Save this animation to an open RFile
		int16_t Save(RFile* pcf);

		// Go to the next frame of animation if it is time
		int16_t Animate(void);

		// Set to specified frame in current animation.
		// Updates m_sAnimFrameNum, m_sAnimFrameNum and m_usAnimFlags.
 		// NOTE: If frame is too high then ending frame is used.
		// NOTE: A sequential search for specified frame is required!
		// NOTE: If ANIMSPRITE_FLAGS_NOSKIP flag is set, nearest key-frame
		// is used (nearest before, nearest after, or just nearest?)
		int16_t SetFrame(int16_t sFrameNum);	

		// Go on to next frame of current anim.  If anim was on ending frame
		// then this will wrap back around to frame 1 (not frame 0!)
		// Updates m_sAnimFrameNum, m_sAnimFrameNum and m_usAnimFlags.
		int16_t NextFrame(void);

		// Go on to next key frame of current anim.  If anim was on ending frame
		// then this will wrap back around to frame 1 (not frame 0!)
		// Updates m_sAnimFrameNum, m_sAnimFrameNum and m_usAnimFlags.
		int16_t NextKeyFrame(void);

		// Go on to previous frame of current anim.  If anim was on first frame
		// then this will wrap around to the end frame
		int16_t PrevFrame(void);

		// Function to allocate space for given number of frames
		int16_t AllocateFrames(int16_t sNumFrames);

		// Function to allocate space for given number of RImage pointers
		int16_t AllocatePictures(int16_t sNumPictures);

		// Function to free frames if you allocated them
		int16_t FreeFrames(void);

		// Function to free pictues if you allocated them
		int16_t FreePictures(void);

		// Return a pointer to the current frame's RImage
		RImage* GetFrameImage(void)
		{
			if (m_sCurrFrame >= 0 && m_sCurrFrame < m_sNumFrames)
				return m_aFrames[m_sCurrFrame].pImage;
			else
				return nullptr;
		};
		
		// Return a pointer to a specified frame's RImage
		RImage* GetFrameImage(int16_t sFrameNumber)
		{
			if (sFrameNumber >= 0 && sFrameNumber < m_sNumFrames)
				return m_aFrames[sFrameNumber].pImage;
			else
				return nullptr;
		}
		
		// Return a pointer to the current frame
		FRAME* GetFrame(void)
		{
			return &(m_aFrames[m_sCurrFrame]);
		};

		// Get the current frame's hold time
		int16_t GetFrameTime(void)
		{
			return m_aFrames[m_sCurrFrame].sHold;
		};

	private:
		// Function used by Save to loop through and save all of the images
		int16_t WritePictures(RFile* pcf);

		// Function used by Save to loop through and save all of the frames
		int16_t WriteFrames(RFile* pcf);

		// Function used by Load to loop through and load all of the images
		int16_t ReadPictures(RFile* pcf);

		// Function used by Load to loop through and load all of the frames
		int16_t ReadFrames(RFile* pcf);
};

#endif //animsprt_h

//*****************************************************************************
// EOF
//*****************************************************************************
