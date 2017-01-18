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
// This is the header file for Encrypt.cpp
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _ENCRYPT_H
#define _ENCRYPT_H

size_t Encrypt(char const * plain, char * cipher, size_t sSourceLength);
int Decrypt(char const * cipher, char * plain, size_t sSourceLength);
int16_t Encrypt(char const * szFileName, char const * szIntputString);
int16_t Decrypt(char const * szFileName, char * szOutputString);

#endif
//////////////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////////////
