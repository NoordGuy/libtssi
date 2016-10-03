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

#ifndef __TSSI_TABLE_NIT_H_INCLUDED__
#define __TSSI_TABLE_NIT_H_INCLUDED__

#include "tables/table.h"
#include "descriptors/descriptorlist.h"

namespace tssi {

	struct NetworkInformation {
		TS_WORD			transport_stream_id;
		TS_WORD			original_network_id;
		DescriptorList	descriptor_list;
	};

	class TS_EXTERN Table_Nit : public Table {

		TS_WORD			m_network_id;
		TS_BYTE			m_version_number;
		DescriptorList	m_descriptor_list;
		TS_PRIVATE_EXPORT( std::vector<NetworkInformation> m_network_list );

		DescriptorList	m_missing_list;
		
	public:
		Table_Nit();
		virtual ~Table_Nit();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		TS_WORD					GetNetworkId() const;
		const DescriptorList&	GetCommonDescriptorList() const;
		unsigned				GetNetworkListLength() const;
		TS_WORD					GetNetworkTransportStreamId(unsigned index) const;
		TS_WORD					GetNetworkOriginalNetworkId(unsigned index) const;
		const DescriptorList&	GetNetworkDescriptorList(unsigned index) const;

	};

}

#endif // __TSSI_TABLE_NIT_H_INCLUDED__