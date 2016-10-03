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

#include "descriptors/descriptor_framerate.h"
#include "utils/parsemac2.h"

tssi::Descriptor_FrameRate::Descriptor_FrameRate() : Descriptor()
{
	m_multiple_frame_rate_flag = TS_FALSE;
	m_frame_rate_code = 0;
}

tssi::Descriptor_FrameRate::~Descriptor_FrameRate()
{
	Descriptor::~Descriptor();
}

TS_VOID tssi::Descriptor_FrameRate::Reset()
{
	Descriptor::Reset();
	m_multiple_frame_rate_flag = TS_FALSE;
	m_frame_rate_code = 0;
}

TS_BOOL	tssi::Descriptor_FrameRate::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	pbuf += 2;

	m_multiple_frame_rate_flag = BIT_VALUE(BYTE_VALUE(pbuf,0),7);
	m_frame_rate_code = (BYTE_VALUE(pbuf,0) >> 3) & 0xf;

	return TS_TRUE;
}

TS_BOOL tssi::Descriptor_FrameRate::GetMultipleFrameRateFlag() const
{
	return m_multiple_frame_rate_flag;
}

TS_BYTE tssi::Descriptor_FrameRate::GetFrameRateCode() const
{
	return m_frame_rate_code;
}