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

#include "descriptors/descriptor_shortevent.h"
#include "utils/parsemac2.h"
#include "utils/tsstring.h"

tssi::Descriptor_ShortEvent::Descriptor_ShortEvent() : Descriptor() 
{
	m_event_language = 0;
}

tssi::Descriptor_ShortEvent::~Descriptor_ShortEvent()
{
	Descriptor::~Descriptor();
	m_event_name.clear();
	m_event_text.clear();
}

TS_VOID tssi::Descriptor_ShortEvent::Reset()
{
	Descriptor::Reset();
	m_event_name.clear();
	m_event_text.clear();
	m_event_language = 0;
}

TS_BOOL	tssi::Descriptor_ShortEvent::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;
	pbuf += 2;

	m_event_language = (NTOH_L(DWORD_VALUE(pbuf,0)) >> 8);
	pbuf += 3;

	TS_BYTE name_length  = BYTE_VALUE(pbuf, 0);
	++pbuf;
	m_event_name = ConvertBufferToString(pbuf, name_length);
	pbuf += name_length;

	TS_BYTE text_length  = BYTE_VALUE(pbuf, 0);
	++pbuf;
	m_event_text = ConvertBufferToString(pbuf, text_length);
		
	return TS_TRUE;
}

const TS_STRING& tssi::Descriptor_ShortEvent::GetEventName() const
{
	return m_event_name;
}

const TS_STRING& tssi::Descriptor_ShortEvent::GetEventText() const
{
	return m_event_text;
}