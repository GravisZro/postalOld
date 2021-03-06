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
// localize.cpp
// Project: Postal
//
// This module deals with localization issues.
//
// History:
//    11/19/96 MJR   Started.
//
//    01/21/97 JMI   Added "AssetsMissingError" for when the condition occurs
//                   that is an error when the assets are unfound by the nostril.
//
//    01/22/97 JMI   Added "SaveFileQuery".
//
//    01/22/97 MJR   Added "BadCDPath"_hash, "BadHDPath"_hash, "BadVDPath"
//
//    02/03/97 JMI   Added "BadNoSakDirPath"[].
//
//    04/11/97 JMI   Added "SaveDemoTitle" and "GeneralWriteError".
//
//    04/16/97 MJR   Added a few more messages and cleaned up text for others.
//
//                   And then added a few more video messages, too.
//
//    04/21/97 MJR   Created generic version of "bad path" message.
//
//    05/14/97 JMI   Added "PickedUpMessage_ld" for CDude.
//
//    06/03/97 JMI   Changed "AudioModeError" to "AudioModeGeneralError"
//                   and added "AudioModeInUseError"_hash,
//                   "AudioModeNoDeviceError"_hash,
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
//    07/18/97 BRH   Added strings for new dialogs for loading and saving
//                   games.
//
//    07/21/97 JMI   Added "NoWeaponButHaveAmmo" and "NoWeapon".
//
//    07/28/97 JMI   Added "DispenserNoDispenseeTypeChosen".
//
//    07/30/97 BRH   Added death messages that come up when the CDude dies.
//
//    08/05/97 JMI   Added "DontHaveExecuteWeapon" and
//                   "DontHaveSuicideWeapon".
//                   Also, made g_sLocalizeNumDeadMessages get its value based
//                   on the number of elements in g_apszDeathMessages[] so we
//                   don't have to worry about keeping that up to date.
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
//    08/27/97 JMI   Upgraded "CannotOpenSoundFiles" message.
//
//    09/09/97 MJR   Changed "BadBlueInit" to include reference to DirectX.
//
//             MJR   Further changed "BadBlueInit" to include NT info.
//
//    09/11/97 JMI   Added "PlayOneRealmOnlyMessage".
//
//    09/18/97 JMI   Added localization vars for menus.
//
//    09/18/97 JMI   Added localization vars for SampleMaster categories.
//
//    09/19/97 JMI   Columnized menu strings so it would be easier to copy
//                   and paste from here into Excel and back.
//
//    09/23/97 JMI   Added localizable sections for menus for US/UK, FRENCH, &
//                   GERMAN.
//
//    09/24/97 BRH   Made ini file based on LOCALE.
//
//    09/26/97 JMI   Added French & German for Menus.
//
//    09/29/97 JMI   Added "NoSoundFiles".
//
//    09/30/97 JMI   "CantFindAssets" was missing the word 'in' in the
//                   clause "you may need to change the  '%s =' entry the file
//                   PREFS_FILE to specify where the APP_NAME files are
//                   located" between 'entry' and 'the file'.
//
//    09/30/97 BRH   Changed postalUK.ini to postalEU.ini since it applies
//                   to all of the foreign versions.
//
//    10/07/97 JMI   Changed PostalEU.ini to PostalUK.INI, PostalFr.INI, and
//                   PostalGr.INI.
//
//    10/07/97 BRH   Added Score localizations to the file.
//
//    10/09/97 JMI   Added "VideoChangeDepthErrorUnderGDI" and modified
//                   "VideoChangeDepthError" to only mention the part about
//                   Windows' help under Win32.
//
//    10/13/97 JMI   Added "ControlsMenu_UseJoystick" to localizable texts.
//
//    12/04/97 BRH   Added "StartSinglePlayerMenu_AddOn".
//
//    03/05/98 BRH   Added "WrongCD" message that tells the player that they
//                   must have the original Postal CD in the drive in order to
//                   play the Postal Add on Pack.
//
//    09/27/99 JMI   Added conditions for LOCALE == JAPAN.
//
//    10/07/99 JMI   Added conditions for TARGET == JAPAN_ADD_ON and
//                   TARGET == SUPER_POSTAL.
//
//    11/28/99 MJR   For JAPAN_ADD_ON changed the .ini file to "Postal.ini" so
//                   that the installation process is easier, since it can then
//                   just use the same .ini as the original version of Postal.
//
//    12/02/99 MJR   Changed JAPAN_ADD_ON back to using a special .ini file.
//
//    02/04/00 MJR   Added "PromptForOriginalCD".
//
//    03/30/00 MJR   Moved APP_NAME and PREFS_FILE macro definitions into
//                   CompileOptions.h.
//                   Switched to using new START_MENU_ADDON_ITEM macro to
//                   control whether there's an add-on item on the START menu.
//                   Added new POSTAL_PLUS text.
//
//    04/02/00 MJR   Major changes to lots of error messages.  The idea was to
//                   reduce the verbage and instead refer the user to the
//                   help files for details.
//
//    04/03/00 MJR   New text for POSTAL_PLUS start menu items.
//
//    06/25/01 MJR   Added some new messages.
//
//    05/07/03 MJR   Changed error messages that said "CD-ROM drive" to say
//                "drive it was installed from".  This should help clear
//                up confusion for people that have multiple drives such
//                as DVD, CD-R, etc.
//
////////////////////////////////////////////////////////////////////////////////
#define LOCALIZE_CPP

//#include <RSPiX.h>
#include "Localize.h"
//
#include <Realm.h>

// STL
#include <string>
#include <unordered_map>

// C++
#include <cstring>

////////////////////////////////////////////////////////////////////////////////
// Macros/types/etc.
////////////////////////////////////////////////////////////////////////////////

// CRC32 Table (zlib polynomial)
static constexpr uint32_t crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

// compiletime hashing
template<size_t idx> constexpr uint32_t crc32(const char * str)
  { return (crc32<idx-1>(str) >> 8) ^ crc_table[(crc32<idx-1>(str) ^ str[idx]) & 0x000000FF]; }

// This is the stop-recursion function
template<> constexpr uint32_t crc32<size_t(-1)>(const char *) { return 0xFFFFFFFF; }

constexpr uint32_t operator "" _hash(const char* str, const std::size_t) noexcept
  { return crc32<sizeof(str) - 2>(str) ^ 0xFFFFFFFF; }

// Message used in a few places
#define CD_DRIVE_CHANGE_MESSAGE     "If you added (or removed) a drive to your system after the game was installed, try putting the CD in another drive (if you have more than one) or re-install the game."

#define ADVANCED_USERS_CHANGE_PATH  "For advanced users: Instead of re-installing you can edit the '" PREFS_FILE "' file and change the '%s' entry in the [%s] section to the correct drive letter."

#define ENGLISH_LOCALE (LOCALE == US || LOCALE == UK || LOCALE == JAPAN)

////////////////////////////////////////////////////////////////////////////////
// Variables/data
////////////////////////////////////////////////////////////////////////////////

