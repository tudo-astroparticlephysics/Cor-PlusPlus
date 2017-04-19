/* char_to_byte.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <string>
#include <array>
#include <stdint.h>
#include <cstring>

namespace lib
{
	namespace algorithm
	{

		/// conversion from char to ascii
		/**
		*   Generic conversion function
		*	\tparam TSize Size of char to convert
		*   \param obj Object contains the data to convert
		*
		*   \return Returnes the char array to ascii
		*/
		template<size_t TSize>
		inline std::array<uint8_t, TSize> char_to_byte(const char(&p_obj)[TSize])
		{
			std::array<uint8_t, TSize> converted;
			memset(converted.data(), 0, TSize);

			for (unsigned int i = 0; i < TSize; i++)
			{
				uint8_t ascii = 0;

				switch (p_obj[i])
				{
				case '\t':  ascii = 0x09; break;
				case ' ':  ascii = 0x20; break;
				case '!':  ascii = 0x21; break;
				case '\"':  ascii = 0x22; break;
				case '#':  ascii = 0x23; break;
				case '$':  ascii = 0x24; break;
				case '%':  ascii = 0x25; break;
				case '&':  ascii = 0x26; break;
				case '\'':  ascii = 0x27; break;
				case '(':  ascii = 0x28; break;
				case ')':  ascii = 0x29; break;
				case '*':  ascii = 0x2A; break;
				case '+':  ascii = 0x2B; break;
				case ',':  ascii = 0x2C; break;
				case '-':  ascii = 0x2D; break;
				case '.':  ascii = 0x2E; break;
				case '/':  ascii = 0x2F; break;
				case '0':  ascii = 0x30; break;
				case '1':  ascii = 0x31; break;
				case '2':  ascii = 0x32; break;
				case '3':  ascii = 0x33; break;
				case '4':  ascii = 0x34; break;
				case '5':  ascii = 0x35; break;
				case '6':  ascii = 0x36; break;
				case '7':  ascii = 0x37; break;
				case '8':  ascii = 0x38; break;
				case '9':  ascii = 0x39; break;
				case ':':  ascii = 0x3A; break;
				case ';':  ascii = 0x3B; break;
				case '<':  ascii = 0x3C; break;
				case '=':  ascii = 0x3D; break;
				case '>':  ascii = 0x3E; break;
				case '?':  ascii = 0x3F; break;
				case '@':  ascii = 0x40; break;
				case 'A':  ascii = 0x41; break;
				case 'B':  ascii = 0x42; break;
				case 'C':  ascii = 0x43; break;
				case 'D':  ascii = 0x44; break;
				case 'E':  ascii = 0x45; break;
				case 'F':  ascii = 0x46; break;
				case 'G':  ascii = 0x47; break;
				case 'H':  ascii = 0x48; break;
				case 'I':  ascii = 0x49; break;
				case 'J':  ascii = 0x4A; break;
				case 'K':  ascii = 0x4B; break;
				case 'L':  ascii = 0x4C; break;
				case 'M':  ascii = 0x4D; break;
				case 'N':  ascii = 0x4E; break;
				case 'O':  ascii = 0x4F; break;
				case 'P':  ascii = 0x50; break;
				case 'Q':  ascii = 0x51; break;
				case 'R':  ascii = 0x52; break;
				case 'S':  ascii = 0x53; break;
				case 'T':  ascii = 0x54; break;
				case 'U':  ascii = 0x55; break;
				case 'V':  ascii = 0x56; break;
				case 'W':  ascii = 0x57; break;
				case 'X':  ascii = 0x58; break;
				case 'Y':  ascii = 0x59; break;
				case 'Z':  ascii = 0x5A; break;
				case '[':  ascii = 0x5B; break;
				case '\\':  ascii = 0x5C; break;
				case ']':  ascii = 0x5D; break;
				case '^':  ascii = 0x5E; break;
				case '_':  ascii = 0x5F; break;
				case '`': ascii = 0x60; break;
				case 'a': ascii = 0x61; break;
				case 'b': ascii = 0x62; break;
				case 'c': ascii = 0x63; break;
				case 'd': ascii = 0x64; break;
				case 'e': ascii = 0x65; break;
				case 'f': ascii = 0x66; break;
				case 'g': ascii = 0x67; break;
				case 'h': ascii = 0x68; break;
				case 'i': ascii = 0x69; break;
				case 'j': ascii = 0x6A; break;
				case 'k': ascii = 0x6B; break;
				case 'l': ascii = 0x6C; break;
				case 'm': ascii = 0x6D; break;
				case 'n': ascii = 0x6E; break;
				case 'o': ascii = 0x6F; break;
				case 'p': ascii = 0x70; break;
				case 'q': ascii = 0x71; break;
				case 'r': ascii = 0x72; break;
				case 's': ascii = 0x73; break;
				case 't': ascii = 0x74; break;
				case 'u': ascii = 0x75; break;
				case 'v': ascii = 0x76; break;
				case 'w': ascii = 0x77; break;
				case 'x': ascii = 0x78; break;
				case 'y': ascii = 0x79; break;
				case 'z': ascii = 0x7A; break;
				case '{': ascii = 0x7B; break;
				case '|': ascii = 0x7C; break;
				case '}': ascii = 0x7D; break;
				case '~': ascii = 0x7E; break;
				default:
					ascii = 0x00; break;
				}
				converted[i] = ascii;
			}
			return converted;
		}


	}
}






