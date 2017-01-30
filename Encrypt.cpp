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
// Module Name:   Encrypt.cpp
//
// Description:   This is the encryption module of Syndicated Game Shell.
//
// History:
//
//    03/25/96 AJM      Created.
//
//    07/10/97 BRH      Commented out #include AppMain.h so that
//                      it will work with Postal.  It doesn't seem to
//                      need that file.
//
//    07/10/97 BRH      Branched this version of encrypt to use
//                      Postal's GetRandom() rather than rand();
//
//    07/11/97 BRH      Changed Encrypt to return the length of
//                      the buffer that needs to be stored, and
//                      changed Decrypt to subtract 2 from that
//                      length to use as the number of bytes to
//                      decrypt.
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// Windows Headers.
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// C Headers.
//////////////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <climits>
#include <RSPiX.h>
#include <Game.h>
//////////////////////////////////////////////////////////////////////////////////////
// Local Headers.
//////////////////////////////////////////////////////////////////////////////////////
#include "Encrypt.h"
//////////////////////////////////////////////////////////////////////////////////////
// Module specific macros.
//////////////////////////////////////////////////////////////////////////////////////
#define NUM_KEYS              10
#define KEY_LENGTH            128
#define MAX_STRING_LENGTH     256

#define CRCPOLY  0xA001  /* ANSI CRC-16 */
                         /* CCITT: 0x8408 */

#define UPDATE_CRC(c) \
   ms_usCRC = ms_usCRCtable[(ms_usCRC ^ (c)) & 0xFF] ^ (ms_usCRC >> CHAR_BIT)

//////////////////////////////////////////////////////////////////////////////////////
// Module specific typedefs.
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// Module specific (static) globals.
//////////////////////////////////////////////////////////////////////////////////////
static char const *ms_seeds[NUM_KEYS] = {
"S3D5Lf6klfdsjiureLJKHLKmnblkjshgwieourLKHLKJHSDA0432175094SJAKLFK7348LSDLKJDHFOIUhjgdskgfeiurytowelxcmnvbdfhgeoriyfdslkhgfdgyuie",
"KlkjILKJ879IHKJ8kj67kjh7IUKJHjkh98769872vYUTui6JHG87jhg78JHg787JHGUYuytuyjg876RHCVBnvbAIUT678787igJGFHGFTy67uhHJKJHUYTjhg6HGHG76",
"4LKJH5Euhysdiouyfjlkw45879345kjshfdgcvxJSDHFR8734hdskmfv7y8KJLSDGFAVBMZXBV789kMNADFYUYjhkgkjhdsgfowe787243KJHGDSAGJKkjgdfuye3sd7",
"l45kjhdfuihlIUYSLKHi87ylKJHYOIUmsagKJLHgY9802734KJHLKiuoymNJHTmnb4iU7687nkmbojyTD64654HJG765KHKJhg87jKHKJ76dcbnmbDBDSJFDHJ6IHGHJ",
"fdlkhgljslfkdfhvcxzb 902374jklhfdgoIUOYLSKFDJH98798ysdlk9870OLHL987sdxmncosidHGKJ876kjhtdg786JHG87kj76Kjhgkjhi8K8HG98kgdnb mvzms",
"lsdhflkasjhLKJHSLKHAFD987435LRKJHL34UY5L34J64MNB7IU4Yiuoyt87sdfjshg87ahjkjhgds87JHGKJHT8kjhgdfsvbmyt4eu678JHGkjyt6jKH78k0kjt02KJ",
"lkjhUIYkljhlUYOI6yt9876HJMNBVuyt97JKHfMvhjytrfiyIUHGKVBMNBuyi9872548JKN76kj9fsd8KHG8JKg7872732jK8JgkjhKJHGKJHgkjhg7jkhgkjhgkjhgk",
"dsdfLKJH;LKjKJFLdJGmhpi657oU6Y76fhgf5dyfh879346nmzmbvc56erter654gvcbmzwsdfdsmxcngfvRDFrEDfg76bDewdfvdfgwqsdfdgfcrRDfhgfderfdfteP",
"lkhfdsRE546457djh6g65l67KJHLkhmnbvxcbDF09KLJliuyfdsgxcvbFDGERsdasdfgTEdhFDtYrt8765DFRrfgfhvhghretwRF67654DFhDGFDVctTRTtrytrRyiTY",
"28734jsdfhg873jksgUIYTIUkhgiuT7YIGVBKjg8796KHG8o76HJG876tKJHG8976GFd6584H6G67RT987kJHG65978JKHG8JHGKJHGkhjgkjhgidsytf87439649283"
};

static uint16_t ms_usCRCtable[UINT8_MAX + 1];
static uint16_t ms_usCRC;

//////////////////////////////////////////////////////////////////////////////////////
// Modular functions.
//////////////////////////////////////////////////////////////////////////////////////
int16_t Verify(char const *szSource, int16_t sLength, uint16_t usCRC);
void MakeCRCTable();

//////////////////////////////////////////////////////////////////////////////////////
// Globally externable variables.
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// External Functions.
//////////////////////////////////////////////////////////////////////////////////////

