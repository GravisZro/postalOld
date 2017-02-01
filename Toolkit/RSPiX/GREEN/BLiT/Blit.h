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
//********************  USER FUNCTION SUMMARY  **********************
//
// BLiT.LIB depends on BLUE.LIB and IMAGE.LIB
//
// You only need to include BLiT.H
//
// See "BLiT.DOC" for detailed function descriptions.
//
/********************************************************************

---------------------  USEFUL BLUE EXTENSIONS ----------------------

BASIC TYPES:  RRect, PRect = {sX,sY,sW,sH}
INTEGER TYPES:  uint64_t,int64_t,U128,S128 integer types
STANDARD TEMPLATE EXTENSIONS to blue MACROS:  SQR / ABS / ABS2 / MIN / MAX / SGN

int16_t	rspSetWindowColors()	- Restore Windows System Colors
void	rspWaitForClick(sButton = 3) - Wait for down andup of specified button
void	rspSetBMPColors(pim,sStart,sNum) - Insert BMP colors into palette

--------------------  USEFUL GRAPHICS FUNCTIONS  -------------------

int16_t rspLasso(anyColor,pimSrc,sX,sY,sW,sH,iFlagL=1,iFlagR=1,iFlagT=1,iFlagB=1)
	- deluxe function for locating images somewhere inside of a buffer
void	rspPlot(anyColor,pimDst,sX,sY) - convenient plotter
int16_t	rspRect(u32Color,pimDst,sX,sY,sW,sH,prClip=nullptr) - high speed solid rectangle
int16_t	rspRect(sThickness,u32Color,pimDst,sX,sY,sW,sH,prClip=nullptr) - high speed hollow rectangle
int16_t rspCrop(pimSrc,sX,sY,sW,sH,[sAlign],[pimDst])
int16_t rspPad(pimSrc,sX,sY,sW,sH,[sAlign],[pimDst])

----------------------  VITAL BLiT FUNCTIONS  -----------------------

int16_t	rspLockScreen() - optimize your screen access
int16_t	rspUnlockScreen()
void	rspNameBuffers(ppimMemBuf,ppimFrontVid=nullptr,ppimBackVid=nullptr)
	- needed to BLiT to "official" buffers and screens

-----------------------  BLiTting OPERATIONS  -----------------------

int16_t	rspBlit(pimSrc,pimDst,sSrcX,sSrcY,sDstX,sDstY,sW,sH,prDst=nullptr,prSrc=nullptr)
- opaque copy for any uncompressed buffer
int16_t	rspBlit(pimSrc,pimDst,sX,sY,prDst=nullptr)
- transparent copy from any compressed buffer
int16_t	rspBlitA(pimSrc,pimDst,sX,sY,sW,sH,const RRect* prDst=nullptr,RRect* prSrc=nullptr)
- an opaque, auto-aligned "punch through" BLiT with double clipping.
rspBlit(pixForeColor,pixBackColor,pimSrc,pimDst,sDstX,sDstY,prDst = nullptr,sAddW = 0)
- monochrome FSPR1 BLiTting...
CStrafe => pImage,sHotX,sHotY,psLinkX,psLinkY = one NODE of a strafe
int16_t rspStrafeRotate(pvReturnArray,pimSrc,sCenterX,sCenterY,dScale,sNumFrames,
							[ dStartDeg=0,dDegInc=0,
							sNumLinks=0,psX=nullptr,psY=nullptr ])
- Create an array of rotated images.

------------------  Image Type Conversion Functions  ----------------

  Image extensions:
  FSPR8 - ConvertFrom, ConvertTo, Delete
  FSPR1 - ConvertFrom, ConvertTo, Delete:
	rspSetConvertToFSPR1(ulLassoBackCol=0,ulComprBackCol=0,sTrimT=0,sTrimB=0)
	rspSetConvertFromFSPR1(ulFrontCol,ulBackCol=0)
  FSPR16,FSPR32 - NYI
  ROTBUF - ConvertFrom, ConvertTo, Delete:
	rspSetConvertToROTBUF(sCenterX,sCenterY);


----------------------------- PRINTING text -------------------------

< This is a BLiT96 patch >

RFont	fntMyFont; // instantiate a font
RPrint pntBuf;	// instantiate a printing buffer

fntMyFont.Load(pszPath,pszName); // Will ADD a ".fnt"!
RPrint pntBuf;	// instantiate a printing buffer
pntBuf.set(&fntMyFont,sCellHeight); // set font and size
pntBuf.SetColor(sLetterColor,sBackColor); // set colors
pntBuf | "Input Text and " | sNumbers | " or anything.\n");
pntBuf.print(sX,sY,pimDst);	// PRINT IT!
----------------
pntBuf.printC(sX,sY,sW,pimDst); // Centers text in box
pntBuf.GetWidth(); // returns width in pixels of entire text buffer
pntBuf.GetCell(sASCII,sW,sH); // Sets (sW,sH) to size of character
----------------
pntBuf.SetColumn(sX,sY,sW,sH,pimDst); // Autowrap is crude right now
pntBuf.Clear(); // Clear the print buffer early

******************************************************************/

