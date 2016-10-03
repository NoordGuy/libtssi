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

#include "descriptors/descriptor_service.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_Service::Descriptor_Service() : Descriptor() 
{
	m_service_type = 0;
}

tssi::Descriptor_Service::~Descriptor_Service() 
{
	Descriptor::~Descriptor();
	m_provider_name.clear();
	m_service_name.clear();
}

TS_VOID tssi::Descriptor_Service::Reset()
{
	Descriptor::Reset();
	m_provider_name.clear();
	m_service_name.clear();
	m_service_type = 0;
}

TS_BOOL	tssi::Descriptor_Service::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	m_service_type = DESC_SERVICE_TYPE(pbuf);

	unsigned prv_length  = DESC_SERVICE_PROVIDER_NAME_LENGTH(pbuf);
	unsigned name_length = DESC_SERVICE_NAME_LENGTH(pbuf);

	pbuf += 4;

	m_provider_name = ConvertBufferToString(pbuf, prv_length);

	pbuf += prv_length + 1;

	m_service_name = ConvertBufferToString(pbuf, name_length);
		
	return TS_TRUE;
}

TS_BYTE tssi::Descriptor_Service::GetServiceType() const
{
	return m_service_type;
}

const TS_STRING& tssi::Descriptor_Service::GetProviderName() const
{
	return m_provider_name;
}

const TS_STRING& tssi::Descriptor_Service::GetServiceName() const
{
	return m_service_name;
}

