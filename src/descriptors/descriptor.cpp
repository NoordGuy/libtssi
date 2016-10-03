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

#include "descriptors/descriptor.h"
#include "utils/parsemac2.h"

tssi::Descriptor::Descriptor() 
{
	m_descriptor_tag = 0;
	m_descriptor_length = 0;
}

tssi::Descriptor::~Descriptor() 
{
}

TS_VOID tssi::Descriptor::Reset()
{
	m_descriptor_tag = 0;
	m_descriptor_length = 0;
}

TS_BOOL tssi::Descriptor::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (in_len < 2) return TS_FALSE;

	m_descriptor_tag = DESC_TAG(pbuf);
	m_descriptor_length = DESC_LENGTH(pbuf);

	return TS_TRUE;
}

TS_BYTE tssi::Descriptor::GetDescriptorTag() const
{
	return m_descriptor_tag;
}

TS_BYTE tssi::Descriptor::GetDescriptorLength() const
{
	return m_descriptor_length;
}
