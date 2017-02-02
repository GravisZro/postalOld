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
#ifndef CVIDEO_H
#define CVIDEO_H

#include <cstdint>

//////////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////////

// Error codes.
#define VIDEO_SUCCESS						((int16_t)0)
#define VIDEO_ERR_GENERIC					((int16_t)1)
#define VIDEO_ERR_VIDEO_UNSUPPORTED		((int16_t)2)
#define VIDEO_ERR_CREATE					((int16_t)3)
#define VIDEO_ERR_OPEN						((int16_t)4)
#define VIDEO_ERR_PLAY						((int16_t)5)
#define VIDEO_ERR_PAUSE						((int16_t)6)
#define VIDEO_ERR_RESUME					((int16_t)7)
#define VIDEO_ERR_STOP						((int16_t)8)
#define VIDEO_ERR_CLOSE						((int16_t)9)
#define VIDEO_ERR_ALREADY_CREATED		((int16_t)10)
#define VIDEO_ERR_GETTING_WRECT			((int16_t)11)
#define VIDEO_ERR_RESIZE_FAIL				((int16_t)12)
#define VIDEO_ERR_SET_FRAMES				((int16_t)13)
#define VIDEO_ERR_SET_TIME					((int16_t)14)


// Clipping modes.
#define MODE_NORMAL							((int16_t)0)			// no clipping
#define MODE_CLIP								((int16_t)1)			// clip
#define MODE_STRETCH							((int16_t)2)			// stretch/shrink to fit

// Play modes.
#define PLAY_ALL								((int16_t)0)		// Play the entire video from begin to end
#define PLAY_SELECTION_TIME				((int16_t)1)		// Play a selection specified in milliseconds
#define PLAY_SELECTION_FRAME				((int16_t)2)		// Play a selection specified in frames

class CVideo
	{
	// Constructor/Destructor
	public:
		// Default constructor.
		CVideo(void);

		// Extended constructor which also creates the playback window.
		CVideo(char* szFilename,						// filename of the video
         int32_t x, 									// left edge of playback window
         int32_t y, 									// top of the playback window
         int32_t w, 									// width of the playback window
         int32_t h, 									// height of the playback window
				 int16_t sClip); 							// clipping flag

		// Default destructor.
		~CVideo(void);

	// Implementation
	public:
		// Create a window which requests the digital video device.
		int16_t Create(void);

		// Destroy the window and free the digital video device.
		void Destroy(void);
		
		// Open a video file and create the playback window.
		int16_t Open(char* szFileName,						// name of the video file
            int32_t x,									// left edge of the playback window
            int32_t y,									// top of the playback window
            int32_t w,									// width of the playback window
            int32_t h,									// height of the playback window
					  int16_t sClip);							// clipping flag, use following values:
						 											//    0: no clipping, window will be resized to 
																	//			actual video's size.
																	//		1:	video will be clipped to the size supplied.
																	//		2:	video will not be clipped, video will be
																	//			stretched/shrinked to the window's size.
																	// Close a video file.  Will abort the current video file, if playing.
		int16_t Close(void);

		// Play the current video.  Currently, when this function is called, the video will be played
		//	from begin to end.  Possible future functionality may include setting from/to selections,
		// repeat characteristics, etc.  For now, just plain play the darn video.
		// The next day: I realize that we need additional functionality for playing segments within
		// the video.  This could be accomplished with 3 parameters, all of which could be left out
		// for default operation.
		// sMode: 	0 - default operation, lStart/lEnd ignored
		// 			1 - lStart and lEnd specified as time (milliseconds)
		//				2 - lStart and lEnd specified as number of frames.
		int16_t Play(int16_t sMode = 0, int32_t lStart = 0, int32_t lEnd = 0);

		// Pause the current video.  This function must be used if you wish to pause the video.  Stop
		// is not the same as this function.
		int16_t Pause(void);

		// Resume the current video.  The video must have been previously paused.
		int16_t Resume(void);

		// Stop the current video.  This, in effective, will reset the position to the beginning of the
		// video.
		int16_t Stop(void);

		// Get the current frame number of the video.
    int32_t GetFrame(void);

		// Get the current position of the video (in milliseconds).
    int32_t GetPosition(void);

		// The following few functions return status/progress information about the current video.
		// If the status requested is valid, TRUE will be returned - FALSE otherwise.
		int16_t IsOpened(void);		// TRUE if a video is currently opened.
		int16_t IsPlaying(void);		// TRUE if the current video is playing.
		int16_t IsPaused(void);		// TRUE if the current video was paused.
		int16_t IsStopped(void);		// TRUE if the current video was stopped but not paused.

	private:
		void Init(void);
    int32_t TraceMCIError(char* szErrOrigin);


	// Data
	private:
		uint32_t		m_ulDeviceID;
    int32_t		m_lhwnd;
	};

#endif
