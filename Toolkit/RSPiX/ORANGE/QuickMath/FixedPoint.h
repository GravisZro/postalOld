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
#ifndef FIXED_POINT_H
#define FIXED_POINT_H
//====================
#include <BLUE/System.h> // for environment variables...
#include "QuickMath.h"
//====================
/*****************************************************************
This header depends on the rest QuickMath, but is not necessary
for use of Quickmath, and deals primarily with high speed fixed
point operations.
// *****************************************************************
Hungarian:	fp = either generic fixed point or signed 32 (int16_t.uint16_t)
				pfp = pointer to fixed point number
						(sorry FILE*)
				fpint32_t = int16_t.uint16_t
				type = RFixedint32_t, members = "frac" and "mod"

				fpint16_t = int8_t.uint8_t
				fpuint16_t = uint8_t.uint8_t, etc.
				type = RFixedint16_t, same members, etc., etc.

				& = implicit pass by reference, do NOT use a pointer
				& is used in almost all cases to prevent VC from creating
				a local stack frame which would slow things down by 20 times.

NOTE: Some functions can take a long AS an fpint32_t.  Understand that this is
a memory cast as a fpint32_t and does NOT represent the actual int32_t "value"!
Use Get and Set "Value" functions to translate integers to fixed point!

// *****************************************************************
RFixedint32_t { mod, frac, copy }
RFixeduint16_t { mod, frac, copy }

inline RFixedint32_t rspfpSin(sDeg)
inline RFixedint32_t rspfpCos(sDeg)
inline RFixedint32_t rspfpOneOver(sDen)

inline void rspfpSet(&fpX,lVal) // lVal is an fpint32_t in int32_t form (mem)
inline void rspfpAdd(f&pDst,&fpAdd)
inline void rspfpSub(&fpDst,&fpSub)
inline void rspfpMul(&fpDst,&fpMul)
inline void rspfpMul(&fpDst,&fpA,&fpB)
inline void rspfpMul(&fpDst,int32_t lA,int32_t lB) // fpint32_t s as int32_ts (mem)
inline void rspfpAddHalf(&fpDst) // useful for rounding functions
inline void rspfpSetValue(&fpDst,double dVal) // translates VALUE into fp
inline double rspfpGetValue(&fpDst)

// ****************************************************************/
// Fixed Point 32S
// ======================================= signed 15:16 fixed point
typedef union	{
  int32_t	val; // ********* Full 32 bit signed value
	struct	
		{
#ifdef __BIG_ENDIAN__ // big endian
//-----------------------------------------------
		union
			{
			int16_t	mod; //********* signed 16-bit integer part
			struct {
            int8_t upper; 	// for 256v level z-coloring:
            uint8_t lower;
				};
			};
		uint16_t frac; // unsigned 16-bit fractional part
//-----------------------------------------------
#else // little endian
		uint16_t frac;
		union	// for 256v level z-coloring:
			{
			int16_t	mod;
			struct {
            uint8_t lower;
            int8_t upper;
				};
			};
#endif
		};
	} RFixedint32_t;
//====================
inline void rspfpSet(RFixedint32_t& s32fx,int32_t lVal) 
	{ s32fx.val = lVal; }
inline void rspfpAdd(RFixedint32_t& s32fx,RFixedint32_t& s32fxAdd)
	{ s32fx.val += s32fxAdd.val; }
inline void rspfpSub(RFixedint32_t& s32fx,RFixedint32_t& s32fxSub)
	{ s32fx.val -= s32fxSub.val; }

// NEED 64-bit multiplication to handle overflow!
#if 0
#ifdef _M_IX86 // INTEL!
	inline void rspfpMul(RFixedint32_t& s32fx,RFixedint32_t& s32fxMul)
		{ 
		// Use the 64-bit resultant to get the true value!
		__asm	
			{
			mov eax,s32fx.val
			mov ebx,s32fxMul.val
			imul eax,ebx // result is in EDX::EAX
			mov s32fx.val,edx // only the upper word is valid!
			}
		}

	inline void rspfpMul(RFixedint32_t& s32fxC,RFixedint32_t& s32fxA,RFixedint32_t& s32fxB)
		{ 
		// Use the 64-bit resultant to get the true value!
		__asm	
			{
			mov eax,s32fxA.val
			mov ebx,s32fxB.val
			imul ebx // ([*eax]) result is in EDX::EAX
			mov s32fxC.val,edx // only the upper word is valid!
			}
		}

	inline void rspfpMul(RFixedint32_t &lC,int32_t lA,int32_t lB)
		{ 
		union {
			uint32_t val;
			struct { uint16_t lo; uint16_t hi; };
			} temp;
      int32_t temp2;

		// Use the 64-bit resultant to get the true value!
		__asm	
			{
			mov eax,lA
			mov ebx,lB
			imul ebx // ([*eax]) result is in EDX::EAX
			// PROBLEM: we want bits 16-47!!!!!!
			mov temp2,edx // only need hwlf the integral part...
			mov temp,eax	// get upper half!
			}
		lC.mod = (int16_t)temp2; // keep sign, lose upper half!
		lC.frac = temp.hi;
		}
#endif
#endif

inline void rspfpAddHalf(RFixedint32_t& s32fx) // useful for rounding
	{ s32fx.val += (int32_t)32768; }
//========================= debugging only! (slow)
inline void rspfpSetValue(RFixedint32_t& s32fx,double dVal)
	{ s32fx.val = (int32_t) (dVal * 65536.0); }
inline double rspfpGetValue(RFixedint32_t& s32fx)
	{ return (double)s32fx.mod + (double)s32fx.frac / (double)65536.0; }

// For passing back an error:
const RFixedint32_t	s32fxERROR = {-65536};

// For fixed32 type stuff:
//extern uint32_t OneOver[256];
//extern void SetOneOver();
#define NUM_ONEOVER_FP32 1280

class RInitNum
	{
public:
	RInitNum();
   static int32_t OneOver[NUM_ONEOVER_FP32];
	};

//======================================= unsigned 8:8 fixed point
typedef union	{
	uint16_t	val;
	struct	
		{
#ifdef __BIG_ENDIAN__ // big endian
      uint8_t	mod;
      uint8_t frac;
#else // little endian
      uint8_t frac;
      uint8_t	mod;
#endif
		};
	} RFixeduint16_t;
//-------------------------------------

//======================================= signed 8:8 fixed point
typedef union	{
   int16_t	val;
	struct	
		{
#ifdef __BIG_ENDIAN__ // big endian
      int8_t	mod;
      uint8_t frac;
#else // little endian
      uint8_t frac;
      int8_t	mod;
#endif
		};
	} RFixedint16_t;
//-------------------------------------

//====================
extern RFixedint32_t fpSINQ[csNumRotSteps],fpCOSQ[csNumRotSteps];
extern	void InitTrigFP();
inline RFixedint32_t rspfpSin(int16_t sDeg) { return fpSINQ[sDeg]; }
inline RFixedint32_t rspfpCos(int16_t sDeg) { return fpCOSQ[sDeg]; }
inline int32_t rspfpOneOver(int16_t sDen) { return RInitNum::OneOver[sDen]; }

// Auto Initialize hook!
class RQuickTrigFP
	{
public:
	RQuickTrigFP() { InitTrigFP(); TRACE("QTRIG fixed point initialized!\n"); }
	~RQuickTrigFP() {  }
	};

//====================
#endif
