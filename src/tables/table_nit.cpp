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

#include "tables/table_nit.h"
#include "utils/parsemac2.h"

tssi::Table_Nit::Table_Nit() : Table()
{
	m_network_id = 0;
	m_version_number = 0xff;
}

tssi::Table_Nit::~Table_Nit()
{
	Table::~Table();
}

TS_VOID tssi::Table_Nit::Reset()
{
	Table::Reset();
	m_network_id = 0;
	m_descriptor_list.Reset();
	m_network_list.clear();
	m_version_number = 0xff;

}

TS_BOOL	tssi::Table_Nit::Process(TS_PBYTE pbuf, unsigned in_len)
{
	TS_BOOL new_version = TS_FALSE;
	if (!Table::Process(pbuf, in_len, TS_FALSE, new_version)) 
		return TS_FALSE;
	if (!new_version)
		return TS_TRUE;
	if (m_table_id != 0x40) 
		return TS_FALSE;
	// rare cases of table_id 0x41 (other network information) are omitted 
	// and would require small interface adjustments of Table_Nit
	// table_id 0x40 -> network_id is constant

	TS_BYTE version_number = NIT_VERSION_NUMBER_VALUE(pbuf);
	if (m_version_number != version_number) {
		// new version, new descriptors
		m_descriptor_list.Reset();
		m_network_list.clear();
		m_version_number = version_number;
	}

	m_network_id = NIT_NETWORK_ID_VALUE(pbuf);
	unsigned desc_length = NIT_NETWORK_DESCRIPTORS_LENGTH(pbuf);
	pbuf += 10;
	m_descriptor_list.Process(pbuf, desc_length);
	pbuf += desc_length;

	unsigned loop_length = NIT_TRANSPORT_STREAM_LOOP_LENGTH(pbuf);
	pbuf += 2;

	while ( loop_length > 0 ) {
		NetworkInformation information;
		information.transport_stream_id = NIT_TRANSPORT_STREAM_ID(pbuf);
		information.original_network_id = NIT_ORIGINAL_NETWORD_ID(pbuf);
		desc_length = NIT_TRANSPORT_DESCRIPTORS_LENGTH(pbuf);

		pbuf += 6;
		loop_length -= 6;		

		// is a similar item stored?
		unsigned x;
		for (x = 0; x < m_network_list.size(); ++x) {
			if ((m_network_list[x].transport_stream_id == information.transport_stream_id) &&
				(m_network_list[x].original_network_id == information.original_network_id)) {
					if (! m_network_list[x].descriptor_list.Process(pbuf, desc_length) )
						return TS_FALSE;
					break;
			}
		}

		// not found? add
		if (x == m_network_list.size()) {
			if (! information.descriptor_list.Process(pbuf, desc_length) )
				return TS_FALSE;
			m_network_list.push_back(information);	
		}

		pbuf += desc_length;
		loop_length -= desc_length;
	
	}

	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
}

TS_WORD	tssi::Table_Nit::GetNetworkId() const
{
	return m_network_id;
}

const tssi::DescriptorList& tssi::Table_Nit::GetCommonDescriptorList() const
{
	return m_descriptor_list;
}

unsigned tssi::Table_Nit::GetNetworkListLength() const
{
	return static_cast<unsigned> ( m_network_list.size() );
}

TS_WORD	tssi::Table_Nit::GetNetworkTransportStreamId(unsigned index) const
{
	if (index >= GetNetworkListLength()) return 0;
	return m_network_list[index].transport_stream_id;
}

TS_WORD	tssi::Table_Nit::GetNetworkOriginalNetworkId(unsigned index) const
{
	if (index >= GetNetworkListLength()) return 0;
	return m_network_list[index].original_network_id;
}

const tssi::DescriptorList& tssi::Table_Nit::GetNetworkDescriptorList(unsigned index) const
{
	if (index >= GetNetworkListLength()) return m_missing_list;
	return m_network_list[index].descriptor_list;
}