// Depends only on image
#ifndef BLIT_H
#define BLIT_H

/////////////////////////////////////////////////////////////////////
// 07/19/96	JMI	Altered to allow use of simple RSPiX root base path
//						plus RSPiX/Blue path.  This should also allow this
//						header to be used on the Mac if it is necessary.
// JEFF: Changed this block of code FROM:
//#include "image/image.h"
//#include <BLUE/Blue.h>
// TO:
#include <BLUE/Blue.h>

#include <GREEN/Image/Image.h>
#include <GREEN/Image/SpecialTyp.h>

//END//CHANGES//BY//JON//////////////////////////////////////////////

// This is the user header for all 2D sprite operations.

// If you already have a int16_t version, then you can use it, too!
class	RRect
	{
public:
	int16_t sX;
	int16_t sY;
	int16_t sW;
	int16_t sH;
	// Returns -1 if clipped completely out,
	// It clips your rect to the input rect
	// If clipped completely, sets rect to 0
	// & sets this rect to {0,0,0,0}
	//
	int16_t ClipTo(RRect *prClipTo);
   RRect(){}
	RRect(int16_t sXi,int16_t sYi,int16_t sWi=0,int16_t sHi=0)
		{ sX = sXi,sY = sYi,sW = sWi,sH = sHi; }
   ~RRect(){}

	}; // used for clipping!

// templated forms of common functions:
// Name your screen references...
extern void rspNameBuffers(RImage** ppimMemBuf,RImage** ppimVidBuf = nullptr,
									RImage** ppimBackBuf = nullptr);

// For your convenience...
extern	int16_t rspSetWindowColors(); // restore system colors

// For your convenience...
extern void	rspWaitForClick(int16_t sButton = 3);

// For your convenience...
extern	void	rspSetBMPColors(RImage* pim,int16_t sStartIndex,int16_t sNum);

extern	int16_t	rspBlit(RImage* pimSrc,RImage* pimDst,int16_t sSrcX,int16_t sSrcY,int16_t sDstX,
			  int16_t sDstY,int16_t sW,int16_t sH,const RRect* prDst=nullptr,const RRect* prSrc=nullptr);

// This does the compressed transparent BLiTting
// Currently only 8-bit mode.
//
extern	int16_t	rspBlit(RImage* pimSrc,RImage* pimDst,int16_t sX,int16_t sY,const RRect* prDst=nullptr);
extern	int16_t	rspBlitT(RImage* pimSrc,RImage* pimDst,int16_t sSrcX,int16_t sSrcY,int16_t sDstX,
			  int16_t sDstY,int16_t sW,int16_t sH,const RRect* prDst,const RRect* prSrc);


// Used only to punch a rectangle between two 128-bit aligned buffers.
// Source and Destination (X,Y) must be the same.
// Will widen your rectangle to take advantage of your bus!
// 
extern	int16_t	rspBlitA(RImage* pimSrc,RImage* pimDst,int16_t sX,int16_t sY,
				int16_t sW,int16_t sH,const RRect* prDst=nullptr,const RRect* prSrc=nullptr);

// This is not currently designed for 24-bit mode.
// Returns 0 if something found...
// The direction flags are non-zero to operate a direction.  They refer to
// the side shrinking.
//
#ifdef WIN32
	template <class PIXSIZE>
	extern	int16_t	rspLasso(PIXSIZE ignoreColor,RImage* pimSrc,int16_t &x,int16_t &y,int16_t &w,int16_t &h,
						int lFlag=1,int rFlag=1,int tFlag=1,int bFlag=1);
#else // MAC
	template <class PIXSIZE>
	int16_t	rspLasso(PIXSIZE ignoreColor,RImage* pimSrc,int16_t &x,int16_t &y,int16_t &w,int16_t &h,
						int lFlag=1,int rFlag=1,int tFlag=1,int bFlag=1);
