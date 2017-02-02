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
#ifndef SNDFX_H
#define SNDFX_H

#include <cstdio> // For FILE*

#include <BLUE/System.h>

//////////////////////////////////////////////////////////////////////////////
// Macros.
//////////////////////////////////////////////////////////////////////////////
#define RSP_SNDFX_MAX_BITSPERSAMPLE		32			// Maximum bits per sample.
#define RSP_SNDFX_NUM_FADE_INTERVALS	10			// Size of fades table.

//////////////////////////////////////////////////////////////////////////////
// Types.
//////////////////////////////////////////////////////////////////////////////

class RSndFx
	{
	/////////////////////// Typedefs & Enums ///////////////////////////////////
	public:
		// Effects that can be active simulatneously.
		// GetCurrentFX() returns these combined by | for all active fx:
		enum FX {
			NoFX				= 0x0000,	// No current FX.
			// FX that affect all channels.
			FadeIn			= 0x0001,	// Fade in all channels.
			FadeOut			= 0x0002,	// Fade out all channels.
			// FX that affect individual channels.
			DecimateLeft	= 0x0004,	// Decimate left channel.	NYI.
			DecimateRight	= 0x0008		// Decimate right channel.	NYI.
		};

	public:
		// Default constructor.
		RSndFx();
		// Destructor.
		~RSndFx();

	////////////////////////// Querries ///////////////////////////////////////
	public:
		// Returns the currently active fx as FX enums combined with |.
		FX GetCurrentFX(void)	{ return m_fx; }
		
	/////////////////////// Global Methods ////////////////////////////////////
	public:
		// Sets the fade accuracy (i.e., the number of steps to perform a fade).
		// This costs sNumStemps * 256 * bits per sample / 8 bytes of memory.
		// This function should only be called after the bits per sample have
		// been set via a call to SetDataType().
		static int16_t SetFadeAccuracy(	// Returns 0 on success.
			int16_t sNumSteps);				// Number of steps to fades; see above.

		// Release any dynamic memory referenced by static members.
		static void CleanUp(void);

	////////////////////////// Methods ////////////////////////////////////////
	public:
		// Release any dynamic memory and reset variables.
		// Clears all effects.
		void Clear(void);

		// Reset effects to start.
		void Reset(void);

		// Set type of PCM data in use.  This will reset all effects.
		static void SetDataType(	// Returns nothing.
			int32_t lSamplesPerSec,		// Samples per second.
			int32_t lBitsPerSample,		// Bits per sample.
			int32_t lNumChannels);		// Number of channels.
		
		// Implements the effect on the provided buffer.
		void Do(								// Returns nothing.
			uint8_t* pucSrcData,			// Data to affect.
			int32_t lBufSize,					// Amount of data.
			uint8_t* pucDstData = nullptr);	// Destination for data, defaults to
												// same as source.
		
		/////////////////////////////////////////////////////////////////////////
		// Various FX.
		/////////////////////////////////////////////////////////////////////////

		/////////////////////// Fade In /////////////////////////////////////////

		// Set up a fade in.
		int16_t SetUpFadeIn(	// Returns 0 on success.
			int32_t lDuration);	// Duration until silence in milliseconds.

		// Activate/Deactivate fade in.
		void ActivateFadeIn(	// Returns nothing.
			int16_t	sActivate);	// TRUE to activate, FALSE to deactivate.

		/////////////////////// Fade Out ////////////////////////////////////////

		// Set up a fade out.
		int16_t SetUpFadeOut(	// Returns 0 on success.
			int32_t lDuration);	// Duration until full volume in milliseconds.
		
		// Activate/Deactivate fade out.
		void ActivateFadeOut(	// Returns nothing.
			int16_t	sActivate);		// TRUE to activate, FALSE to deactivate.

	////////////////////////// Internal Methods ///////////////////////////////
	protected:
		// Initialize instantiable members.
		void Init(void);

	////////////////////////// Member vars ////////////////////////////////////
	public:
				
	protected:
		FX		m_fx;				// Currently active effects.

		/////////////////////////////////////////////////////////////////////////
		// Various FX.
		/////////////////////////////////////////////////////////////////////////

		/////////////////////// Fade In /////////////////////////////////////////
		
		long	m_lFadeInMillisecondsDuration;	// Original duration.
		long	m_lFadeInBytesDurationAffected;	// Amount left to fade.
		long	m_lFadeInBytesDuration;				// Duration in bytes.

		long	m_lFadeInRate;

		/////////////////////// Fade Out ////////////////////////////////////////

		long	m_lFadeOutMillisecondsDuration;		// Original duration.
		long	m_lFadeOutBytesDurationRemaining;	// Amount left to fade.
		long	m_lFadeOutBytesDuration;				// Duration in bytes.

		long	m_lFadeOutRate;

		///////////////////// Protected Typedefs ///////////////////////////////
		struct PCMINFO {		// Stores info particular PCM type.
			int32_t	lMin;		// Mininum value (silence).
			int32_t	lMax;		// Maximum value (saturation).
		};

		/////////////////////// Static members /////////////////////////////////

		static long	ms_lSamplesPerSec;	// Samples per second.
		static long	ms_lBitsPerSample;	// Bits per sample.
		static long	ms_lNumChannels;		// Number of channels.

		static long	ms_lBitsPerSec;	// Number of bits per second.
												// Can be used to convert bytes to milliseconds
												// and convert milliseconds into bytes.  See
												// macros BYTES2MS and MS2BYTES in SndFx.CPP.

		static PCMINFO	ms_apcminfo[RSP_SNDFX_MAX_BITSPERSAMPLE + 1];	// Stores info 
																			// particular to each 
																			// PCM type.

		static uint8_t*		ms_pu8Fade;			// Unsigned 8 bit output
													// fade table.
		static int16_t*		ms_ps16Fade;		// Signed 16 bit output
													// fade table.

		static int16_t	ms_sNumFadeSteps;		// Number of fade steps.
	};

#endif // SNDFX_H
//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
