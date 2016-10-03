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

#include "descriptors/descriptor_content.h"
#include "utils/parsemac2.h"

tssi::Descriptor_Content::Descriptor_Content() : Descriptor()
{
}

tssi::Descriptor_Content::~Descriptor_Content()
{
	Descriptor::~Descriptor();
	m_nibbles.clear();
}

TS_VOID tssi::Descriptor_Content::Reset()
{
	Descriptor::Reset();
	m_nibbles.clear();
}

TS_BOOL tssi::Descriptor_Content::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf, in_len)) return TS_FALSE;	
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	pbuf += 2;
		
	unsigned nibble_count = (m_descriptor_length >> 1);
	m_nibbles.resize(nibble_count);

	unsigned x;
	for (x = 0; x < nibble_count; ++x)
		m_nibbles[x] = pbuf[x << 1];
	
	return TS_TRUE;
}

unsigned tssi::Descriptor_Content::GetNibbleListLength() const
{
	return static_cast<unsigned> ( m_nibbles.size() );
}

TS_BYTE tssi::Descriptor_Content::GetContentNibbleLevel1(unsigned index) const
{
	if (index >= GetNibbleListLength()) return 0; 
	return m_nibbles[index] >> 4;
}

TS_BYTE tssi::Descriptor_Content::GetContentNibbleLevel2(unsigned index) const
{
	if (index >= GetNibbleListLength()) return 0; 
	return m_nibbles[index] & 0xF;
}