std::unordered_map<uint32_t, const char*> g_text =
{
  { "AppName"_hash, APP_NAME },

  { "CriticalErrorTitle"_hash, APP_NAME },

  { "BadBlueInit"_hash,
   "A system incompatibility has been detected."
   "\n\n"
#ifdef WIN32
   "Updating your video and/or sound drivers may solve this problem."
   "\n\n"
#endif
   "See code R100 in " APP_NAME " Help for more information." },

  { "VideoModeError"_hash,
   "The required display settings (%s) were reported by the system as being available, "
   "but could not be set properly."
   "\n\n"
   "An updated video driver may solve this problem.  "
   "Contact your video card vendor for more help."
   "\n\n"
   "See code R101 in " APP_NAME " Help for more information." },

  { "VideoChangeDepthError"_hash,
   "The required display settings (%s) could not be set properly."
   "\n\n"
   "The required number of colors is different from your current settings."
   "\n\n"
   "Change your color settings before running " APP_NAME "."
   "\n\n"
   "See code R102 in " APP_NAME " Help for more information." },

  { "VideoDepthError"_hash,
   "The required display settings (%s) could not be set properly."
   "\n\n"
   "Your video card does not appear to support this number of colors."
   "\n\n"
   "An updated video driver could solve this, but it is more likely due to limitations "
   "of your video card hardware.  Contact your video card vendor for more help."
   "\n\n"
   "See code R103 in " APP_NAME " Help for more information." },

  { "VideoResolutionError"_hash,
   "The required display settings (%s) could not be set properly."
   "\n\n"
   "Your video card does not appear to support this pixel area (resolution) with this "
   "number of colors."
   "\n\n"
   "An updated video driver could solve this, but it is more likely due to limitations "
   "of your video card hardware.  Contact your video card vendor for more help."
   "\n\n"
   "See code R104 in " APP_NAME " Help for more information." },

  { "VideoPagesError"_hash,
   "The required display settings (%s) could not be set properly."
   "\n\n"
   "Your video card does not appear to support this number of pages at these settings."
   "\n\n"
   "An updated video driver could solve this, but it is more likely due to limitations "
   "of your video card hardware.  Contact your video card vendor for more help."
   "\n\n"
   "See code R105 in " APP_NAME " Help for more information." },

  { "VideoChangeDepthErrorUnderGDI"_hash,
   "The required display settings (%s) could not be set properly because you are not "
   "using DirectX."
   "\n\n"
   "You can manually change your color settings before running " APP_NAME ", but "
   "we recommend using DirectX, which allows " APP_NAME " to use any color depth."
   "\n\n"
   "See code R106 in " APP_NAME " Help for more information." },

  { "AudioModeGeneralError"_hash,
   "The required audio mode (%s) could not be set properly."
   "\n\n"
   "If the audio device is or was being used, it may be available once the current "
   "sound is done.  Choose 'Retry' if you want to try again."
   "\n\n"
   "If there is no audio device or it does not support the required mode, you can "
   "choose 'Ignore' to continue without audio."
   "\n\n"
   "If you want to stop the program, choose 'Abort'." },

  { "AudioModeInUseError"_hash,
   "The required audio mode (%s) could not be set properly."
   "\n\n"
   "The audio device is or was being used, it may be available once the current "
   "sound is done.  Choose 'Retry' if you want to try again."
   "\n\n"
   "If you would like to continue without sound from " APP_NAME ", you can "
   "choose 'Ignore'."
   "\n\n"
   "If you want to stop the program, choose 'Abort'." },

  { "AudioModeNoDeviceError"_hash,
   "The required audio mode (%s) could not be set properly."
   "\n\n"
   "There is no audio device or the driver is incorrectly installed or missing."
   "\n\n"
   "If you want to continue without audio, choose 'Yes'."
   "\n"
   "If you want to stop the program, choose 'No'."
   "\n\n"
   "Would you like to continue without audio?" },

  { "AudioModeNotSupportedError"_hash,
   "The audio mode (%s) could not be set properly."
   "\n\n"
   "The audio device does not support this mode."
   "\n\n"
   "If you want to stop the program, choose 'Abort'."
   "\n\n"
   "If you want to try another simpler mode that might work "
   "choose 'Retry'."
   "\n\n"
   "If you want to continue without audio, choose 'Ignore'." },

  { "AudioVanillaModeNotSupportedError"_hash,
   "The audio mode (%s) could not be set properly."
   "\n\n"
   "The audio device does not support this mode."
   "\n\n"
   "If you want to continue without audio, choose 'Yes'."
   "\n"
   "If you want to stop the program, choose 'No'."
   "\n\n"
   "Would you like to continue without audio?" },

  { "PrefFileName"_hash, PREFS_FILE },

  { "PrefOpenError"_hash,
   "The preference file '" PREFS_FILE "' could not be opened."
   "\n\n"
   "See code A100 in " APP_NAME " Help for more information." },

  { "PrefReadError"_hash,
   "An error occurred while reading from '" PREFS_FILE "'."
   "\n\n"
   "See code A101 in " APP_NAME " Help for more information." },

  { "PrefWriteError"_hash,
   "An error occurred while saving to '" PREFS_FILE "'."
   "\n\n"
   "Any settings that you may have made will not be saved."
   "\n\n"
   "See code A102 in " APP_NAME " Help for more information." },

  { "PrefReadOnly"_hash,
   "The file '" PREFS_FILE "' is set to 'READ-ONLY'."
   "\n\n"
   "Any settings that you may have made will not be saved."
   "\n\n"
   "See code A103 in " APP_NAME " Help for more information." },

  { "TitleError"_hash,
   "An error occurred during the title sequence."
   "\n\n"
   "See code A104 in " APP_NAME " Help for more information." },

  { "CantFindAssets"_hash,
   "One or more required files could not be found."
   "\n\n"
   CD_DRIVE_CHANGE_MESSAGE
   "\n\n"
   "See code A105 in " APP_NAME " Help for more information." },

  { "WrongCD"_hash,
   "The original Postal CD is not in the drive it was installed from."
   "\n\n"
   "The CD in the drive does not appear to be the original Postal CD."
   "\n\n"
   "Please insert the original Postal CD and click on Retry." },

  { "PromptForOriginalCD"_hash,
   "Please make sure the original Postal CD\n"
   "is in the drive it was installed from." },

  { "NotOnCDROM"_hash,
   "Please insert the " APP_NAME " CD into the drive you used to install it."
   "\n\n"
   CD_DRIVE_CHANGE_MESSAGE
   "\n\n"
   "See code A106 in " APP_NAME " Help for more information." },

  { "GeneralError"_hash,
   "An error has occurred.  This application cannot proceed."
   "\n\n"
   "See code A107 in " APP_NAME " Help for more information." },

  { "BadPath"_hash,
   "One or more file locations for the game are invalid."
   "\n\n"
   CD_DRIVE_CHANGE_MESSAGE
   "\n\n"
   ADVANCED_USERS_CHANGE_PATH
   "\n\n"
   "See code A108 in " APP_NAME " Help for more information." },

  { "BadCDPath"_hash,
   "Make sure the " APP_NAME " CD is in the drive you used to install it."
   "\n\n"
   CD_DRIVE_CHANGE_MESSAGE
   "\n\n"
   ADVANCED_USERS_CHANGE_PATH
   "\n\n"
   "See code A108 in " APP_NAME " Help for more information." },

  { "CannotOpenSoundFiles"_hash,
   "Your audio hardware supports %s, but the " APP_NAME " file(s) associated with that "
   "sound format were not installed."
   "\n\n"
   "See code A109 in " APP_NAME " Help for more information." },

  { "NoSoundFiles"_hash,
   "There is no sound file installed."
   "\n\n"
   "Please run " APP_NAME " Setup and choose a sound option."
   "\n\n"
   "See code A110 in " APP_NAME " Help for more information." },


  { "AssetsMissingError"_hash,
   "One or more files needed by the editor could not be found." },

  { "SaveFileQuery"_hash,
   "Save file before this operation?" },

  { "SaveDemoTitle"_hash,
   "Save Demo" },

  { "SaveGameTitle"_hash,
   "Choose a name for your saved game" },

  { "SaveGameErrorTitle"_hash,
   "Error saving file" },

  { "SaveGameErrorText"_hash,
   "Your game could not be saved.  Check to see if your disk is full." },

  { "LoadGameTitle"_hash,
   "Choose the game you wish to restore" },

  { "FileOpenError"_hash,
   "Unable to open the file '%s'."
   "\n\n"
   "The file may be missing or corrupted, or you may not have permission to open it." },

  { "FileReadError"_hash,
   "An error has occurred while reading from the file '%s'."
   "\n\n"
   "The file may be corrupted, or you may not have permission to access it." },

  { "FileWriteError"_hash,
   "An error has occurred while writing to the file '%s'."
   "\n\n"
   "The file may be corrupted, or you may not have permission to write to it." },


  { "DispenserNoDispenseeTypeChosen"_hash,
   "You must choose a dispensee type or Cancel." },

  { "GenericBrowseFor_Title"_hash,
   "Browse for %s" },

  { "GenericMustBeRelativePath"_hash,
   "You must choose a file below path \"%s\".\n" },

  { "DontDropYourself"_hash,
   "You don't really want to drop yourself!!\n" },

  { "DoofusCannotFindNavNet_EditMode"_hash,
   "Doofus with ID %hd found that ID %hd (its NavNet ID) was not "
   "a NavNet.\n" },

  { "DoofusCannotFindNavNet_PlayMode"_hash,
   "A character with ID %hd was unable to locate its NavNet with "
   "ID %hd.\n" },

  { "PlayOneRealmOnlyMessage"_hash,
   "This version of " APP_NAME " only allows you to play the"
   "\n"
   "levels that it came with."
   "\n" },


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Misc  ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// NOTICE: These aren't currently available in any language other than english!
#if ENGLISH_LOCALE
     { "EditorDisabled"_hash       ,  "The editor is not available in this demo version." },
     { "MultiplayerDisabled"_hash     ,  "Multiplayer is not available in this demo version." },
     { "Buy"_hash                     ,  "You can order the full version of the game from"
                                                "\n\n"
                                                "         www.gopostal.com"
                                                "\n\n"
                                                "You know you want it... what are you waiting for?!" },
#elif LOCALE == GERMAN
#elif LOCALE == FRENCH
#endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Menus ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if ENGLISH_LOCALE ////////////////////////////////////////////////////////////

     { "MainMenu_Title"_hash                      , MAIN_MENU_TITLE },
     { "MainMenu_Start"_hash                      , "START" },
     { "MainMenu_Options"_hash                    , "OPTIONS" },
     { "MainMenu_Editor"_hash                     , "EDITOR" },
     { "MainMenu_Buy"_hash                        , "ORDER INFO" },
     { "MainMenu_Exit"_hash                       , "EXIT" },

     { "VerifyExitMenu_Title"_hash                , "REALLY EXIT?" },
     { "VerifyExitMenu_Yes"_hash                  , "YES" },
     { "VerifyExitMenu_No"_hash                   , "NO" },

     { "VerifyQuitMenu_Title"_hash                , "REALLY QUIT?" },
     { "VerifyQuitMenu_Yes"_hash                  , "YES" },
     { "VerifyQuitMenu_No"_hash                   , "NO" },

     { "GameMenu_Title"_hash                      , "GAME" },
     { "GameMenu_Continue"_hash                   , "CONTINUE" },
     { "GameMenu_Save"_hash                       , "SAVE" },
     { "GameMenu_Options"_hash                    , "OPTIONS" },
     { "GameMenu_Quit"_hash                       , "QUIT" },

     { "EditorMenu_Title"_hash                    , "EDITOR" },
     { "EditorMenu_Continue"_hash                 , "CONTINUE" },
     { "EditorMenu_Options"_hash                  , "OPTIONS" },
     { "EditorMenu_Quit"_hash                     , "QUIT" },

     { "OptionsMenu_Title"_hash                   , "OPTIONS" },
     { "OptionsMenu_Video"_hash                   , "VIDEO" },
     { "OptionsMenu_Audio"_hash                   , "AUDIO" },
     { "OptionsMenu_Controls"_hash                , "CONTROLS" },
     { "OptionsMenu_Multiplayer"_hash             , "MULTIPLAYER" },
     { "OptionsMenu_Performance"_hash             , "PERFORMANCE" },
     { "OptionsMenu_Difficulty"_hash              , "DIFFICULTY" },
     { "OptionsMenu_Crosshair"_hash               , "CROSSHAIR" },

     { "DifficultyMenu_Title"_hash                , "DIFFICULTY" },
     { "DifficultyMenu_SetDifficulty"_hash        , "SET" },

     { "OrganMenu_Title"_hash                     , "SOUND TEST" },
     { "OrganMenu_SpecialKeysHeading"_hash        , "SPECIAL KEYS" },
     { "OrganMenu_NumericKeysFunction"_hash       , "  0 thru 9 - PLAY A SOUND" },
     { "OrganMenu_AlphaKeysFunction"_hash         , "  A thru Z - PLAY A SOUND" },
     { "OrganMenu_TabKeyFunction"_hash            , "  TAB - NEXT SET OF SOUNDS" },
     { "OrganMenu_Exit"_hash                      , "EXIT" },

     { "AudioMenu_Title"_hash                     , "AUDIO" },
     { "AudioMenu_Mixer"_hash                     , "MIXER" },
     { "AudioMenu_SoundTest"_hash                 , "SOUND TEST" },

     { "VideoMenu_Title"_hash                     , "VIDEO" },
     { "VideoMenu_Gamma"_hash                     , "GAMMA" },

     { "ControlsMenu_Title"_hash                  , "CONTROLS" },
     { "ControlsMenu_KeyboardSetup"_hash          , "KEYBOARD SETUP" },
     { "ControlsMenu_MouseSetup"_hash             , "MOUSE SETUP" },
     { "ControlsMenu_TurningSpeeds"_hash          , "TURNING SPEEDS" },
     { "ControlsMenu_UseMouse"_hash               , "USE MOUSE" },
     { "ControlsMenu_HorizMouseSensitivity"_hash  , "HORIZ MOUSE SENS." },
     { "ControlsMenu_VertMouseSensitivity"_hash   , "VERT MOUSE SENS." },
     { "ControlsMenu_JoystickSetup"_hash          , "X CONTROLLER SETUP" },
     { "ControlsMenu_UseJoystick"_hash            , "USE X CONTROLLER" }, // was not in original localizable text.

     { "KeyboardSetupMenu_Title"_hash             , "KEYBOARD SETUP" },

     { "MouseSetupMenu_Title"_hash                , "MOUSE SETUP" },

     { "JoystickSetupMenu_Title"_hash             , "X CONTROLLER SETUP" },

     { "PerformanceMenu_Title"_hash               , "PERFORMANCE" },
     { "PerformanceMenu_Transparency"_hash        , "TRANSPARENCY" },
     { "PerformanceMenu_3dLighting"_hash          , "3D LIGHTING" },
     { "PerformanceMenu_Particles"_hash           , "PARTICLES" },
     { "PerformanceMenu_DynamicVolume"_hash       , "DYNAMIC VOLUME" },
     { "PerformanceMenu_AmbientSounds"_hash       , "AMBIENT SOUNDS" },

     { "RotationSetupMenu_Title"_hash             , "TURNING SPEEDS " },
     { "RotationSetupMenu_RunningSlow"_hash       , "RUNNING (SLOW)" },
     { "RotationSetupMenu_RunningFast"_hash       , "RUNNING (FAST)" },
     { "RotationSetupMenu_StandingSlow"_hash      , "STANDING (SLOW)" },
     { "RotationSetupMenu_StandingFast"_hash      , "STANDING (FAST)" },
     { "RotationSetupMenu_TapDegrees"_hash        , "TAP DEGREES" },
     { "RotationSetupMenu_RestoreDefaults"_hash   , "RESTORE DEFAULTS" },
     { "RotationSetupMenu_RestoreDefaultsOld"_hash, "RESTORE OLD DEFAULTS" },

     { "VolumesMenu_Title"_hash                   , "AUDIO MIXER" },

     { "StartGameMenu_Title"_hash                 , "START GAME" },
     { "StartGameMenu_SinglePlayer"_hash          , "SINGLE PLAYER" },
     { "StartGameMenu_Multiplayer"_hash           , "MULTIPLAYER" },
     { "StartGameMenu_Demo"_hash                  , "DEMO" },

     { "StartSinglePlayerMenu_Title"_hash         , "SINGLE PLAYER" },
#if defined(START_MENU_ADDON_ITEM)
   #if TARGET == JAPAN_ADD_ON
        { "StartSinglePlayerMenu_New"_hash        , "GO POSTAL IN THE USA" },
        { "StartSinglePlayerMenu_AddOn"_hash      , "GO POSTAL IN JAPAN" },
   #elif TARGET == POSTAL_PLUS
        { "StartSinglePlayerMenu_New"_hash        , "NEW GAME (ALL LEVELS)" },
        { "StartSinglePlayerMenu_AddOn"_hash      , "SPECIAL DELIVERY LEVELS" },
   #elif TARGET == POSTAL_2015
        { "StartSinglePlayerMenu_New"_hash        , "NEW GAME" },
        { "StartSinglePlayerMenu_AddOn"_hash      , "SPECIAL DELIVERY" },
   #else
      #error Strings must be customized for current TARGET
   #endif
#else
   #if TARGET == SUPER_POSTAL
        { "StartSinglePlayerMenu_New"_hash        , "GO POSTAL ALL OVER" },
   #elif (TARGET == POSTAL_PLUS || TARGET == POSTAL_2015)
        { "StartSinglePlayerMenu_New"_hash        , "NEW GAME" },
   #else
//      #error Strings must be customized for current TARGET
   #endif
#endif
     { "StartSinglePlayerMenu_LoadLevel"_hash     , "LOAD LEVEL" },
     { "StartSinglePlayerMenu_LoadGame"_hash      , "LOAD GAME" },
     { "StartSinglePlayerMenu_Challenge"_hash     , "GAUNTLET CHALLENGE" },

     { "StartChallengeMenu_Title"_hash            , "CHALLENGE" },
     { "StartChallengeMenu_Gauntlet"_hash         , "THE GAUNTLET" },
     { "StartChallengeMenu_Timed"_hash            , "TIMED" },
     { "StartChallengeMenu_Goal"_hash             , "GOAL" },
     { "StartChallengeMenu_Flag"_hash             , "FLAG" },
     { "StartChallengeMenu_CheckPoint"_hash       , "CHECKPOINT" },

     { "StartMultiplayerMenu_Title"_hash          , "MULTIPLAYER" },
     { "StartMultiplayerMenu_Join"_hash           , "JOIN GAME" },
     { "StartMultiplayerMenu_Host"_hash           , "HOST GAME" },
     { "StartMultiplayerMenu_Options"_hash        , "OPTIONS" },


     { "JoinGameMenu_Title"_hash                  , "JOIN GAME" },
     { "JoinGameMenu_Browse"_hash                 , "BROWSE (LAN only)" },
     { "JoinGameMenu_ConnectTo"_hash              , "CONNECT TO" },

     { "HostGameMenu_Title"_hash                  , "HOST GAME" },
     { "HostGameMenu_Start"_hash                  , "START" },

     { "StartDemoMenu_Title"_hash                 , "DEMO" },
     { "StartDemoMenu_Browse"_hash                , "BROWSE" },
     { "StartDemoMenu_Play"_hash                  , "PLAY" },
     { "StartDemoMenu_Record"_hash                , "RECORD" },

     { "MultiplayerSetupMenu_Title"_hash          , "MULTIPLAYER" },
     { "MultiplayerSetupMenu_Name"_hash           , "NAME" },
     { "MultiplayerSetupMenu_Color"_hash          , "COLOR" },
     { "MultiplayerSetupMenu_Protocol"_hash       , "PROTOCOL" },
     { "MultiplayerSetupMenu_Connection"_hash     , "CONNECTION" },

#elif LOCALE == GERMAN  ///////////////////////////////////////////////////////

     { "MainMenu_Title"_hash                      , MAIN_MENU_TITLE },
     { "MainMenu_Start"_hash                      , "SPIEL STARTEN" },
     { "MainMenu_Options"_hash                    , "OPTIONEN" },
     { "MainMenu_Editor"_hash                     , "EDITOR" },
     { "MainMenu_Buy"_hash                        , "ORDER INFO" },
     { "MainMenu_Exit"_hash                       , "BEENDEN" },

     { "VerifyExitMenu_Title"_hash                , "WIRKLICH BEENDEN?" },
     { "VerifyExitMenu_Yes"_hash                  , "JA" },
     { "VerifyExitMenu_No"_hash                   , "NEIN" },

     { "VerifyQuitMenu_Title"_hash                , "WIRKLICH BEENDEN?" },
     { "VerifyQuitMenu_Yes"_hash                  , "JA" },
     { "VerifyQuitMenu_No"_hash                   , "NEIN" },

     { "GameMenu_Title"_hash                      , "SPIEL" },
     { "GameMenu_Continue"_hash                   , "WEITER" },
     { "GameMenu_Save"_hash                       , "SPEICHERN" },
     { "GameMenu_Options"_hash                    , "OPTIONEN" },
     { "GameMenu_Quit"_hash                       , "BEENDEN" },

     { "EditorMenu_Title"_hash                    , "EDITOR" },
     { "EditorMenu_Continue"_hash                 , "WEITER" },
     { "EditorMenu_Options"_hash                  , "OPTIONEN" },
     { "EditorMenu_Quit"_hash                     , "BEENDEN" },

     { "OptionsMenu_Title"_hash                   , "OPTIONEN" },
     { "OptionsMenu_Video"_hash                   , "VIDEO" },
     { "OptionsMenu_Audio"_hash                   , "AUDIO" },
     { "OptionsMenu_Controls"_hash                , "STEUERUNG" },
     { "OptionsMenu_Multiplayer"_hash             , "MEHRERE SPIELER" },
     { "OptionsMenu_Performance"_hash             , "LEISTUNG" },
     { "OptionsMenu_Difficulty"_hash              , "SCHWIERIGKEIT" },

     { "DifficultyMenu_Title"_hash                , "SCHWIERIGKEIT" },
     { "DifficultyMenu_SetDifficulty"_hash        , "" },

     { "OrganMenu_Title"_hash                     , "SOUND-TEST" },
     { "OrganMenu_SpecialKeysHeading"_hash        , "SONDERTASTEN" },
     { "OrganMenu_NumericKeysFunction"_hash       , "0 BIS 9 - SOUND ABSPIELEN" },
     { "OrganMenu_AlphaKeysFunction"_hash         , "A BIS Z - SOUND ABSPIELEN" },
     { "OrganMenu_TabKeyFunction"_hash            , "TAB - NACHSTE SOUND-REIHE" },
     { "OrganMenu_Exit"_hash                      , "BEENDEN" },

     { "AudioMenu_Title"_hash                     , "AUDIO" },
     { "AudioMenu_Mixer"_hash                     , "MISCHPULT" },
     { "AudioMenu_SoundTest"_hash                 , "SOUND-TEST" },

     { "VideoMenu_Title"_hash                     , "VIDEO" },
     { "VideoMenu_Gamma"_hash                     , "GAMMA" },

     { "ControlsMenu_Title"_hash                  , "STEUERUNG" },
     { "ControlsMenu_KeyboardSetup"_hash          , "TASTATUR-SETUP" },
     { "ControlsMenu_MouseSetup"_hash             , "MAUS-SETUP" },
     { "ControlsMenu_TurningSpeeds"_hash          , "DREHGESCHWINDIGKEITEN" },
     { "ControlsMenu_UseMouse"_hash               , "MAUS VERWENDEN" },
     { "ControlsMenu_HorizMouseSensitivity"_hash  , "HORIZONTALE MAUSBEWEGUNG" },
     { "ControlsMenu_VertMouseSensitivity"_hash   , "VERTIKALE MAUSBEWEGUNG" },
     { "ControlsMenu_JoystickSetup"_hash          , "JOYSTICK-SETUP" },
     { "ControlsMenu_UseJoystick"_hash            , "JOYSTICK VERWENDEN" }, // was not in original localizable text.

     { "KeyboardSetupMenu_Title"_hash             , "TASTATUR-SETUP" },

     { "MouseSetupMenu_Title"_hash                , "MAUS-SETUP" },

     { "JoystickSetupMenu_Title"_hash             , "JOYSTICK-SETUP" },

     { "PerformanceMenu_Title"_hash               , "LEISTUNG" },
     { "PerformanceMenu_Transparency"_hash        , "TRANSPARENZ" },
     { "PerformanceMenu_3dLighting"_hash          , "3D-BELEUCHTUNG" },
     { "PerformanceMenu_Particles"_hash           , "PARTIKEL" },
     { "PerformanceMenu_DynamicVolume"_hash       , "DYNAMISCHE LAUTSTARKE" },
     { "PerformanceMenu_AmbientSounds"_hash       , "UMGEBENDE KLANGE" },

     { "RotationSetupMenu_Title"_hash             , "DREHGESCHWINDIGKEITEN" },
     { "RotationSetupMenu_RunningSlow"_hash       , "LAUFT (LANGSAM)" },
     { "RotationSetupMenu_RunningFast"_hash       , "LAUFT (SCHNELL)" },
     { "RotationSetupMenu_StandingSlow"_hash      , "STEHT (LANGSAM)" },
     { "RotationSetupMenu_StandingFast"_hash      , "STEHT (SCHNELL)" },
     { "RotationSetupMenu_TapDegrees"_hash        , "TIPPEN GRADE" },
     { "RotationSetupMenu_RestoreDefaults"_hash   , "ZURUCKSETZEN" },

     { "VolumesMenu_Title"_hash                   , "AUDIO-MISCHPULT" },

     { "StartGameMenu_Title"_hash                 , "SPIEL STARTEN" },
     { "StartGameMenu_SinglePlayer"_hash          , "EINZELSPIELER" },
     { "StartGameMenu_Multiplayer"_hash           , "MEHRERE SPIELER" },
     { "StartGameMenu_Demo"_hash                  , "DEMO" },

     { "StartSinglePlayerMenu_Title"_hash         , "EINZELSPIELER" },
     { "StartSinglePlayerMenu_New"_hash           , "ORIGINAL SPIEL" },  // Mike's lame translation
     { "StartSinglePlayerMenu_AddOn"_hash         , "ADD-ON SPIEL" },    // Mike's lame translation
     { "StartSinglePlayerMenu_LoadLevel"_hash     , "EBENE LADEN" },
     { "StartSinglePlayerMenu_LoadGame"_hash      , "SPIEL LADEN" },
     { "StartSinglePlayerMenu_Challenge"_hash     , "HERAUSFORDERUNG" },

     { "StartChallengeMenu_Title"_hash            , "HERAUSFORDERUNG" },
     { "StartChallengeMenu_Gauntlet"_hash         , "SPIESSRUTEN" },
     { "StartChallengeMenu_Timed"_hash            , "ZEIT NEHMEN" },
     { "StartChallengeMenu_Goal"_hash             , "ZIEL" },
     { "StartChallengeMenu_Flag"_hash             , "FLAGGE" },
     { "StartChallengeMenu_CheckPoint"_hash       , "CHECKPOINT" },

     { "StartMultiplayerMenu_Title"_hash          , "MEHRERE SPIELER" },
     { "StartMultiplayerMenu_Join"_hash           , "MITSPIELEN" },
     { "StartMultiplayerMenu_Host"_hash           , "HOST-SPIEL" },
     { "StartMultiplayerMenu_Options"_hash        , "OPTIONEN" },


     { "JoinGameMenu_Title"_hash                  , "MITSPIELEN" },
     { "JoinGameMenu_Browse"_hash                 , "DURCHSUCHEN" },
     { "JoinGameMenu_ConnectTo"_hash              , "VERBINDEN MIT" },

     { "HostGameMenu_Title"_hash                  , "HOST-SPIEL" },
     { "HostGameMenu_Start"_hash                  , "START" },

     { "StartDemoMenu_Title"_hash                 , "DEMO" },
     { "StartDemoMenu_Browse"_hash                , "DURCHSUCHEN" },
     { "StartDemoMenu_Play"_hash                  , "ABSPIELEN" },
     { "StartDemoMenu_Record"_hash                , "AUSZEICHNEN" },

     { "MultiplayerSetupMenu_Title"_hash          , "MEHRERE SPIELER" },
     { "MultiplayerSetupMenu_Name"_hash           , "NAME" },
     { "MultiplayerSetupMenu_Color"_hash          , "FARBE" },
     { "MultiplayerSetupMenu_Protocol"_hash       , "PROTOKOLL" },
     { "MultiplayerSetupMenu_Connection"_hash     , "VERBINDUNG" },

#elif LOCALE == FRENCH  ///////////////////////////////////////////////////////

     { "MainMenu_Title"_hash                      , MAIN_MENU_TITLE },
     { "MainMenu_Start"_hash                      , "DEMARRER" },
     { "MainMenu_Options"_hash                    , "OPTIONS" },
     { "MainMenu_Editor"_hash                     , "EDITEUR" },
     { "MainMenu_Buy"_hash                        , "ORDER INFO" },
     { "MainMenu_Exit"_hash                       , "QUITTER" },

     { "VerifyExitMenu_Title"_hash                , "VRAIMENT SORTIR ?" },
     { "VerifyExitMenu_Yes"_hash                  , "OUI" },
     { "VerifyExitMenu_No"_hash                   , "NON" },

     { "VerifyQuitMenu_Title"_hash                , "VRAIMENT QUITTER ?" },
     { "VerifyQuitMenu_Yes"_hash                  , "OUI" },
     { "VerifyQuitMenu_No"_hash                   , "NON" },

     { "GameMenu_Title"_hash                      , "JEU" },
     { "GameMenu_Continue"_hash                   , "CONTINUER" },
     { "GameMenu_Save"_hash                       , "ENREGISTRER" },
     { "GameMenu_Options"_hash                    , "OPTIONS" },
     { "GameMenu_Quit"_hash                       , "QUITTER" },

     { "EditorMenu_Title"_hash                    , "EDITEUR" },
     { "EditorMenu_Continue"_hash                 , "CONTINUER" },
     { "EditorMenu_Options"_hash                  , "OPTIONS" },
     { "EditorMenu_Quit"_hash                     , "QUITTER" },

     { "OptionsMenu_Title"_hash                   , "OPTIONS" },
     { "OptionsMenu_Video"_hash                   , "VIDEO" },
     { "OptionsMenu_Audio"_hash                   , "AUDIO" },
     { "OptionsMenu_Controls"_hash                , "COMMANDES" },
     { "OptionsMenu_Multiplayer"_hash             , "JOUEURS MULTIPLES" },
     { "OptionsMenu_Performance"_hash             , "PERFORMANCE" },
     { "OptionsMenu_Difficulty"_hash              , "DIFFICULTE" },

     { "DifficultyMenu_Title"_hash                , "DIFFICULTE" },
     { "DifficultyMenu_SetDifficulty"_hash        , "DEFINIR" },

     { "OrganMenu_Title"_hash                     , "TEST SONORE" },
     { "OrganMenu_SpecialKeysHeading"_hash        , "TOUCHES SPECIALES" },
     { "OrganMenu_NumericKeysFunction"_hash       , "0 A 9 - JOUER UN SON" },
     { "OrganMenu_AlphaKeysFunction"_hash         , "A A Z - JOUER UN SON" },
     { "OrganMenu_TabKeyFunction"_hash            , "TAB - PROCHAIN JEU DE SONS" },
     { "OrganMenu_Exit"_hash                      , "QUITTER" },

     { "AudioMenu_Title"_hash                     , "AUDIO" },
     { "AudioMenu_Mixer"_hash                     , "MIXER" },
     { "AudioMenu_SoundTest"_hash                 , "TEST SONORE" },

     { "VideoMenu_Title"_hash                     , "VIDEO" },
     { "VideoMenu_Gamma"_hash                     , "GAMMA" },

     { "ControlsMenu_Title"_hash                  , "COMMANDES" },
     { "ControlsMenu_KeyboardSetup"_hash          , "CONFIGURATION CLAVIER" },
     { "ControlsMenu_MouseSetup"_hash             , "CONFIGURATION SOURIS" },
     { "ControlsMenu_TurningSpeeds"_hash          , "VITESSES DE ROTATION" },
     { "ControlsMenu_UseMouse"_hash               , "UTILISER LA SOURIS" },
     { "ControlsMenu_HorizMouseSensitivity"_hash  , "SOURIS HORIZONTALE" },
     { "ControlsMenu_VertMouseSensitivity"_hash   , "SOURIS VERTICALE" },
     { "ControlsMenu_JoystickSetup"_hash          , "CONFIGURATION DE LA MANETTE DE JEU" },
     { "ControlsMenu_UseJoystick"_hash            , "UTILISER LA MANETTE DE JEU" }, // was not in original localizable text.

     { "KeyboardSetupMenu_Title"_hash             , "CONFIGURATION CLAVIER" },

     { "MouseSetupMenu_Title"_hash                , "CONFIGURATION SOURIS" },

     { "JoystickSetupMenu_Title"_hash             , "CONFIGURATION DE LA MANETTE DE JEU" },

     { "PerformanceMenu_Title"_hash               , "PERFORMANCE" },
     { "PerformanceMenu_Transparency"_hash        , "TRANSPARENCE" },
     { "PerformanceMenu_3dLighting"_hash          , "ECLAIRAGE 3D" },
     { "PerformanceMenu_Particles"_hash           , "PARTICULES" },
     { "PerformanceMenu_DynamicVolume"_hash       , "VOLUME DYNAMIQUE" },
     { "PerformanceMenu_AmbientSounds"_hash       , "SONS AMBIANTS" },

     { "RotationSetupMenu_Title"_hash             , "VITESSES DE ROTATION" },
     { "RotationSetupMenu_RunningSlow"_hash       , "COURIR (LENTEMENT)" },
     { "RotationSetupMenu_RunningFast"_hash       , "COURIR (VITE)" },
     { "RotationSetupMenu_StandingSlow"_hash      , "SE TENIR DEBOUT (LENTEMENT)" },
     { "RotationSetupMenu_StandingFast"_hash      , "SE TENIR DEBOUT (VITE)" },
     { "RotationSetupMenu_TapDegrees"_hash        , "DEGRES DE FRAPPE" },
     { "RotationSetupMenu_RestoreDefaults"_hash   , "DEFAUT" },

     { "VolumesMenu_Title"_hash                   , "MIXER AUDIO" },

     { "StartGameMenu_Title"_hash                 , "DEMARRE LE JEU" },
     { "StartGameMenu_SinglePlayer"_hash          , "JOUEUR UNIQUE" },
     { "StartGameMenu_Multiplayer"_hash           , "JOUEURS MULTIPLES" },
     { "StartGameMenu_Demo"_hash                  , "DEMO" },

     { "StartSinglePlayerMenu_Title"_hash         , "JOUEUR UNIQUE" },
     { "StartSinglePlayerMenu_New"_hash           , "CHARGER ORIGINAL" },   // Mike's lame translation
     { "StartSinglePlayerMenu_AddOn"_hash         , "CHARGER ADD-ON" },     // Mike's lame translation
     { "StartSinglePlayerMenu_LoadLevel"_hash     , "CHARGER LE NIVEAU" },
     { "StartSinglePlayerMenu_LoadGame"_hash      , "CHARGER LE JEU" },
     { "StartSinglePlayerMenu_Challenge"_hash     , "DEFI" },

     { "StartChallengeMenu_Title"_hash            , "DEFI" },
     { "StartChallengeMenu_Gauntlet"_hash         , "LE GANT" },
     { "StartChallengeMenu_Timed"_hash            , "CHRONOMETRE" },
     { "StartChallengeMenu_Goal"_hash             , "BUT" },
     { "StartChallengeMenu_Flag"_hash             , "DRAPEAU" },
     { "StartChallengeMenu_CheckPoint"_hash       , "CONTROLE" },

     { "StartMultiplayerMenu_Title"_hash          , "JOUEURS MULTIPLES" },
     { "StartMultiplayerMenu_Join"_hash           , "JOINDRE LE JEU" },
     { "StartMultiplayerMenu_Host"_hash           , "ANIMER LE JEU" },
     { "StartMultiplayerMenu_Options"_hash        , "OPTIONS" },


     { "JoinGameMenu_Title"_hash                  , "JOINDRE LE JEU" },
     { "JoinGameMenu_Browse"_hash                 , "PARCOURIR (RESEAU LOCAL UNIQUEMENT)" },
     { "JoinGameMenu_ConnectTo"_hash              , "SE CONNECTER A" },

     { "HostGameMenu_Title"_hash                  , "ANIMER LE JEU" },
     { "HostGameMenu_Start"_hash                  , "DEMARRER" },

     { "StartDemoMenu_Title"_hash                 , "DEMO" },
     { "StartDemoMenu_Browse"_hash                , "PARCOURIR" },
     { "StartDemoMenu_Play"_hash                  , "JOUER" },
     { "StartDemoMenu_Record"_hash                , "ENREGISTRER" },

     { "MultiplayerSetupMenu_Title"_hash          , "JOUEURS MULTIPLES" },
     { "MultiplayerSetupMenu_Name"_hash           , "NOM" },
     { "MultiplayerSetupMenu_Color"_hash          , "COULEUR" },
     { "MultiplayerSetupMenu_Protocol"_hash       , "PROTOCOLE" },
     { "MultiplayerSetupMenu_Connection"_hash     , "CONNEXION" },


#endif
};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Sample Master ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if ENGLISH_LOCALE

   char const * const g_apszSoundCategories[] =
      {
      "GENERAL",
      "SOUNDTRACK",
      "WEAPONS",
      "FEEDBACK",
      "DESTRUCTION",
      "AMBIENT",
      "DEMON",
      "SHOUTING",
      "PAIN",
      "SUFFERING"
      };

