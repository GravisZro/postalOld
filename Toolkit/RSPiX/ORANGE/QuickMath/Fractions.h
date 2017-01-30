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
#ifndef FRACTIONS_H
#define FRACTIONS_H
//=======================
#include <BLUE/System.h>
//====================
/*****************************************************************
This header depends on the rest QuickMath, but is not necessary
for use of Quickmath, and deals primarily with high speed fraction
operations.
/*****************************************************************
Hungarian:	fr = either generic fraction or signed 16 (int16_t + int16_t [ / uint16_t] )
				pfr = pointer to fraction
				frint32_t = (int32_t + int32_t [ / uint32_t] )
				type = RFracint32_t, members = "frac" and "mod" and "set"

				frint16_t = int16_t + int16_t [ / uint16_t]
				fruint16_t = int16_t + uint16_t [ / uint16_t]
				type = RFracuint16_t, same members, etc., etc.

				& = implicit pass by reference, do NOT use a pointer
				& is used in almost all cases to prevent VC from creating
				a local stack frame which would slow things down by 20 times.

NOTE: The integral value of the fraction is always a SIGNED quantity.  
The S/U refers to whether or not the NUMERATOR of the fraction is
signed or unsigned.

NOTE: Some functions can take a long AS an frint16_t.  Understand that this is
a memory cast as a frint16_t and does NOT represent the actual int32_t "value"!
Use Get and Set "Value" functions to translate integers to fixed point!
/*****************************************************************
RFracint32_t { mod, frac, set }
RFracint16_t { mod, frac, set }
RFracuint16_t { mod, frac, set }

inline void rspfrDiv(&frDst,sNum,sDen) // sDen > 0
inline void rspfrSet(&frDst,lVal) // a memset!
inline void rspfrAdd(&frDst,frAdd,int16_t sDen)
inline void rspfrSub(&frDst,frSub,int16_t sDen)
inline void rspfrAddHalf(&frDst,int16_t sDen) // useful for rounding
inline void rspfrSetValue(&frDst,double dVal,int16_t sDen)
inline void rspMakeProper(&fruint16_tDst,usNum,usDen)
inline RFracuint16_t* rspfruint16_tStrafe256(usNum,usDen) // gives 0..255 * fraction!

//*********************** HIGH INTENSITY SPEED! ***********************
inline void rspfrAdd32(&lVal,&lNum,&lDel,&lInc,&lDen,&lAdd,PROPER,POSITIVE)
inline void rspfrAdd32(&lVal,&lNum,&lDel,&lInc,&lDen,&lAdd,IMPROPER,POSITIVE)
inline void rspfrAdd32(&lVal,&lNum,&lDel,&lInc,&lDen,&lAdd,IMPROPER,POSITIVE)
inline void rspfrAdd32(&lVal,&lNum,&lDel,&lInc,&lDen,&lAdd,PROPER,NEGATIVE)
inline void rspfrAdd32(&lVal,&lNum,&lDel,&lInc,&lDen,&lAdd,IMPROPER,NEGATIVE)
inline void rspfrAdd32(&lVal,&lNum,&lDel,&lInc,&lDen,&lPixSize)
/****************************************************************/

  typedef union
	{
	struct {
	uint16_t mod;
	uint16_t frac;
			};
	uint32_t set;
	}	RFracuint16_t;	// No denominator, unsigned values...

//======================================= 
typedef union	{
   int32_t	set;
	struct	
		{
		int16_t	mod;
		int16_t frac;
		};
	} RFracint16_t;

//=======================================
typedef union	{
	int64_t	set;
	struct	
		{
      int32_t mod;
      int32_t frac;
		};
	} RFracint32_t; // good for compound fractions

//************* ALERT ALERT!!! FRACTION STUFF HERE!!!!!  *******************
// There should be more functions to handle simplified cases!
// sDen MUST be positive!
inline void rspfrDiv(RFracint16_t& fr,int16_t sNum,int16_t sDen)
	{
	fr.mod = sNum / sDen;
	fr.frac = sNum % sDen;
	if (sNum < 0) // special case
		if (fr.frac != 0) // put on correct side of negative number!
			{
			fr.mod--;
			fr.frac += sDen;
			}
	}

inline void rspfrSet(RFracint16_t& fr,int32_t lVal) { fr.set = lVal; }

// Signed can go either way... use unsigned for speed!
// This also assumes it can add greater than one!
inline void rspfrAdd(RFracint16_t& frDst,RFracint16_t& frAdd,int16_t sDen)
	{ 
	frDst.mod += frAdd.mod;
	frDst.frac += frAdd.frac;
	if (frDst.frac >= sDen) {frDst.mod++;frDst.frac -= sDen;}
	if (frDst.frac < 0) {frDst.mod--;frDst.frac += sDen;}
	}

inline void rspfrSub(RFracint16_t& frDst,RFracint16_t& frSub,int16_t sDen)
	{ 
	frDst.mod -= frSub.mod;
	frDst.frac -= frSub.frac;
	if (frDst.frac < 0) {frDst.mod--;frDst.frac += sDen;}
	if (frDst.frac >= sDen) {frDst.mod++;frDst.frac -= sDen;}
	}

