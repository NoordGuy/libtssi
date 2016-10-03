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

#include "descriptors/descriptor_dvbjapplicationlocation.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_DvbJApplicationLocation::Descriptor_DvbJApplicationLocation() : Descriptor()
{
}

tssi::Descriptor_DvbJApplicationLocation::~Descriptor_DvbJApplicationLocation()
{
	Descriptor::~Descriptor();
}

TS_VOID tssi::Descriptor_DvbJApplicationLocation::Reset()
{
	Descriptor::Reset();
	m_base_directory.clear();
	m_class_extension.clear();
	m_initial_class.clear();
}

TS_BOOL	tssi::Descriptor_DvbJApplicationLocation::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	unsigned residual_size = m_descriptor_length;

	pbuf += 2;

	unsigned plen = BYTE_VALUE(pbuf,0);
	++pbuf; --residual_size;

	m_base_directory = ConvertBufferToString(pbuf, plen);

	pbuf += plen;
	residual_size -= plen;
	plen = BYTE_VALUE(pbuf,0);
	++pbuf; --residual_size;

	m_class_extension = ConvertBufferToString(pbuf, plen);
	
	pbuf += plen;
	residual_size -= plen;
	plen = residual_size;

	m_initial_class = ConvertBufferToString(pbuf, plen);

	return TS_TRUE;
}

const TS_STRING& tssi::Descriptor_DvbJApplicationLocation::GetBaseDirectory() const
{
	return m_base_directory;
}

const TS_STRING& tssi::Descriptor_DvbJApplicationLocation::GetClasspathExtension() const
{
	return m_class_extension;
}

const TS_STRING& tssi::Descriptor_DvbJApplicationLocation::GetInitialClass() const
{
	return m_initial_class;
}
