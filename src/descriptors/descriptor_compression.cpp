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

#include "descriptors/descriptor_compression.h"
#include "utils/parsemac2.h"

tssi::Descriptor_Compression::Descriptor_Compression() : Descriptor() 
{
	m_compression = 0;
	m_original_size = 0;
}

tssi::Descriptor_Compression::~Descriptor_Compression()
{
	Descriptor::~Descriptor();
}

TS_VOID tssi::Descriptor_Compression::Reset()
{
	Descriptor::Reset();
	m_compression = 0;
	m_original_size = 0;
}

TS_BOOL	tssi::Descriptor_Compression::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf, in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	
	m_compression = BYTE_VALUE(pbuf, 2);
	m_original_size = NTOH_L(DWORD_VALUE(pbuf, 3));

	return TS_TRUE;
}

TS_BYTE tssi::Descriptor_Compression::GetCompressionSystem() const
{
	return m_compression;
}

TS_DWORD tssi::Descriptor_Compression::GetOriginalSize() const
{
	return m_original_size;
}