#endif


/* no restrictions
#ifdef WIN32
	template <class PIXSIZE>
	extern	void rspPlot(PIXSIZE color, RImage* pimDst,int16_t sX,int16_t sY);
#else	// MAC
	template <class PIXSIZE>
	void rspPlot(PIXSIZE color, RImage* pimDst,int16_t sX,int16_t sY);
#endif
*/
#define rspPlot(color,pimDst,sX,sY) do { const RRect rspPlotTempRect(0,0,pimDst->m_sWidth,pimDst->m_sHeight); rspClipPlot(color,pimDst,sX,sY,&rspPlotTempRect); } while (0)

// no restrictions
#ifdef WIN32
	template <class PIXSIZE>
	extern	void rspClipPlot(PIXSIZE color, RImage* pimDst,int16_t sX,int16_t sY,const RRect* prClip);
#else	// MAC
	template <class PIXSIZE>
	void rspClipPlot(PIXSIZE color, RImage* pimDst,int16_t sX,int16_t sY,const RRect* prClip);
#endif

extern	int16_t rspRect(uint32_t color,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,RRect* prClip=nullptr);

extern	int16_t rspRect(int16_t sThickness,uint32_t color,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,RRect* prClip=nullptr);

extern	int16_t	rspLockBuffer();
extern	int16_t	rspLockScreen();
extern	int16_t	rspUnlockBuffer();
extern	int16_t	rspUnlockScreen();
extern	int16_t	rspGeneralLock(RImage* pimDst);
extern	int16_t	rspGeneralUnlock(RImage* pimDst);

/* archaic
extern	void	rspSetConvertToFSPR1(uint32_t ulLassoBackCol = 0,uint32_t ulComprBackCol = 0,
								int16_t sTrimT = 0,int16_t sMaxH = 0,uint8_t ucASCII = 0);

extern	void	rspSetConvertFromFSPR1(uint32_t ulFrontCol,uint32_t ulBackCol = 1);
*/

/* Archaic
extern	int16_t	rspBlit(uint32_t ulForeColor,uint32_t ulBackColor,RImage* pimSrc,RImage* pimDst,
								  int16_t sDstX,int16_t sDstY,const RRect* prDst = nullptr,int16_t sAddW = 0);
extern	int16_t	rspBlit(uint32_t ulForeColor,uint32_t ulBackColor,RImage* pimSrc,RImage* pimDst,
	int16_t sDstX,int16_t sDstY,int16_t sDstW,int16_t sDstH,const RRect* prDst=nullptr,int16_t sAddW=0,
	int16_t *psItalics=nullptr);
	*/

//============================  The modern version FSPR1 =================================
extern	int16_t rspBlit(
				  uint32_t ulForeColor, // will draw in color 0!
				  RImage* pimSrc,
				  RImage* pimDst,
				  int16_t sDstX,
				  int16_t sDstY,
				  const RRect* prDst = nullptr
				  ); // no scaling, clipping, or text effects.

extern	int16_t rspBlit(
				  uint32_t ulForeColor, // will draw color 0
				  RImage* pimSrc,
				  RImage* pimDst,
				  int16_t sDstX,
				  int16_t sDstY,
				  int16_t sDstW,
				  int16_t sDstH,
				  const RRect* prDst = nullptr
				  ); // no clipping or text effects, but scaling.

extern	int16_t rspBlit(
				  uint32_t ulForeColor,
				  RImage* pimSrc,
				  RImage* pimDst,
				  int16_t sDstX,
				  int16_t sDstY,
				  int16_t* psLineOffset // Must be as long as the height!
				  );	// No clipping ever, text effects, no scaling.

extern	int16_t rspBlit(
				  uint32_t ulForeColor, // will draw color 0
				  RImage* pimSrc,
				  RImage* pimDst,
				  int16_t sDstX,
				  int16_t sDstY,
				  int16_t sDstW,
				  int16_t sDstH,
				  int16_t* psLineOffset // Must be as long as the height!
				  ); // No clipping ever, text effects, scaling	

//========================================================================================
	
extern	int16_t	rspCrop(RImage* pimSrc,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
				  int16_t sAlign = 16);
extern	int16_t	rspPad(RImage* pimSrc,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
				  int16_t sAlign = 16);
extern	int16_t rspRemovePadding(RImage* pimSrc);
extern	int16_t rspAddRotationPadding(RImage* pimSrc,int16_t sHotX,int16_t sHotY);


