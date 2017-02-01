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
// localize.h
// Project: Postal
//
// History:
//    02/03/97 JMI   Added "BadNoSakDirPath".
//
//    04/11/97 JMI   Added "SaveDemoTitle" and "GeneralWriteError").
//
//    04/16/97 MJR   Added a few more messages.
//
//                   And then added a few more video messages, too.
//
//    04/21/97 MJR   Created generic version of "bad path" message.
//
//    05/14/97 JMI   Added "PickedUpMessage_ld" for CDude.
//
//    06/03/97 JMI   Changed "AudioModeError" to "AudioModeGeneralError")
//                   and added "AudioModeInUseError",
//                   "AudioModeNoDeviceError",
//                   "AudioModeNotSupportedError".
//
//    06/04/97 JMI   Added "NotOnCDROM".
//
//    06/06/97 JMI   Changed message for "PickedUpMessage_ld" and its name
//                   to "PickedUpMessage".
//
//    06/14/97 MJR   Added/enhanced general file error messages.
//
//    07/13/97 JMI   Changed "AudioModeNotSupportedError" to
//                   "AudioModeNotSupportedError" and changed to ask the
//                   user if they want to try the vanilla mode.
//                   Also, changed the other audio errors to include the
//                   sprintf format parameters in their names.
//                   Also, added "AudioVanillaModeNotSupportedError" for
//                   the case when the hardware does not support the vanilla
//                   mode.
//
//    07/21/97 JMI   Added "NoWeaponButHaveAmmo" and "NoWeapon").
//
//    07/28/97 JMI   Added "DispenserNoDispenseeTypeChosen".
//
//    08/05/97 JMI   Added "DontHaveExecuteWeapon" and
//                   "DontHaveSuicideWeapon".
//
//    08/12/97 JMI   Added "GenericBrowseFor_Title" and
//                   "GenericMustBeRelativePath".
//
//    08/17/97 JMI   Got rid of m_szMessages and all message related functions
//                   and variables from CDude since we are now using the toolbar
//                   for dude status feedback to the user.  This includes:
//                   MsgTypeInfo, m_lNextStatusUpdateTime, m_lMsgUpdateDoneTime,
//                   m_print, m_bClearedStatus, m_szMessages[], m_sDeadMsgNum,
//                   ms_amtfMessages[], ms_u8FontForeIndex, ms_u8FontBackIndex,
//                   ms_u8FontShadowIndex, DrawStatus(), StatusChange(),
//                   MessageChange(), Message(), UpdateFontColors(),
//                   CPowerUp::ms_apszPowerUpTypeNames[],
//                   CPowerUp::GetDescription(), and some strings and a string
//                   array in localize.*.
//
//    08/20/97 JMI   Added "DontDropYourself".
//
//    08/21/97 JMI   Added "DoofusCannotFindNavNet_EditMode" and
//                   "DoofusCannotFindNavNet_PlayMode".
//
//    08/25/97 JMI   Added "CannotOpenSoundFiles".
//
//    09/11/97 JMI   Added "PlayOneRealmOnlyMessage".
//
//    09/18/97 JMI   Added localization vars for menus.
//
//    09/18/97 JMI   Added localization vars for SampleMaster categories.
//
//    09/29/97 JMI   Added "NoSoundFiles".
//
//    10/07/97 BRH   Added score localizations.
//
//    10/09/97 JMI   Added "VideoChangeDepthErrorUnderGDI".
//
//    10/13/97 JMI   Added "ControlsMenu_UseJoystick" to localizable texts.
//
//    12/04/97 BRH   Added "StartSinglePlayerMenu_AddOn".
//
//    03/05/98 BRH   Added "WrongCD" message that tells the player that they
//                   must have the original Postal CD in the drive in order to
//                   play the Postal Add on Pack.
//
//    02/04/00 MJR   Added "PromptForOriginalCD".
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef LOCALIZE_H
#define LOCALIZE_H


// C++
#include <cstdint>

extern const char* operator "" _lookup(const char* str, const std::size_t sz) noexcept;


// I went with variables instead of macros for one primary reason.  At some
// point, most likely near the end of the project, when time is in short supply,
// someone will need to go through and clean up all these strings.  If these
// were macros, then nearly every file in the project would need to recompile
// each time this file was changed.  With strings, you generally don't modify
// this file, but instead modify the .CPP file, so only that file needs to be
// recompiled.  The disadvantage is that whenever you need to add, delete, or
// rename strings, you have to do it both here and in the .CPP file.

extern char const * const g_apszScoreDisplayText[];
extern char const * const g_apszScoreGoalText[];
extern char const * const g_apszScoreUnits[];
extern char const * const g_apszScoreExplanations[];

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Sample Master ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern char const * const g_apszSoundCategories[];


#endif // LOCALIZE_H
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
