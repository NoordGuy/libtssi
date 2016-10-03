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

#include "dsm-cc/biopmessage.h"
#include "utils/parsemac2.h"

tssi::BiopMessage::BiopMessage()
{
	m_magic_code = 0;
	m_major_version = 0;
	m_minor_version = 0;
	m_message_type = 0;
	m_message_size = 0;
	m_object_key_length = 0;
	m_object_key = 0;
	m_object_kind = 0;
	m_object_info_length = 0;

	m_total_length = 0;
}

tssi::BiopMessage::~BiopMessage()
{
}

TS_VOID tssi::BiopMessage::Reset()
{
	m_magic_code = 0;
	m_major_version = 0;
	m_minor_version = 0;
	m_message_type = 0;
	m_message_size = 0;
	m_object_key_length = 0;
	m_object_key = 0;
	m_object_kind = 0;
	m_object_info_length = 0;

	m_total_length = 0;
}

TS_BOOL tssi::BiopMessage::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (in_len < 13)
		return TS_FALSE;

	m_magic_code = NTOH_L(DWORD_VALUE(pbuf,0));

	if (m_magic_code != kBiopMagic) return TS_FALSE;

	m_major_version = BYTE_VALUE(pbuf,4);
	m_minor_version = BYTE_VALUE(pbuf,5);
	m_message_type = BYTE_VALUE(pbuf,7);
	m_message_size = NTOH_L(DWORD_VALUE(pbuf,8));

	m_total_length = m_message_size + 12;

	m_object_key_length = BYTE_VALUE(pbuf,12);

	if (in_len < m_total_length)
		return TS_FALSE;

	pbuf += 13;

	if (m_object_key_length > 0) {
		m_object_key = NTOH_L(DWORD_VALUE(pbuf, 0));
		if (m_object_key_length == 1)
			m_object_key = m_object_key & 0x000000ff;
		else if (m_object_key_length == 2)
			m_object_key = m_object_key & 0x0000ffff;
		else if (m_object_key_length == 3)
			m_object_key = m_object_key & 0x00ffffff;
		else // dvb allows up to 4 bytes
			m_object_key = m_object_key;

		pbuf += m_object_key_length;
	} else
		return TS_FALSE;

	TS_DWORD object_kind_length = NTOH_L(DWORD_VALUE(pbuf,0));
	if (object_kind_length != 4)
		return TS_FALSE;

	m_object_kind = NTOH_L(DWORD_VALUE(pbuf,4));

	m_object_info_length = NTOH_S(WORD_VALUE(pbuf,8));

	return TS_TRUE;
}

TS_BYTE tssi::BiopMessage::GetMessageType() const
{
	return m_message_type;
}

TS_DWORD tssi::BiopMessage::GetMessageSize() const
{
	return m_message_size;
}

TS_DWORD tssi::BiopMessage::GetObjectKey() const
{
	return m_object_key;
}

TS_DWORD tssi::BiopMessage::GetObjectKind() const
{
	return m_object_kind;
}

TS_WORD tssi::BiopMessage::GetObjectInfoLength() const
{
	return m_object_info_length;
}

TS_DWORD tssi::BiopMessage::GetTotalLength() const
{
	return m_total_length;
}