/* archaic
extern	int16_t	_rspBlit(uint32_t ulClrLTR,uint32_t ulClrBKD,RImage* pimSrc,RImage* pimDst,
				  int16_t sDstX,int16_t sDstY,int16_t sW,int16_t sH,
				  int16_t sAddW=0,u16Frac* frSkipY=nullptr,u16Frac* frSkipX=nullptr);
extern	int16_t	_rspBlit(uint32_t ulForeColor,uint32_t ulBackColor,RImage* pimSrc,RImage* pimDst,
								  int16_t sDstX,int16_t sDstY,const RRect* prDst = nullptr,int16_t sAddW = 0);

extern	int16_t	_rspBlit(uint32_t ulForeColor,uint32_t ulBackColor,RImage* pimSrc,RImage* pimDst,
								  int16_t sDstX,int16_t sDstY,int16_t sDstW,int16_t sDstH,
								  const RRect* prDst = nullptr,int16_t sAddW = 0);
*/

extern	int16_t rspRectToMono(uint32_t ulColor,RImage* pimDst,int16_t sX,int16_t sY,
						int16_t sW,int16_t sH);

//****************************************************************************************
//****************************************************************************************
//=============================== Silly Font Test Stuff! =================================
//****************************************************************************************
//****************************************************************************************

struct FontNode {
	int16_t	sCellHeight;
	int16_t	sUnproportionalWidth;
	RImage*	pLetters[256];	// ptr to letters or nullptr
	FontNode* pNext;
};

// put in header...
class	RFontOld
	{
public:
	char*	m_pszFontName;
	int16_t	m_sMaxCellHeight;	// maximum original size

	FontNode* m_pHead;	// last created font...
	FontNode* m_pFontList;
	//================== initialization:
	RFontOld();
	RFontOld(char*	pszName);
	~RFontOld();
	//===================================================
	//===========  Font Installation:
	//===================================================
	void	AddLetter(RImage* pimLetter);
	int16_t	Save(char* pszPath); // only 5 chars standard...
	int16_t	Save(RFile* pFile);
	int16_t	Load(char* pszPath,char* pszName); // only 5 chars standard...
	int16_t	Load(char* pszName); // only 5 chars standard...
	};

// This IS currently 8=bit color hard wired!

// flags for formatting: (FORMATTING NYI)
const	int16_t	TXT_LEFT = 1;
const int16_t	TXT_CENTER = 2;
const	int16_t	TXT_WORDWRAP = 4;
const int16_t	TXT_WORDJUST = 8; // change space size
const	int16_t	TXT_CHARJUST = 16;
const	int16_t	TXT_MULTILINE = 32; // allow multiple lines

// flags for attributes:
// (Tab and Spacing are always considered ON at default values.)
const	int16_t	TXT_BOLD = 1;
const	int16_t	TXT_ITALIC = 2;
const	int16_t	TXT_WIDE = 4;
const int16_t cgsMAX_FONT_SIZE = 256; // may need to adjust!

