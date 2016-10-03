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

#include "dsm-cc/biopmessage_file.h"
#include "utils/parsemac2.h"

tssi::BiopMessage_File::BiopMessage_File() : BiopMessage()
{
	m_content_length = 0;
	m_content_data = 0;
}

tssi::BiopMessage_File::~BiopMessage_File()
{
	BiopMessage::~BiopMessage();
	if (m_content_data) delete m_content_data;
}

tssi::BiopMessage_File::BiopMessage_File(const tssi::BiopMessage_File&)
{
}

tssi::BiopMessage_File& tssi::BiopMessage_File::operator=(const tssi::BiopMessage_File&)
{
	return *this;
}

TS_VOID tssi::BiopMessage_File::Reset()
{
	BiopMessage::Reset();

	if (m_content_data) delete m_content_data;
	m_content_length = 0;
	m_content_length = 0;	
}

TS_BOOL	tssi::BiopMessage_File::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!BiopMessage::Process(pbuf, in_len)) return TS_FALSE;

	pbuf += 23 + m_object_key_length;
	
	if (m_object_info_length > 0)
	{
		// ToDo: parse object info data
		pbuf += m_object_info_length;
	}

	TS_BYTE service_context_list_count = BYTE_VALUE(pbuf, 0);
	++pbuf;

	while (service_context_list_count > 0)
	{
		TS_WORD entry_length = NTOH_S(WORD_VALUE(pbuf,4)) + 6;

		--service_context_list_count;
		pbuf += entry_length;
	}

	m_content_length = NTOH_L(DWORD_VALUE(pbuf, 4));

	pbuf += 8;

	if (m_content_length)
	{
		m_content_data = new TS_BYTE[m_content_length];
		memcpy( m_content_data, pbuf, m_content_length );
	}

	return TS_TRUE;
}

TS_DWORD tssi::BiopMessage_File::GetContentLength() const
{
	return m_content_length;
}

TS_PBYTE tssi::BiopMessage_File::GetContentData() const
{
	return m_content_data;
}
