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
// May or may not join blit.h
#ifndef RPRINT_H
#define RPRINT_H
/****************************************************** QUICK CHECK
	//-------------------------- Higher Level
	char const * print(char const * pszFormat,...);
	char const * print(int16_t sX,int16_t sY,char const * pszFormat,...);
	char const * print(RImage* pimDst,int16_t sX,int16_t sY,char const * pszFormat,...);
	int16_t SetEffectAbs(Effect eEffect,int16_t sVal); // absolute:
	int16_t SetEffect(Effect eEffect,double dVal); // relative:
	int16_t SetColor(uint32_t ulForeColor,uint32_t ulBackColor=0,uint32_t ulShadowColor=0);
	int16_t SetDestination(RImage* pimDst,RRect* prColumn = nullptr);
	void SetJustifyRight();
	void SetJustifyCenter();
	void SetJustifyLeft();
	void SetWordWrap(int16_t sOn = TRUE);
	void ResetEffects(); // includes tab and to relative
	void ResetMode(); // all zero
	void SetMode(Mode eMode,int16_t sVal); // 0 = off
	int16_t SetFont(int16_t sCellH,RFont* pFont = nullptr);
	void	GetPos(int16_t *psX,int16_t *psY,int16_t *psW,int16_t *psH);
	int16_t	GetWidth(char const * pszString); // see m_sNumChar,ms_szLineText,ms_sCharPosX
	RFont* GetFont() { return m_pfnCurFont; }
	int16_t SetColumn(int16_t sX,int16_t sY,int16_t sW,int16_t sH);
// ****************************************************** QUICK CHECK
*/

#include "Blit.h"
//====================
class RPrint
	{
public:
	//-------------------------- Public members

	uint32_t	m_ForegroundColor;
	uint32_t	m_BackgroundColor;	// 0 = OFF
	uint32_t	m_ShadowColor;	// Used if offsets are not zero!
	RImage*	m_pimDst;
	RRect		m_rClip;	// used for wrapping..

	// Printing Modes: Default is left justify, proportional
	// Note: JUSTIFY_F & JUSTIFY_R are not independent
	enum Mode {
		UNPROPORTIONAL = 1, // 0 = Proportional
		WORD_WRAP = 2,	// 0 = RAW
		FIELD = 4,	// 0 = line mode => SYTEM FLAG!
		JUSTIFY_ON = 8,	// 0 = Left Justified
		JUSTIFY_FULL = 16, // 0 = Check JUSTIFY_RIGHT
		JUSTIFY_RIGHT = 32, // 0 = Center justified
		TAB_STOP = 64, // 0 = disble
		DECIMAL_STOP = 128, // 0 = disble
	};

	// NEW generic use of varying effects:
	// A floating point value array (default),
	// a int16_t array (optional), and a int16_t value array
	// for each letter effect.  A byte list will flag
	// use of the absolute (int16_t) values:
	enum Effect {
		STRETCH = 0,ITALIC,BOLD,SHADOW_X,SHADOW_Y,TAB,UP_W,UP_H,ADD_W,ADD_H,
		NUM_OF_EFFECTS
	}; // binary
	//-------------------------- Higher Level
	char const * print(char const * pszFormat,...);
	char const * print(int16_t sX,int16_t sY,char const * pszFormat,...);
	char const * print(RImage* pimDst,int16_t sX,int16_t sY,char const * pszFormat,...);
	int16_t SetEffectAbs(Effect eEffect,int16_t sVal); // absolute:
	int16_t SetEffect(Effect eEffect,double dVal); // relative:
	int16_t SetColor(uint32_t ulForeColor,uint32_t ulBackColor=0,uint32_t ulShadowColor=0);
	int16_t SetDestination(RImage* pimDst,RRect* prColumn = nullptr);
	int16_t SetColumn(int16_t sX,int16_t sY,int16_t sW,int16_t sH);
	void SetJustifyRight();
	void SetJustifyCenter();
	void SetJustifyLeft();
	void SetJustifyFull();
	void SetWordWrap(int16_t sOn = TRUE);
	void ResetEffects(); // includes tab and to relative
	void ResetMode(); // all zero
	void SetMode(Mode eMode,int16_t sVal); // 0 = off
	int16_t SetFont(int16_t sCellH,RFont* pFont = nullptr);
	void	GetPos(int16_t *psX,int16_t *psY = nullptr,int16_t *psW = nullptr,int16_t *psH = nullptr);
	int16_t	GetWidth(char const * pszString);
	RFont* GetFont() { return m_pfnCurFont; }
	//-------------------------- Lower Level
	//void OffsetShadow();
	char const * printInt(char const * pszInput);
	void GetPropEffX(int16_t sX,int16_t sE,int16_t sNext,
								  int16_t *psEffX = nullptr,int16_t *psEffE = nullptr,
								  int16_t *psEffNext = nullptr);
	int16_t GetPropCellX(int16_t sChar,int16_t *psX = nullptr,int16_t *psE = nullptr,
			int16_t *psNext = nullptr,int16_t sFirst = FALSE);
	char const * ScanLine(char const * pszInput);
	void  FormatText();
	void	DrawText();
	int16_t GetBlitW(uint8_t c);
	int16_t FrameIt(); // 1= fresh line, -1 = off bottom
	void	printLine();
	void ClearTabs(); // both types
	int16_t IsClipped(int16_t sX,int16_t sY,int16_t sW,int16_t sH);
	char const *	GetNextLine(); // characters from inputted text to be line formatted, nullptr = done.
	int16_t IsWhiteSpace(int16_t sChar)
		{ if ((sChar == ' ')||(sChar == '\t')) return 1; return 0;}
	int16_t SetCellW();

	//-------------------------- Construction
	RPrint();
	~RPrint();
public:

	int16_t m_sCurX;	// lower left cursor position
	int16_t m_sCurY;

	Mode	m_eModes;	// Set different text modes
	// a 0 signals using the Height relative value
	uint8_t		m_aAbsFlag[NUM_OF_EFFECTS]; 
	int16_t	m_aAbsEffect[NUM_OF_EFFECTS]; // go by this...
	float	m_aARelEffect[NUM_OF_EFFECTS]; 

	//----------------------------- More parameters
	RFont*	m_pfnCurFont; // The high level font

	int16_t m_sNumTabStops;
	int16_t m_sNumDecStops;
	int16_t *m_psTabStops;
	int16_t *m_psDecStops;

	//----------------------------- Working variables
	RFont::RFontSet* m_pCurFontSet; // set to nullptr initially.
	float	m_fWidthScale;				// Compared to current font
	float m_fHeightScale;			// Compared to current font

	int16_t	m_sCellH;	// Current Visual Letter Height:
	int16_t m_sCellW;	// Curent maximum width based on effects

	int16_t m_sUP_W;		// Max width or fixed grid width
	int16_t m_sUP_H;		// Max width or fixed grid width

	int16_t m_sNumWhite;
	//---------------
	int16_t m_sNumChar;
	int16_t m_sExtX;
	//----------------------------- Static variables
	static	uint8_t	ms_szLineText[1024]; // Stores a line at a time
	static	int16_t	ms_sCharPosX[1024]; // positions of each character in the line...
	static	char	ms_szInput[4096]; //
	};

//====================
#endif