#elif LOCALE == GERMAN

   char const * const g_apszSoundCategories[] =
      {
      "ALLGEMEIN",
      "SOUNDTRACK",
      "WAFFEN",
      "FEEDBACK",
      "ZERSTORUNG",
      "UMGEBUNG",
      "DAMON",
      "GESCHREI",
      "SCHMERZEN",
      "LEIDEN",
      };

#elif LOCALE == FRENCH

   char const * const g_apszSoundCategories[] =
      {
      "GENERAL",
      "PISTE MUSICALE",
      "ARMES",
      "FEED-BACK",
      "DESTRUCTION",
      "AMBIANTE",
      "DEMON",
      "CRIS",
      "DOULEUR",
      "SOUFFRANCE",
      };

#endif

////////////////////////////////////////////////////////////////////////////////
// Score module
////////////////////////////////////////////////////////////////////////////////

#if ENGLISH_LOCALE // ScoreDisplayText

char const * const g_apszScoreDisplayText[] =
   {
   "      Population %d                        Hostiles %d   Killed %d (%d%% / %d%%)", // Standard
   " Time Remaining %d:%2.2d                                Kills %d",           // Timed
   " Time Remaining %d:%2.2d            Kills %d               Remaining %d / %d",     // TimedGoal
   " Time Remaining %d:%2.2d",                                                   // TimedFlag
   " Kills %d                     Remaining %d            Time Elapsed %d:%2.2d",// Goal
   " Time Elapsed %d:%2.2d",                                                     // CaptureFlag
   " Clock %d:%2.2d    You have %d flags    Flags Remaining %d",                 // Checkpoint
   "Time Remaining %d:%2.2d",                                                    // MPTimed
   "Time Remaining %d:%2.2d",                                                    // MPFrag
   "",                                                                           // MPLastMan,
   "",                                                                           // MPCaptureFlag
   "",                                                                           // MPTimedFlag,
   "",                                                                           // MPTimedFrag
   "",                                                                           // MPLastManFrag
   "",                                                                           // MPLastManTimed
   ""                                                                            // MPLastManTimedFrag
   };

