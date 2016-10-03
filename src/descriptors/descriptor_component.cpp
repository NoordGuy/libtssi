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

#include "descriptors/descriptor_component.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_Component::Descriptor_Component() : Descriptor()
{
	m_text.clear();
	m_stream_content = 0;
	m_component_type = 0;
	m_component_tag = 0;
	m_language_code = 0;
}

tssi::Descriptor_Component::~Descriptor_Component()
{
	Descriptor::~Descriptor();
	m_text.clear();
}

TS_VOID tssi::Descriptor_Component::Reset()
{
	Descriptor::Reset();
	m_text.clear();
	m_stream_content = 0;
	m_component_type = 0;
	m_component_tag = 0;
	m_language_code = 0;
}

TS_BOOL	tssi::Descriptor_Component::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	if (m_descriptor_length < 6) return TS_FALSE;
	pbuf += 2;

	m_stream_content = BYTE_VALUE(pbuf,0) & 0xF; ++pbuf;
	m_component_type = BYTE_VALUE(pbuf,0); ++pbuf;
	m_component_tag = BYTE_VALUE(pbuf,0); ++pbuf;
	m_language_code = (NTOH_L(DWORD_VALUE(pbuf,0)) >> 8); pbuf += 3;
	m_text = ConvertBufferToString(pbuf, m_descriptor_length - 6);
	
	return TS_TRUE;
}

const TS_STRING& tssi::Descriptor_Component::GetText() const
{
	return m_text;
}

TS_BYTE tssi::Descriptor_Component::GetStreamContent() const
{
	return m_stream_content;
}

TS_BYTE tssi::Descriptor_Component::GetComponentType() const
{
	return m_component_type;
}

TS_BYTE tssi::Descriptor_Component::GetComponentTag() const
{
	return m_component_tag;
}

TS_DWORD tssi::Descriptor_Component::GetLanguageCode() const
{
	return m_language_code;
}
