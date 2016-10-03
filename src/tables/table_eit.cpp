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

#include "tables/table_eit.h"
#include "utils/parsemac2.h"

tssi::Table_Eit::Table_Eit() : Table()
{
	m_missing_event.transport_stream_id = 0xffff;
	m_missing_event.start_time = 0;
}

tssi::Table_Eit::~Table_Eit() 
{
	Table::~Table();
}

TS_VOID tssi::Table_Eit::Reset()
{
	Table::Reset();

	m_eit_data.clear();	
}

TS_BOOL	tssi::Table_Eit::Process(TS_PBYTE pbuf, unsigned in_len)
{
	TS_BOOL new_version = TS_FALSE;
	if (!Table::Process(pbuf, in_len, TS_TRUE, new_version)) 
		return TS_FALSE;
	if (!new_version)
		return TS_TRUE;
	if ((m_table_id < 0x4e) || (m_table_id > 0x6f))
		return TS_FALSE;

	TS_WORD transport_stream_id = EIT_TRANSPORT_STREAM_ID_VALUE(pbuf);
	TS_WORD original_network_id = EIT_ORIGINAL_NETWORK_ID_VALUE(pbuf);
	TS_WORD service_id = EIT_SERVICE_ID_VALUE(pbuf);

	unsigned residual_length = in_len - 14;
	pbuf += 14;
	
	while (residual_length > 4) {	 // crc32

		EitEvent eit_event;

		eit_event.transport_stream_id = transport_stream_id;
		eit_event.original_network_id = original_network_id;
		eit_event.service_id = service_id;
		eit_event.event_id = EIT_EVENT_ID_VALUE(pbuf);
		eit_event.start_time = EIT_START_TIME_VALUE(pbuf);
		eit_event.duration = static_cast<TS_DWORD> ( EIT_DURATION_VALUE(pbuf) );
		eit_event.running_status = EIT_RUNNING_STATUS_VALUE(pbuf);
		eit_event.free_ca_mode = EIT_FREE_CA_MODE_BIT(pbuf);

		unsigned desc_length = EIT_DESCRIPTORS_LOOP_LENGTH_VALUE(pbuf);

		pbuf += 12;
		residual_length -= 12;

		if (! eit_event.descriptor_list.Process(pbuf, desc_length) )
			return TS_FALSE;

		pbuf += desc_length;
		residual_length -= desc_length;

		// is a similar item stored?
		unsigned x;
		for (x = 0; x < m_eit_data.size(); ++x) {
			if ((m_eit_data[x].event_id == eit_event.event_id) &&
				(m_eit_data[x].service_id == eit_event.service_id) &&
				(m_eit_data[x].transport_stream_id == eit_event.transport_stream_id) &&
				(m_eit_data[x].original_network_id == eit_event.original_network_id)) {
					m_eit_data[x] = eit_event;
					break;
			}
		}

		// not found? add
		if (x == m_eit_data.size())
			m_eit_data.push_back(eit_event);
		


	}
	
	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
}

unsigned tssi::Table_Eit::GetEventListLength() const
{
	return static_cast<unsigned> ( m_eit_data.size() );
}

const tssi::EitEvent& tssi::Table_Eit::GetEvent(unsigned index) const
{
	if (index >= GetEventListLength())
		return m_missing_event;
	return m_eit_data[index];
}