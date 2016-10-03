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

#include "descriptors/descriptor_dvbjapplication.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_DvbJApplication::Descriptor_DvbJApplication() : Descriptor()
{
	m_parameter.clear();
}

tssi::Descriptor_DvbJApplication::~Descriptor_DvbJApplication()
{
	Descriptor::~Descriptor();
	m_parameter.clear();
}

TS_VOID tssi::Descriptor_DvbJApplication::Reset()
{
	Descriptor::Reset();
	m_parameter.clear();
}

TS_BOOL	tssi::Descriptor_DvbJApplication::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	m_parameter.clear();

	unsigned residual_size = m_descriptor_length;

	pbuf += 2;

	while (residual_size > 0) {
		unsigned plen = BYTE_VALUE(pbuf,0);
		++pbuf;
		--residual_size;

		m_parameter.push_back( ConvertBufferToString(pbuf, plen) );
		
		pbuf += plen;
		residual_size -= plen;
	}

	return TS_TRUE;
}

unsigned tssi::Descriptor_DvbJApplication::GetParameterListLength() const
{
	return static_cast<unsigned> ( m_parameter.size() );
}

const TS_STRING& tssi::Descriptor_DvbJApplication::GetParameter(unsigned index) const
{
	if (index >= GetParameterListLength() ) return m_missing_string;
	return m_parameter[index];
}