#elif LOCALE == GERMAN
char const * const g_apszScoreDisplayText[] =
   {
   "      Leute %d                        Feinden %d   Totungen %d (%d%%)",   // Standard
   " Verbleibende Zeit %d:%2.2d                                Totungen %d",           // Timed
   " Verbleibende Zeit %d:%2.2d        Totungen %d           Verbleibend %d",    // TimedGoal
   " Verbleibende Zeit %d:%2.2d",                                                   // TimedFlag
   " Totungen %d             Verbleibend %d         Vergangene Zeit %d:%2.2d",// Goal
   " Vergangene Zeit %d:%2.2d",                                                     // CaptureFlag
   " Uhr %d:%2.2d    Sie haben %d Flaggen    Verbleibende Flaggen %d",                 // Checkpoint
   "Verbleibende Zeit %d:%2.2d",                                                    // MPTimed
   "Verbleibende Zeit %d:%2.2d",                                                    // MPFrag
   "",                                                                           // MPLastMan,
   "",                                                                           // MPCaptureFlag
   "",                                                                           // MPTimedFlag,
   "",                                                                           // MPTimedFrag
   "",                                                                           // MPLastManFrag
   "",                                                                           // MPLastManTimed
   ""                                                                            // MPLastManTimedFrag
   };

#elif LOCALE == FRENCH
char const * const g_apszScoreDisplayText[] =
   {
   "      Personnes %d                        Ennemis %d   Victimes %d (%d%%)",  // Standard
   " Temps restant %d:%2.2d                                Victimes %d",            // Timed
   " Temps restant %d:%2.2d            Kills %d               Remaining %d",     // TimedGoal
   " Temps restant %d:%2.2d",                                                 // TimedFlag
   " Victimes %d                 Restant %d            Temps ecoule %d:%2.2d",// Goal
   " Temps ecoule %d:%2.2d",                                                     // CaptureFlag
   " Horloge %d:%2.2d    Vous avez %d drapeaux    Drapeaux restants %d",                  // Checkpoint
   "Temps restant %d:%2.2d",                                                     // MPTimed
   "Temps restant %d:%2.2d",                                                     // MPFrag
   "",                                                                           // MPLastMan,
   "",                                                                           // MPCaptureFlag
   "",                                                                           // MPTimedFlag,
   "",                                                                           // MPTimedFrag
   "",                                                                           // MPLastManFrag
   "",                                                                           // MPLastManTimed
   ""                                                                            // MPLastManTimedFrag
   };
