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

#include "descriptors/descriptor_iso639.h"
#include "utils/parsemac2.h"

tssi::Descriptor_Iso639::Descriptor_Iso639() : Descriptor() 
{
}

tssi::Descriptor_Iso639::~Descriptor_Iso639() 
{
	Descriptor::~Descriptor();
	m_language_codes.clear();
	m_language_types.clear();
}

TS_VOID tssi::Descriptor_Iso639::Reset()
{
	Descriptor::Reset();
	m_language_codes.clear();
	m_language_types.clear();
}

TS_BOOL	tssi::Descriptor_Iso639::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	m_language_codes.clear();
	m_language_types.clear();

	unsigned residual_length = m_descriptor_length;

	pbuf += 2;

	while (residual_length > 0) {
		
		m_language_codes.push_back( DESC_ISO639_LANGUAGE_CODE(pbuf) );
		m_language_types.push_back( DESC_ISO639_AUDIO_TYPE(pbuf) );

		residual_length -= 4;
		pbuf += 4;
	}

	return TS_TRUE;
}

unsigned tssi::Descriptor_Iso639::GetLanguageListLength() const
{
	return static_cast<unsigned> ( m_language_codes.size() );
}

TS_DWORD tssi::Descriptor_Iso639::GetLanguageCode(unsigned index) const
{
	if ( index >= m_language_codes.size() ) return 0;
	return m_language_codes[index];
}

TS_BYTE tssi::Descriptor_Iso639::GetLanguageType(unsigned index) const
{
	if ( index >= m_language_types.size() ) return 0;
	return m_language_types[index];
}

