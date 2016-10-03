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

#include "descriptors/descriptor_application.h"
#include "utils/parsemac2.h"

tssi::Descriptor_Application::Descriptor_Application() : Descriptor()
{
	m_service_bound = 0;
	m_visibility = 0;
	m_application_priority = 0;

}

tssi::Descriptor_Application::~Descriptor_Application()
{
	Descriptor::~Descriptor();
	m_app_version.clear();

}

TS_VOID tssi::Descriptor_Application::Reset()
{
	Descriptor::Reset();
	m_app_version.clear();
	m_service_bound = 0;
	m_visibility = 0;
	m_application_priority = 0;
}

TS_BOOL tssi::Descriptor_Application::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	m_app_version.clear();

	TS_BYTE prof_len = BYTE_VALUE(pbuf, 2);
	unsigned x;
	pbuf += 3;
	for (x = 0; x < (prof_len / static_cast<unsigned> (5)); ++x) {
		AppVersion version;
		version.application_profile = NTOH_S(WORD_VALUE(pbuf,0));
		version.version_major = BYTE_VALUE(pbuf,2);
		version.version_minor = BYTE_VALUE(pbuf,3);
		version.version_micro = BYTE_VALUE(pbuf,4);
		m_app_version.push_back(version);

		pbuf += 5;
	}
	
	m_service_bound = BYTE_VALUE(pbuf,0) >> 7;
	m_visibility = (BYTE_VALUE(pbuf,0) >> 5) & 0x03;
	m_application_priority = BYTE_VALUE(pbuf,1);

	return TS_TRUE;
}

unsigned tssi::Descriptor_Application::GetVersionListLength() const
{
	return static_cast<unsigned> ( m_app_version.size() );
}

TS_WORD tssi::Descriptor_Application::GetApplicationProfile(unsigned index) const
{
	if (index >= m_app_version.size() ) return 0;
	return m_app_version[index].application_profile;
}

TS_BYTE tssi::Descriptor_Application::GetVersionMajor(unsigned index) const
{
	if (index >= m_app_version.size() ) return 0;
	return m_app_version[index].version_major;
}

TS_BYTE tssi::Descriptor_Application::GetVersionMinor(unsigned index) const
{
	if (index >= m_app_version.size() ) return 0;
	return m_app_version[index].version_minor;
}

TS_BYTE tssi::Descriptor_Application::GetVersionMicro(unsigned index) const
{
	if (index >= m_app_version.size() ) return 0;
	return m_app_version[index].version_micro;
}

TS_BOOL tssi::Descriptor_Application::GetServiceBound() const
{
	return m_service_bound;
}

TS_BYTE tssi::Descriptor_Application::GetVisibility() const
{
	return m_visibility;
}

TS_BYTE tssi::Descriptor_Application::GetApplicationPriority() const
{
	return m_application_priority;
}
