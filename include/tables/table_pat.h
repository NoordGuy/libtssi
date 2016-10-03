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

#ifndef __TSSI_TABLE_PAT_H_INCLUDED__
#define __TSSI_TABLE_PAT_H_INCLUDED__

#include "tables/table.h"

namespace tssi {

	class TS_EXTERN Table_Pat : public Table {

		struct PatProgram {
			TS_WORD		program_number;
			TS_WORD		program_map_pid;
		};

		TS_PRIVATE_EXPORT( std::vector<PatProgram> m_program_list );
				
		TS_WORD	m_transport_stream_id;
		TS_WORD	m_network_pid;

	public:
		Table_Pat();
		virtual ~Table_Pat();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		TS_WORD		GetTransportStreamId() const;
		TS_WORD		GetNetworkPid() const;
		unsigned	GetProgramListLength() const;
		TS_WORD		GetProgramNumber(unsigned index) const;
		TS_WORD		GetProgramMapPid(unsigned index) const;

	};

}

#endif // __TSSI_TABLE_PAT_H_INCLUDED__