#endif // ScoreDisplayText

#if ENGLISH_LOCALE // ScoreGoalText
char const * const g_apszScoreGoalText[] =
   {
   "      You must kill %d%% of the hostiles.",                                  // Standard
   " Score as many kills as possible in the time remaining.",                    // Timed
   " Kill everyone before time runs out.",                                       // TimedGoal
   " Capture the flag before time runs out.",                                    // TimedFlag
   " Kill %d People in as little time as possible.",                             // Goal
   " Capture the flag in as little time as possible.",                           // CaptureFlag
   " Grab as many flags as possible before time runs out.",                      // Checkpoint
   " The player with the most kills when time expires is the winner",            // MPTimed
   " The first player to get %d kills wins",                                     // MPFrag
   "",                                                                           // MPLastMan,
   "",                                                                           // MPCaptureFlag
   "",                                                                           // MPTimedFlag
   " Try to reach %d kills before time expires",                                 // MPTimedFrag
   "",                                                                           // MPLastManFrag
   "",                                                                           // MPLastManTimed
   " There are no time or kill limits on this game - play as long as you like"   // MPLastManTimedFrag
   };

#elif LOCALE == GERMAN
char const * const g_apszScoreGoalText[] =
   {
   "      Sie mussen %d%% Feinde toten.",                                  // Standard
   " Erzielen Sie in der verbleibenden Zeit su viele Totungen wie moglich.",                    // Timed
   " Toten Sie jeden vor Ablauf der Zeit.",                                      // TimedGoal
   " Nehmen Sie vor Ablauf der Zeit die Flagge ein.",                                  // TimedFlag
   " Toten Sie so schnell wie moglich %d Leute.",                             // Goal
   " Nehmen Sie so schnell wie moglich die Flagge ein.",                         // CaptureFlag
   " Holen Sie sich vor Ablauf der Zeit so viele Flaggen wie moglich.",                      // Checkpoint
   " Der Spieler, dar nach Ablauf der Zeit die meisten Totungen hat, ist der Sieger",           // MPTimed
   " Der erste Spieler mit %d Totungen hat gewonnen",                                     // MPFrag
   "",                                                                           // MPLastMan,
   "",                                                                           // MPCaptureFlag
   "",                                                                           // MPTimedFlag
   " Versuchen Sie, vor Ablauf der Zeit %d Totungen zu erreichen",                                 // MPTimedFrag
   "",                                                                           // MPLastManFrag
   "",                                                                           // MPLastManTimed
   " In diesem Spiel gibt es keine Ziet- oder Totungsbeschrakungen - spielen Sie so lange wie Sie wollen"   // MPLastManTimedFrag

   };

