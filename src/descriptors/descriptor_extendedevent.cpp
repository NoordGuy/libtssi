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

#include "descriptors/descriptor_extendedevent.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_ExtendedEvent::Descriptor_ExtendedEvent() : Descriptor()
{
	m_event_language = 0;
	m_descriptor_number = 0;
	m_last_descriptor_number = 0;
}

tssi::Descriptor_ExtendedEvent::~Descriptor_ExtendedEvent()
{
	Descriptor::~Descriptor();
	m_event_desc.clear();
}

TS_VOID tssi::Descriptor_ExtendedEvent::Reset()
{
	Descriptor::Reset();
	m_event_language = 0;
	m_event_desc.clear();
	m_descriptor_number = 0;
	m_last_descriptor_number = 0;
}

TS_BOOL	tssi::Descriptor_ExtendedEvent::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	pbuf += 2;
		
	m_descriptor_number = (BYTE_VALUE(pbuf,0) >> 4) & 0xf;
	m_last_descriptor_number = BYTE_VALUE(pbuf,0) & 0xf;
	++pbuf;
	
	m_event_language = (NTOH_L(DWORD_VALUE(pbuf,0)) >> 8);
	pbuf += 3;

	TS_BYTE loi = BYTE_VALUE(pbuf,0);
	++pbuf;
	pbuf += loi;

	TS_BYTE textlen = BYTE_VALUE(pbuf,0);
	++pbuf;

	m_event_desc = ConvertBufferToString(pbuf, textlen);

	return TS_FALSE;
}

TS_DWORD tssi::Descriptor_ExtendedEvent::GetEventLanguage() const
{
	return m_event_language;
}

const TS_STRING& tssi::Descriptor_ExtendedEvent::GetEventDescripton() const
{
	return m_event_desc;
}

TS_BYTE tssi::Descriptor_ExtendedEvent::GetDescriptorNumber() const
{
	return m_descriptor_number;
}

TS_BYTE tssi::Descriptor_ExtendedEvent::GetLastDescriptorNumber() const
{
	return m_last_descriptor_number;
}
