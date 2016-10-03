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

#ifndef __TSSI_TABLE_SDT_H_INCLUDED__
#define __TSSI_TABLE_SDT_H_INCLUDED__

#include "tables/table.h"
#include "descriptors/descriptorlist.h"

namespace tssi {

	struct ServiceDescription {
		TS_WORD			transport_stream_id;
		TS_WORD			original_network_id;
		TS_WORD			service_id;
		TS_BOOL			eit_schedule_flag;
		TS_BOOL			eit_present_following_flag;
		TS_BYTE			running_status;
		TS_BOOL			free_ca_mode;
		DescriptorList	descriptor_list;
	};

	class TS_EXTERN Table_Sdt : public Table {
		TS_PRIVATE_EXPORT( std::vector<ServiceDescription> m_sdt_data );
		ServiceDescription m_missing_service;

	public:
		Table_Sdt();
		virtual ~Table_Sdt();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		unsigned					GetServiceListLength() const;
		const ServiceDescription&	GetServiceDescription(unsigned index) const;


	};

}

#endif // __TSSI_TABLE_SDT_H_INCLUDED__