#elif LOCALE == FRENCH
char const * const g_apszScoreGoalText[] =
   {
   "      Vous devez tuer %d%% ennemis.",                                  // Standard
   " Faites autant de victimes que possible dans le delai restant.",                   // Timed
   " Tuez tout le monde avant l'expiration du delai.",                                       // TimedGoal
   " Capturez le drapeau avant l'expiration du delai.",                                   // TimedFlag
   " Tuez %d personnes aussi vite que possible.",                             // Goal
   " Capturez le drapeau aussi vite que possible.",                           // CaptureFlag
   " Saisissez autant de drapeaux que possible avant l'expiration dudelai.",                       // Checkpoint
   " Le joueur ayant fait le plus de victimes a l'expiration du delai est le gagnant",          // MPTimed
   " Le premier joueur faisant %d victimes gagne le jeu.",                                      // MPFrag
   "",                                                                           // MPLastMan,
   "",                                                                           // MPCaptureFlag
   "",                                                                           // MPTimedFlag
   " Essayez de faire %d victimes avant l'expiration du delai",                                 // MPTimedFrag
   "",                                                                           // MPLastManFrag
   "",                                                                           // MPLastManTimed
   " Aucune limite de temps ou de victimes dans ce jeu - jouez aussi longtemps que vous le souhaitez."   // MPLastManTimedFrag

   };
