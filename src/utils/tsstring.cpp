/*++
*    libtssi - A library for parsing MPEG-2 TS and DVB Service Information
*
*    Copyright (C) 2009, 2016 Martin Hoernig (goforcode.com)
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
--*/

#include "utils/tsstring.h"

TS_STRING tssi::ConvertBufferToString(TS_PBYTE pbuf, unsigned in_len) 
{
	// this code is preliminary and has to be changed

	char* text_buffer = new char[in_len];

	unsigned x;
	unsigned y = 0;
	for (x = 0; x < in_len; ++x)
		if ((x != 0) || (pbuf[0] >= 0x20))
			if ((pbuf[0] != 0x10) || ( x > 2 ))
				if ((pbuf[x]<0x80) || (pbuf[x]>0x9f)) {
					text_buffer[y] = pbuf[x];
					++y; 
				}
				
	TS_STRING text (text_buffer, y);
	delete[] text_buffer;

	return text;
}