/*
class	RPrint
	{
public:

	RPrint();
	~RPrint();
	//===========================================
	void	Clear();
	void	SetColumn(int16_t sX,int16_t sY,int16_t sW=0,int16_t sH=0,RImage* pimDst = nullptr);
	void  SetFont(RFontOld* pFnt,int16_t	sHeight);
	void	SetStretch(float fWide); // 1.0 = normal
	void	SetItalic(float fItalic); // 0.0 = normal
	void	SetItalic(int16_t sItalic); // 0 = normal, absolute pixel offset.
	void  SetBold(int16_t sBold); // actual pixel effect, fine control, 0 = normal
	void  SetBold(float fBold); // 1.0 = normal, length in Cell Heights
	void	SetColor(int16_t sLetter,int16_t sBkd = 0);
	void	SetTab(int16_t sPixNum);
	void	SetTab(float fPixNum);
	void	SetSpace(int16_t sPix); // additional
	void	SetSpace(float fSpace); // in Cell heights

	void  set(RFontOld* pFnt,int16_t	sHeight){SetFont(pFnt,sHeight);} // backwards

	int16_t	GetWidth();	// returns width in pixels of current buffer...
	void	GetCell(int16_t sASCII,int16_t &w,int16_t &h); // does NOT include AddW
	//===========================================
	//=========  Let's Print!!!   ===============
	//===========================================
	void	print(int16_t sX,int16_t sY,RImage* pimDst = nullptr);
	void	printC(int16_t sX,int16_t sY,int16_t sW,RImage* pimDst = nullptr);
	RFontOld*	GetFont(){return m_fnCurrent;}

	RPrint& print();

	RPrint&	operator|(char* psz);
	RPrint&	operator<<(char* psz);
	RPrint& operator|(int16_t	in);
	RPrint& operator|(long	in);
	RPrint& operator|(float	in);
	RPrint& operator|(double	in);
	RPrint& operator|(char	in);
	RPrint&	debug();
	int16_t	m_sCellH;	// Current font cell height
	int16_t	m_sCurX;
	int16_t	m_sCurY;

public:

	int16_t LineFeed();
	int16_t	GetChar();
	void	GetCell(RImage* pimLetter,int16_t* psCellH = nullptr,
		int16_t* psCellW = nullptr,int16_t* psStretchW = nullptr); // does NOT include AddW!

  // ***************************************
  // ******  The curent state of the font!
  // ***************************************
private:
	int16_t m_sTabW;
	float m_fTabW;

	float	m_fBold;	// state of bold, relative to cell Height
	int16_t	m_sBold;	// bold pixels added per run

	float	m_fItalic;	// state of italic, relative to cell height
	int16_t	m_sItalic;	// Number pixels slant

	float	m_fWide; // 1.0 = true aspect ratio
					 // No absolute width because it varies by character
	float m_fSpace; // Extra space between pixels based on chosen height.
	int16_t m_sSpace; // Extra kerning space in pixels

	uint8_t	m_clrBKD;	// current color
	uint8_t	m_clrFGD;

	RFontOld*	m_fnCurrent;	// Current font
	FontNode* m_pFontSize;	// Base font template

	u16Frac*	m_pCurFracX;	// to aid in the scaling process!
	u16Frac*	m_pCurFracY;	// to aid in the scaling process!

	char	m_buffer[4096];
	char	m_temp[256];
	char*	m_pBuf; // open stack
	char* m_pCurBuf;

	RImage*	m_pimTarget; // Destination
	int16_t	m_sL,m_sT,m_sW,m_sH;	// Column Rectangle...
	// if (sW < 0) then rect is disabled!
	int16_t	m_sFORMAT;	// formatting flags
	int16_t	m_sATTRIB;	// letter attributes
	int16_t m_psItalic[cgsMAX_FONT_SIZE]; // store by line...

public: // for development ONLY
	void	PrintLine(int16_t sNumChar,int16_t sJustDelta = 0,int16_t sJustFraction = 0,
			int16_t sDen = 1);

	};
*/

class CStrafe{
public:

	CStrafe()
		{
		sHotX = sHotY = 0;
		psLinkX = psLinkY = nullptr;
		pImage = nullptr;
		}

	~CStrafe()
		{
		if (pImage) delete pImage;
		if (psLinkX) free(psLinkX);
		if (psLinkY) free(psLinkY);
		}
	
	RImage* pImage;
	int16_t	sHotX; // subtract from coord
	int16_t sHotY;
	int16_t *psLinkX; // a var length array
	int16_t *psLinkY; // a var length array
};

typedef CStrafe* PStrafe;

// ARCHAIC Backwards support for BLT_STRAFE:

// User will use a Strafe to hold the data!
//
struct StrafeFrame {
	RImage*	pfspr; // must be altered!
	int16_t sHotX;
	int16_t sHotY;
	int16_t sCurDeg;
	int16_t *psLinkX;
	int16_t *psLinkY;

}; // archaic

struct Strafe {
	int16_t sNumFrames;
	int16_t sNumLinks;
	CStrafe *pFrame;
}; // archaic!

/*
extern	Strafe*	BLT_RotStrafe(RImage* pimSrc,int16_t sHotX,int16_t sHotY,int16_t sNumInc,
						  int16_t sDstH,int16_t sNumLinks,int16_t *psX, int16_t* psY);
*/

//extern	void	BLT_FreeStrafe(Strafe* pKill);

extern	void	rspSetConvertToROTBUF(int16_t sCenterX,int16_t sCenterY);

extern	void	_RotateShrink(float fDeg,RImage* pimSrc,RImage* pimDst,
						  int16_t sDstX,int16_t sDstY,int16_t sDstW,int16_t sDstH,
						  int16_t sFlipCode = 0);
