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

#include "descriptors/descriptor_applicationsignalling.h"
#include "utils/parsemac2.h"

tssi::Descriptor_ApplicationSignalling::Descriptor_ApplicationSignalling() : Descriptor()
{
	m_app_type.clear();
	m_ait_version.clear();
	m_info_count = 0;
}

tssi::Descriptor_ApplicationSignalling::~Descriptor_ApplicationSignalling()
{
	Descriptor::~Descriptor();
	m_app_type.clear();
	m_ait_version.clear();	
}

TS_VOID tssi::Descriptor_ApplicationSignalling::Reset()
{
	Descriptor::Reset();
	m_app_type.clear();
	m_ait_version.clear();
	m_info_count = 0;
}

TS_BOOL tssi::Descriptor_ApplicationSignalling::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	m_app_type.clear();
	m_ait_version.clear();
	
	m_info_count = m_descriptor_length / 3;

	pbuf += 2;

	unsigned x;
	for (x = 0; x < m_info_count; ++x) {
		TS_WORD type = HTON_S(WORD_VALUE(pbuf,0));
		TS_BYTE ver = BYTE_VALUE(pbuf,2) & 0x1f;
		m_app_type.push_back(type);
		m_ait_version.push_back(ver);
		pbuf += 3;
	}


	return TS_TRUE;
}

unsigned tssi::Descriptor_ApplicationSignalling::GetListLength() const
{
	return static_cast<unsigned> ( m_app_type.size() );
}

TS_WORD tssi::Descriptor_ApplicationSignalling::GetApplicationType(unsigned index) const
{
	if ( index >= m_app_type.size() ) return 0;
	return m_app_type[index];
}

TS_BYTE tssi::Descriptor_ApplicationSignalling::GetAitVersion(unsigned index) const
{
	if ( index >= m_ait_version.size() ) return 0;
	return m_ait_version[index];
}

