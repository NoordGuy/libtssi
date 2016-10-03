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

#include "tables/table_sdt.h"
#include "utils/parsemac2.h"

tssi::Table_Sdt::Table_Sdt() : Table()
{
	m_missing_service.transport_stream_id = 0xffff;
	m_missing_service.running_status = 0xff;
}

tssi::Table_Sdt::~Table_Sdt()
{
	Table::~Table();
}

TS_VOID tssi::Table_Sdt::Reset()
{
	Table::Reset();

	m_sdt_data.clear();
}

TS_BOOL	tssi::Table_Sdt::Process(TS_PBYTE pbuf, unsigned in_len)
{
	TS_BOOL new_version = TS_FALSE;
	if (!Table::Process(pbuf, in_len, TS_TRUE, new_version)) 
		return TS_FALSE;
	if (!new_version)
		return TS_TRUE;
	if ((m_table_id != 0x42) || (m_table_id > 0x46))
		return TS_FALSE;

	TS_WORD transport_stream_id = SDT_TRANSPORT_STREAM_ID_VALUE(pbuf);
	TS_WORD original_network_id = SDT_ORIGINAL_NETWORK_ID_VALUE(pbuf);

	unsigned residual_length = m_section_length - 4 /*CRC32*/ - 8;
	pbuf += 8 + 3;

	while (residual_length > 0) {

		ServiceDescription service;
		service.transport_stream_id = transport_stream_id;
		service.original_network_id = original_network_id;
		service.service_id = SDT_DESCRIPTON_RECORD_SERVICE_ID_VALUE(pbuf);
		service.eit_schedule_flag = SDT_DESCRIPTON_RECORD_EIT_SCHEDULE_BIT(pbuf);
		service.eit_present_following_flag = SDT_DESCRIPTON_RECORD_EIT_P_F_BIT(pbuf);
		service.running_status = SDT_DESCRIPTON_RECORD_RUNNING_STATUS_VALUE(pbuf);
		service.free_ca_mode = SDT_DESCRIPTON_RECORD_FREE_CA_BIT(pbuf);

		unsigned desc_length = SDT_DESCRIPTON_RECORD_SERVICE_INFO_LENGTH(pbuf);

		pbuf += 5;		
		residual_length -= 5;

		if (! service.descriptor_list.Process(pbuf, desc_length) )
			return TS_FALSE;

		pbuf += desc_length;
		residual_length -= desc_length;

		// is a similar item stored?
		unsigned x;
		for (x = 0; x < m_sdt_data.size(); ++x) {
			if ((m_sdt_data[x].service_id == service.service_id) &&
				(m_sdt_data[x].transport_stream_id == service.transport_stream_id) &&
				(m_sdt_data[x].original_network_id == service.original_network_id)) {
					m_sdt_data[x] = service;
					break;
			}
		}

		// not found? add
		if (x == m_sdt_data.size()) {
			m_sdt_data.push_back(service);		
		}

	}

	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
}

unsigned tssi::Table_Sdt::GetServiceListLength() const
{
	return static_cast<unsigned> ( m_sdt_data.size() );
}

const tssi::ServiceDescription& tssi::Table_Sdt::GetServiceDescription(unsigned index) const
{
	if (index >= GetServiceListLength()) return m_missing_service;
	return m_sdt_data[index];
}