int Decrypt(char const * cipher, char * plain, size_t cipherLen) {
   size_t plainLen   = cipherLen - 2;
   int16_t sStartIndex = cipher[1];
   int16_t sCurrentKey = cipher[0];
   int16_t sSeedIndex  = sStartIndex;

   if(sCurrentKey >= NUM_KEYS || sCurrentKey < 0) {
         TRACE("ERROR: Invalid key.\n");
         return -1;
   }

   if(sStartIndex >= KEY_LENGTH || sStartIndex < 0) {
      TRACE("ERROR: Invalid start position.");
      return -2;
   }

   char * tmp = (char*)malloc(cipherLen+1);
   memcpy(tmp, cipher, cipherLen+1);
   for(size_t i = plainLen; i > 2; --i)
      tmp[i] ^= tmp[i-1];
   tmp[2] ^= tmp[1];

   for(size_t i = 2; i < cipherLen; ++i) {
      plain[i-2] = tmp[i] ^ (~ms_seeds[sCurrentKey][sSeedIndex]);
      sSeedIndex = (sSeedIndex+1) % KEY_LENGTH;
   }
   plain[plainLen] = 0;
   free(tmp);

   return 0;
}

size_t Encrypt(char const * plain, char * cipher, size_t plainLen) {
   size_t cipherLen  = plainLen + 2;
   int16_t sStartIndex = GetRandom() % KEY_LENGTH;
   int16_t sCurrentKey = GetRandom() % NUM_KEYS;
   int16_t sSeedIndex  = sStartIndex;

   //store start point in seed
   cipher[0] = (char)sCurrentKey;
   cipher[1] = (char)sStartIndex;

   //pass one xor against the seed
   for(size_t i = 0; i < plainLen; ++i) {
      cipher[i+2]=plain[i]^(~ms_seeds[sCurrentKey][sSeedIndex]);
      sSeedIndex = (sSeedIndex+1) % KEY_LENGTH;
   }
   cipher[cipherLen] = 0;

   //pass two xor against previous char
   for(size_t i = 2; i < cipherLen - 1; ++i) {
      cipher[i] ^= cipher[i-1];
   }

   return cipherLen;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// Function Name: Encrypt
//
// Description:   This is the file encryption function.
//
// Input:         char const *: name of file to encrypt to,
//                char *: nullptr terminated plaintext to encrypt
//
// Output:        0 on success.
//
// History:       03/25/97, AJM,    Created.
//
//////////////////////////////////////////////////////////////////////////////////////
int16_t Encrypt(char const * szFileName, char const * szInputString)
   {
   int16_t rc=-1,   //assume failure
         sLength=strlen(szInputString);
   RFile cnFile;
   char  szEncrypted[MAX_STRING_LENGTH];

   ms_usCRC=0;

   if(cnFile.Open(szFileName,"wb",RFile::LittleEndian)==0)
      {
      if(Encrypt(szInputString,szEncrypted,sLength)==0)
         {
         MakeCRCTable();

         for (int16_t i=0;i<sLength;i++)
            UPDATE_CRC(szEncrypted[i]);

         //grow 2 bytes for internal info
         cnFile.Write((int16_t*)&sLength);
         cnFile.Write((int8_t*) szEncrypted,sLength+2);
         cnFile.Write((uint16_t*)&ms_usCRC);

         rc=0;
         }

      cnFile.Close();
      }

   return rc;
   }

//////////////////////////////////////////////////////////////////////////////////////
//
// Function Name: Decrypt
//
// Description:   This is the file decryption function.
//
// Input:         char const *: name of file to be decrypted
//                char *: text to decrypt into
//
// Output:        0 on success.
//
// History:       03/25/97, AJM,    Created.
//
//////////////////////////////////////////////////////////////////////////////////////
int16_t Decrypt(char const * szFileName, char * szOutputString)
   {
   int16_t rc=-1,      //assume failure
         sLength;
   RFile cnFile;
   char szDecrypted[MAX_STRING_LENGTH];
   uint16_t usCRC=0;

   if(cnFile.Open(szFileName,"rb",RFile::LittleEndian)==0)
      {
      MakeCRCTable();

      cnFile.Read((int16_t*)&sLength);
      cnFile.Read((int8_t*) szDecrypted,sLength+2);
      cnFile.Read((uint16_t*)&usCRC);

      if(Verify(szDecrypted,sLength,usCRC)==0)
         rc=Decrypt(szDecrypted,szOutputString,sLength);

      cnFile.Close();
      }

   return rc;
   }

//////////////////////////////////////////////////////////////////////////////////////
//
// Function Name: Verify
//
// Description:   This verifys that the data in the encrypted string hasnt been corrupted
//
// Input:         char const *: string to insert checksums into
//                int16_t: length of string
//
// Output:        none
//
// History:       03/25/97, AJM,    Created.
//
//////////////////////////////////////////////////////////////////////////////////////
int16_t Verify(char const *szSource, int16_t sLength, uint16_t usCRC)
   {
   int16_t rc=0; //assume success

   ms_usCRC=0;

   for (int16_t i=0;i<sLength;i++)
      UPDATE_CRC(szSource[i]);

   if(ms_usCRC!=usCRC)
      {
      TRACE("Error: CRC failure !!!  Old CRC: %hu  New CRC: %hu\n");
      rc=-1;
      }

   return rc;
   }


void MakeCRCTable()
   {
   uint16_t i, j, r;

   for (i = 0; i <= UINT8_MAX; i++)
      {
      r = i;
      for (j = 0; j < CHAR_BIT; j++)
         if (r & 1) r = (r >> 1) ^ CRCPOLY;
         else       r >>= 1;
      ms_usCRCtable[i] = r;
      }

   return;
   }

//////////////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////////////
