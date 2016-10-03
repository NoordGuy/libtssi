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

#include "descriptors/descriptor_networkname.h"
#include "utils/tsstring.h"

tssi::Descriptor_NetworkName::Descriptor_NetworkName() : Descriptor() 
{
}

tssi::Descriptor_NetworkName::~Descriptor_NetworkName()
{
	Descriptor::~Descriptor();
	m_network_name.clear();
}

TS_VOID tssi::Descriptor_NetworkName::Reset()
{
	Descriptor::Reset();
	m_network_name.clear();
}

TS_BOOL	tssi::Descriptor_NetworkName::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	pbuf += 2;
	m_network_name = ConvertBufferToString(pbuf, m_descriptor_length);

	return TS_TRUE;
}

const TS_STRING& tssi::Descriptor_NetworkName::GetNetworkName() const
{
	return m_network_name;
}