inline void rspfrAddHalf(RFracint16_t& frDst,int16_t sDen) // useful for rounding
	{ 
	frDst.frac += (sDen >> 1);
	if (frDst.frac >= sDen) {frDst.mod++;frDst.frac -= sDen;}
	}

//========================= debugging only! (slow)
inline void rspfrSetValue(RFracint16_t& frDst,double dVal,int16_t sDen)
	{
	frDst.mod = (int16_t) floor(dVal);
	frDst.frac = (int16_t) ((dVal - floor(dVal))*sDen);
	}

// add two fractions of identical denominators...
// both fraction MUST be PROPER! 
// UNSIGNED!!!!
//
inline void	rspfrAdd(RFracuint16_t& pDst,RFracuint16_t& pAdd,int16_t sDen)
	{
	pDst.mod += pAdd.mod;
	if ( (pDst.frac += pAdd.frac) >= sDen)
		{
		pDst.mod++; // ONLY adding proper fractions
		pDst.frac -= sDen;
		}
	}

// Creates a proper fraction from an improper one:
// The sizes MUST be appropriate!
//
inline void rspMakeProper(RFracuint16_t& pDst,uint16_t usNum,uint16_t usDen)
	{
	pDst.mod = usNum / usDen;
	pDst.frac = usNum % usDen;
	//pDst.frac = usNum - pDst.mod * usDen;// overflow problem
	}

// Creates an array of 256 fractions, uch that 0 is 0,
// 1 is tha base fraction, and 255 is 255 * the base fraction.
// Both must be unsigned!  (uses calloc)
//
inline RFracuint16_t* rspfruint16_tStrafe256(uint16_t usNum,uint16_t usDen)
	{
	RFracuint16_t* pu16fNew = (RFracuint16_t*) calloc(256,sizeof(RFracuint16_t));
	RFracuint16_t u16fInc;
	rspMakeProper(u16fInc,usNum,usDen); // the 2 part mod

	uint32_t ulNumInc = 0;
	for (int16_t i = 1; i < 256 ; i++)
		{
		pu16fNew[i].mod = pu16fNew[i-1].mod + u16fInc.mod;
		pu16fNew[i].frac = pu16fNew[i-1].frac + u16fInc.frac;
		if (pu16fNew[i].frac >= usDen)
			{
			pu16fNew[i].frac -= usDen;
			pu16fNew[i].mod++; // unsigned positive increment only!
			}
		}

	return pu16fNew;
	}

//-------------------------- 32 bit signed integer calculus stuff:
//*********************** HIGH INTENSITY SPEED! ***********************

typedef uint32_t POSITIVE;
typedef int32_t NEGATIVE;

typedef uint32_t PROPER;
typedef int32_t IMPROPER;

// lDel is unused here...
// all values are signed, including lInc & lDel ...
//
inline void rspfrAdd32(int32_t &lVal,int32_t &lNum,int32_t &lDel,int32_t &lInc,
                    int32_t &lDen,int32_t &lAdd,PROPER,POSITIVE)
	{
	lNum += lInc;
	if (lNum >= lDen) { lNum -= lDen; lVal += lAdd; }
	}

// all values are signed, including lInc & lDel ...
//
inline void rspfrAdd32(int32_t &lVal,int32_t &lNum,int32_t &lDel,int32_t &lInc,
                    int32_t &lDen,int32_t &lAdd,IMPROPER,POSITIVE)
	{
	lVal += lDel;
	lNum += lInc;
	if (lNum >= lDen) { lNum -= lDen; lVal += lAdd; }
	}

// lDel is unused here...
// all values are signed, including lInc & lDel & lAdd ...
//
inline void rspfrAdd32(int32_t &lVal,int32_t &lNum,int32_t &lDel,int32_t &lInc,
                    int32_t &lDen,int32_t &lAdd,PROPER,NEGATIVE)
	{
	lNum += lInc;
	if (lNum < 0) { lNum += lDen; lVal -= lAdd; }
	}

// all values are signed, including lInc & lDel & lAdd ...
//
inline void rspfrAdd32(int32_t &lVal,int32_t &lNum,int32_t &lDel,int32_t &lInc,
                    int32_t &lDen,int32_t &lAdd,IMPROPER,NEGATIVE)
	{
	lVal += lDel;
	lNum += lInc;
	if (lNum < 0) { lNum += lDen; lVal -= lAdd; }
	}

// all values are signed, including lInc & lDel & lAdd ...
// General (slowest) form... Improper and UNKNOWN SIGN
//
inline void rspfrAdd32(int32_t &lVal,int32_t &lNum,int32_t &lDel,int32_t &lInc,
                    int32_t &lDen,int32_t &lPixSize)
	{
	lVal += lDel;
	lNum += lInc;
	if (lNum < 0) { lNum += lDen; lVal -= lPixSize; }
	if (lNum >= lDen) { lNum -= lDen; lVal += lPixSize; }
	}

//=======================
#endif