extern	int16_t rspStrafeRotate(void *pReturnArray,	// Output
							RImage* pimSrc,int16_t sCenterX,int16_t sCenterY,double dScale, // Input
							 int16_t sNumFrames,double dStartDeg = 0.0,double dDegInc = 0.0,
							 int16_t sNumLinks=0,int16_t *psX=nullptr,int16_t *psY=nullptr, // input
							 // generic user stucture must be an array:
							 RImage* pIm = nullptr, int16_t *psHotX = nullptr, int16_t *psHotY = nullptr,
							 int16_t **ppsX = nullptr,int16_t **ppsY = nullptr,
							 int32_t lStructSize = sizeof(CStrafe));

extern	int16_t rspBlitToMono(
				  RImage* pimSrc,
				  RImage* pimDst,
				  int16_t sDstX,
				  int16_t sDstY,
				  int16_t sDstW,
				  int16_t sDstH
				  );

//*****************************************************************************
// This is the main controller... It clips in pixels, then thinks in bytes:
// if prSrc == nullptr, no source clipping will occure
// if prDst == nullptr, it will clip to the RImage
//
extern	int16_t	rspBlitT(uint32_t ucTransparent,RImage* pimSrc,RImage* pimDst,int16_t sSrcX,int16_t sSrcY,int16_t sDstX,
			  int16_t sDstY,int16_t sW,int16_t sH,const RRect* prDst,const RRect* prSrc);

extern	int16_t rspBlitT(RImage* pimSrc,RImage* pimDst,int16_t sDstX,int16_t sDstY,
					int16_t sDstW,int16_t sDstH,const RRect* prDstClip=nullptr);

extern	int16_t rspBlitT(RImage* pimSrc,RImage* pimDst,RRect* prSrc,const RRect* prDst,
				  const RRect* prDstClip=nullptr,const RRect* prSrcClip=nullptr);

extern	int16_t rspBlitT(RImage* pimSrc,RImage* pimDst,int16_t sDstX,int16_t sDstY,
							 double dRatW = 1.0,double dRatH = 1.0,const RRect* prDstClip = nullptr);

extern	int16_t rspBlitRot(int16_t sDeg,RImage* pimSrc,RImage* pimDst,
					 int16_t sDstX,int16_t sDstY,int16_t sDstW,int16_t sDstH,
					 const RRect* prDstClip = nullptr);

extern	int16_t rspBlitRot(int16_t sDeg,RImage* pimSrc,RImage* pimDst,
					 int16_t sDstX,int16_t sDstY,double dScaleX,double dScaleY,
					 const RRect* prDstClip = nullptr);

extern	void rspLine(uint8_t ucColor,RImage* pimDst,int16_t sX1,int16_t sY1,int16_t sX2,int16_t sY2,
							 const RRect* prClip = nullptr);

extern	int16_t rspRand(int16_t sMax);

extern	int16_t rspMonoRect(int16_t rspBrightness,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
						const RRect* prClip = nullptr);

extern	int16_t rspMonoRectLightenTo(int16_t rspBrightness,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
						const RRect* prClip = nullptr);

extern	int16_t rspMonoRectDarkenTo(int16_t rspBrightness,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
						const RRect* prClip = nullptr);

extern	int16_t rspClipBrightness(int16_t sNum,int16_t sLev);

extern	int16_t rspMonoRect(int16_t sThickness,int16_t rspBrightness,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
						const RRect* prClip = nullptr);

extern	int16_t rspMonoRectLightenTo(int16_t sThickness,int16_t rspBrightness,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
						const RRect* prClip = nullptr);

extern	int16_t	rspMonoRectDarkenTo(int16_t sThickness,int16_t rspBrightness,RImage* pimDst,int16_t sX,int16_t sY,int16_t sW,int16_t sH,
						const RRect* prClip = nullptr);

extern	int16_t rspGetBrightness(int16_t sIndex,uint8_t* pR,uint8_t* pG,uint8_t* pB,int32_t lInc = 4);

//**********************************  FAST CONVERTS:  *******************************
//----- BYTE dpeth to shift amount:
static const int16_t gsByteToShift[] = {0,0,1,0,2,0,0,0,3,0,0,0,0,0,0,0,4};
//                         BYTES:     0 1 2 3 4 5 6 7 8 9 a b c d e f *
static const int16_t gsBitsToShift[] = 
	   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2};
//BIT: 0 1 2 3 4 5 6 7 8 9 a b c d e f 0 1 2 3 4 5 6 7 8 9 a b c d e f *

//***********************************************************************************

//====================================
#include <GREEN/BLiT/RFont.h>
#include <GREEN/BLiT/RPrint.h>
#include <ORANGE/QuickMath/QuickMath.h>

//====================================
#endif
