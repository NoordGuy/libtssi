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

#include "descriptors/descriptor_pdc.h"
#include "utils/parsemac2.h"

tssi::Descriptor_Pdc::Descriptor_Pdc() : Descriptor()
{
	m_pil = 0;
}

tssi::Descriptor_Pdc::~Descriptor_Pdc()
{
	Descriptor::~Descriptor();
}

TS_VOID tssi::Descriptor_Pdc::Reset()
{
	Descriptor::Reset();
	m_pil = 0;
}

TS_BOOL	tssi::Descriptor_Pdc::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	
	m_pil = (NTOH_L(DWORD_VALUE(pbuf, 1))) & 0xFFFFF;

	return TS_TRUE;
}

TS_DWORD tssi::Descriptor_Pdc::GetPil() const
{
	return m_pil;
}