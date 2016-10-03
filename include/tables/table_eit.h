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

#ifndef __TSSI_TABLE_EIT_H_INCLUDED__
#define __TSSI_TABLE_EIT_H_INCLUDED__

#include "tables/table.h"
#include "descriptors/descriptorlist.h"

namespace tssi {

	struct EitEvent {
		TS_WORD			transport_stream_id;
		TS_WORD			original_network_id;
		TS_WORD			service_id;
		TS_WORD			event_id;
		TS_QWORD		start_time;
		TS_DWORD		duration;
		TS_BYTE			running_status;
		TS_BOOL			free_ca_mode;
		DescriptorList	descriptor_list;
	};

	class TS_EXTERN Table_Eit : public Table {
		TS_PRIVATE_EXPORT( std::vector<EitEvent> m_eit_data );
		EitEvent m_missing_event;
		
	public:
		Table_Eit();
		virtual ~Table_Eit();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		unsigned			GetEventListLength() const;
		const EitEvent&		GetEvent(unsigned index) const;

	
	};

}

#endif // __TSSI_TABLE_EIT_H_INCLUDED__