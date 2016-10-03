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

#include "packets/packet.h"
#include "utils/parsemac2.h"

namespace {
	const TS_WORD kMaxTableLength = 4096;
}

tssi::Packet::Packet()
{
	m_table = 0;
	m_cache = new TS_BYTE[kMaxTableLength];
	m_cache_length = 0;
	m_cache_pos = 0;	
}

tssi::Packet::~Packet()
{
	delete[] m_cache; 
	m_cache = 0;
}

TS_VOID tssi::Packet::Reset()
{
	ResetTable();
	m_cache_length = 0;
	m_cache_pos = 0;
}

tssi::Table* tssi::Packet::GetTable() const
{
	return m_table;
}

TS_BOOL	tssi::Packet::Process(TS_PBYTE pbuf, unsigned in_len)
{
	unsigned packet_bytes_todo = in_len;

	// Is there a payload??? -> adaptation_field_control
	TS_BYTE adaptation_field_control = ADAPTATION_FIELD_CONTROL_VALUE(pbuf);
	if ((adaptation_field_control==0x00) || (adaptation_field_control==0x02)) return TS_TRUE; // no payload

	packet_bytes_todo -= 4;

	TS_PBYTE payload = &pbuf[4];
	// is there an adaptation field?
	if (adaptation_field_control == 0x03) {
		TS_BYTE adaptation_field_length = pbuf[4];
		payload += 5 + adaptation_field_length;
		packet_bytes_todo -= adaptation_field_length + 1;
	}

	// pointer_field and table start?
	TS_BOOL payload_unit_start_indicator = PAYLOAD_UNIT_START_INDICATOR_BIT(pbuf);
	TS_BYTE pointer_field = 0;

	if (payload_unit_start_indicator == TS_TRUE) {
		pointer_field = payload[0];
		--packet_bytes_todo;
		++payload;
	}

	TS_BOOL result_value = TS_TRUE;

	if (((payload_unit_start_indicator == TS_FALSE) || (pointer_field > 0)) && (m_cache_length - m_cache_pos > 0)) {

		TS_BYTE old_size = 0;
		if (pointer_field > 0) old_size = pointer_field;		//payload[0] -> payload[pointer_field]
		else old_size = packet_bytes_todo;						//payload[0] -> payload[packet_length]

		TS_WORD copysize = old_size;
		if (m_cache_length - m_cache_pos < copysize) copysize = m_cache_length - m_cache_pos;

		// copy chunk
		if (m_cache_pos >= m_cache_length) {
			m_cache_pos = 0;
			m_cache_length = 0;
			return TS_FALSE;
		}
		if (copysize + m_cache_pos > kMaxTableLength) {
			m_cache_pos = 0;
			m_cache_length = 0;			
			return TS_FALSE; 
		}
		memcpy(m_cache + m_cache_pos, payload, copysize);

		m_cache_pos += copysize;

		if (m_cache_pos == m_cache_length) {
			// process table
			result_value = m_table->Process(m_cache, m_cache_length );
	
			m_cache_pos = 0;
			m_cache_length = 0;

		}

	}

	payload += pointer_field;
	packet_bytes_todo -= pointer_field;

	if (payload_unit_start_indicator == TS_TRUE) {
		
		while (packet_bytes_todo > 0) {
			TS_BYTE table_id = 0;
			table_id = PSI_TABLE_ID_VALUE(payload);
			
			if (table_id==0xFF) {
				packet_bytes_todo = 0;
				break;
			}

			TS_WORD private_section_length = PSI_SECTION_LENGTH_VALUE(payload);

			m_cache_length = private_section_length + 3;
			m_cache_pos = 0;
			
			TS_WORD copysize = packet_bytes_todo;
			if (m_cache_length < copysize) copysize = m_cache_length;

			memcpy(m_cache, payload, copysize);

			packet_bytes_todo -= copysize;
			m_cache_pos += copysize;

			if (m_cache_pos == m_cache_length) {
				// process table

				// multiply errors
				result_value *= m_table->Process(m_cache, m_cache_length);

				m_cache_pos = 0;
				m_cache_length = 0;
			}
			payload += copysize;
			

		}
	}

	return result_value;

}