#endif // ScoreGoalText

#if ENGLISH_LOCALE // ScoreUnits
// Units for the various scoring types.
char const * const g_apszScoreUnits[] =
      {
      "",            // Standard.
      "Kills",       // Timed.
      "",            // Timed goal.
      "",            // Timed flag.
      "",            // Goal.
      "",            // Capture the flag.
      "Flags",       // Checkpoint.
      "Frags",       // Multiplayer timed.
      "Frags",       // Multiplayer Frag limited.
      "Frags",       // Multiplayer last man standing.
      "Frags",       // Multiplayer capture the flag.
      "Frags",       // Multiplayer Timed capture the flag.
      "Frags",       // Multiplayer timed frags.
      "Frags",       // Multiplayer last man frag limited.
      "Frags",       // Multiplayer last man with time limit.
      "Frags",       // Multiplayer last man frag and time limited.
      };

#elif LOCALE == GERMAN
char const * const g_apszScoreUnits[] =
      {
      "",            // Standard.
      "Totungen",       // Timed.
      "",            // Timed goal.
      "",            // Timed flag.
      "",            // Goal.
      "",            // Capture the flag.
      "Flaggen",        // Checkpoint.
      "Totungen",       // Multiplayer timed.
      "Totungen",       // Multiplayer Frag limited.
      "Totungen",       // Multiplayer last man standing.
      "Totungen",       // Multiplayer capture the flag.
      "Totungen",       // Multiplayer Timed capture the flag.
      "Totungen",       // Multiplayer timed frags.
      "Totungen",       // Multiplayer last man frag limited.
      "Totungen",       // Multiplayer last man with time limit.
      "Totungen",       // Multiplayer last man frag and time limited.
      };

