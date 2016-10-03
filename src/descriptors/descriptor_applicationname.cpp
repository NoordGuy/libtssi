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

#include "descriptors/descriptor_applicationname.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_ApplicationName::Descriptor_ApplicationName() : Descriptor()
{
}

tssi::Descriptor_ApplicationName::~Descriptor_ApplicationName()
{
	Descriptor::~Descriptor();
	m_app_name.clear();
}

TS_VOID tssi::Descriptor_ApplicationName::Reset()
{
	Descriptor::Reset();
	m_app_name.clear();
}

TS_BOOL tssi::Descriptor_ApplicationName::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	m_app_name.clear();

	pbuf += 2;

	int res = in_len;

	while (res > 0)
	{
		AitName name;
		name.iso_639_language_code = DESC_ISO639_LANGUAGE_CODE(pbuf);
		TS_BYTE tlen = BYTE_VALUE(pbuf, 3);
		pbuf += 4;
		res -= 4;

		name.application_name = ConvertBufferToString(pbuf, tlen);
		m_app_name.push_back(name);
		
		pbuf += tlen;
		res -= tlen;
	}

	return TS_TRUE;
}

unsigned tssi::Descriptor_ApplicationName::GetApplicationNameListLength() const
{
	return static_cast<unsigned> ( m_app_name.size() );
}

const TS_STRING& tssi::Descriptor_ApplicationName::GetApplicationName(unsigned index) const
{
	if (index >= m_app_name.size()) return m_missing_string;
	return m_app_name[index].application_name;
}

TS_DWORD tssi::Descriptor_ApplicationName::GetApplicationNameLanguage(unsigned index) const
{
	if (index >= m_app_name.size()) return 0;
	return m_app_name[index].iso_639_language_code;
}

