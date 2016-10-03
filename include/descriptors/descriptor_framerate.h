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

#ifndef __TSSI_DESCRIPTOR_FRAMERATE_H_INCLUDED__
#define __TSSI_DESCRIPTOR_FRAMERATE_H_INCLUDED__

#include "descriptors/descriptor.h"

namespace tssi {

	class TS_EXTERN Descriptor_FrameRate : public Descriptor	{
	private:
		TS_BOOL	m_multiple_frame_rate_flag;
		TS_BYTE	m_frame_rate_code;

	public:
		static const TS_BYTE kDescriptorTag = 0x82;

		Descriptor_FrameRate();
		virtual ~Descriptor_FrameRate();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		TS_BOOL	GetMultipleFrameRateFlag() const;	// true if multiple frame rates are used
		TS_BYTE	GetFrameRateCode() const;
			// frame_rate_code   Meaning                     Also Includes
			// ‘0000’            forbidden
			// ‘0001’            24.000 ÷ 1.001 = 23.976...  -
			// ‘0010’            24.000                      23.976
			// ‘0011’            25.000                      -
			// ‘0100’            30.000 ÷ 1.001 = 29.97...   23.976
			// ‘0101’            30.000                      23.976, 24.000, 29.97
			// ‘0110’            50.000                      25.000
			// ‘0111’            60.000 ÷ 1.001 = 59.94...   23.976, 29.97
			// ‘1000’            60.000                      23.976, 24.000, 29.97, 30.000, 59.94
			// ‘1000’-’1111’     Reserved
	};

}

#endif __TSSI_DESCRIPTOR_FRAMERATE_H_INCLUDED__