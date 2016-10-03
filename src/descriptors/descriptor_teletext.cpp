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

#include "descriptors/descriptor_teletext.h"
#include "utils/parsemac2.h"

tssi::Descriptor_Teletext::Descriptor_Teletext() : Descriptor() 
{
}

tssi::Descriptor_Teletext::~Descriptor_Teletext() 
{
	Descriptor::~Descriptor();
	m_teletext.clear();
}

TS_VOID tssi::Descriptor_Teletext::Reset()
{
	Descriptor::Reset();
	m_teletext.clear();
}

TS_BOOL	tssi::Descriptor_Teletext::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	m_teletext.clear();

	TS_BYTE residual_length = m_descriptor_length;

	pbuf += 2;

	while (residual_length > 0) {
		
		TeletextInformation i;

		i.language = DESC_TELETEXT_LANGUAGE_CODE(pbuf);
		i.type = DESC_TELETEXT_AUDIO_TYPE(pbuf);
		i.magazine_number = DESC_TELETEXT_MAGAZINE_NUMBER(pbuf);
		i.page_number = DESC_TELETEXT_PAGE_NUMBER(pbuf);

		m_teletext.push_back(i);

		residual_length -= 5;
		pbuf += 5;
	}

	return TS_TRUE;
}

unsigned tssi::Descriptor_Teletext::GetTeletextListLength() const
{
	return static_cast<unsigned> ( m_teletext.size() );
}

TS_DWORD tssi::Descriptor_Teletext::GetTeletextLanguage(unsigned index) const
{
	if (index >= GetTeletextListLength()) return 0;
	return m_teletext[index].language;
}

TS_BYTE tssi::Descriptor_Teletext::GetTeletextType(unsigned index) const
{
	if (index >= GetTeletextListLength()) return 0;
	return m_teletext[index].type;
}

TS_BYTE tssi::Descriptor_Teletext::GetTeletextMagazineNumber(unsigned index) const
{
	if (index >= GetTeletextListLength()) return 0;
	return m_teletext[index].magazine_number;
}

TS_BYTE tssi::Descriptor_Teletext::GetTeletextPageNumber(unsigned index) const
{
	if (index >= GetTeletextListLength()) return 0;
	return m_teletext[index].page_number;
}