#elif LOCALE == FRENCH
char const * const g_apszScoreUnits[] =
      {
      "",            // Standard.
      "tues",        // Timed.
      "",            // Timed goal.
      "",            // Timed flag.
      "",            // Goal.
      "",            // Capture the flag.
      "Drapeaux",       // Checkpoint.
      "tues",        // Multiplayer timed.
      "tues",        // Multiplayer Frag limited.
      "tues",        // Multiplayer last man standing.
      "tues",        // Multiplayer capture the flag.
      "tues",        // Multiplayer Timed capture the flag.
      "tues",        // Multiplayer timed frags.
      "tues",        // Multiplayer last man frag limited.
      "tues",        // Multiplayer last man with time limit.
      "tues",        // Multiplayer last man frag and time limited.
      };
#endif // ScoreUnits

#if ENGLISH_LOCALE // ScoreExplanations
// Explanations for the various scoring types.
char const * const g_apszScoreExplanations[] =
      {
      "",                                    // Standard.
      "Most kills in %s",                    // Timed.
      "Best times for killing %d hostiles",  // Timed goal.
      "Best times for capturing the flag",   // Timed flag.
      "Best times for killing %d hostiles",  // Goal.
      "Best times for capturing the flag",   // Capture the flag.
      "Most flags collected",                // Checkpoint.
      "Top %d Scores",                       // Multiplayer timed.
      "Top %d Scores",                       // Multiplayer frag.
      "Top %d Scores",                       // Multiplayer last man standing.
      "Top %d Scores",                       // Multiplayer capture the flag.
      "Top %d Scores",                       // Multiplayer Timed capture the flag.
      "Top %d Scores",                       // Multiplayer timed frags.
      "Top %d Scores",                       // Multiplayer last man frag limited.
      "Top %d Scores",                       // Multiplayer last man with time limit.
      "Top %d Scores",                       // Multiplayer last man frag and time limited.
      };

#elif LOCALE == GERMAN
char const * const g_apszScoreExplanations[] =
      {
      "",                                    // Standard.
      "Die meisten Totungen in %s",                   // Timed.
      "Bestzeiten beim Toten von %d Feinden",   // Timed goal.
      "Bestzeiten beim Einnehmen der Flagge",   // Timed flag.
      "Bestzeiten beim Toten von %d Fienden",   // Goal.
      "Bestzeiten beim Einnehmen der Flagge",   // Capture the flag.
      "Meiste gesammelte Flaggen",                 // Checkpoint.
      "Hochste %d Punktzahlen",                       // Multiplayer timed.
      "Hochste %d Punktzahlen",                       // Multiplayer frag.
      "Hochste %d Punktzahlen",                       // Multiplayer last man standing.
      "Hochste %d Punktzahlen",                       // Multiplayer capture the flag.
      "Hochste %d Punktzahlen",                       // Multiplayer Timed capture the flag.
      "Hochste %d Punktzahlen",                       // Multiplayer timed frags.
      "Hochste %d Punktzahlen",                       // Multiplayer last man frag limited.
      "Hochste %d Punktzahlen",                       // Multiplayer last man with time limit.
      "Hochste %d Punktzahlen",                       // Multiplayer last man frag and time limited.
      };

#elif LOCALE == FRENCH
char const * const g_apszScoreExplanations[] =
      {
      "",                                    // Standard.
      "Maximum de victimes en %s",                    // Timed.
      "Meilleurs temps pour tuer %d ennemis",   // Timed goal.
      "Meilleurs temps de capture des drapeaux",   // Timed flag.
      "Meilleurs temps pour tuer %d ennemis",   // Goal.
      "Meilleurs temps de capture des drapeaux",   // Capture the flag.
      "Nombre maximum de drapeaux amasses",                 // Checkpoint.
      "%d Meilleurs Scores",                       // Multiplayer timed.
      "%d Meilleurs Scores",                       // Multiplayer frag.
      "%d Meilleurs Scores",                       // Multiplayer last man standing.
      "%d Meilleurs Scores",                       // Multiplayer capture the flag.
      "%d Meilleurs Scores",                       // Multiplayer Timed capture the flag.
      "%d Meilleurs Scores",                       // Multiplayer timed frags.
      "%d Meilleurs Scores",                       // Multiplayer last man frag limited.
      "%d Meilleurs Scores",                       // Multiplayer last man with time limit.
      "%d Meilleurs Scores",                       // Multiplayer last man frag and time limited.
      };
#endif // ScoreExplanations

const char* operator "" _lookup(const char* str, const std::size_t) noexcept
  { return g_text.at(crc32<sizeof(str) - 2>(str) ^ 0xFFFFFFFF); }

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
