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

#include "tables/table_pat.h"
#include "utils/parsemac2.h"

tssi::Table_Pat::Table_Pat() : Table()
{

	m_transport_stream_id = 0;
	m_network_pid = 0;
}

tssi::Table_Pat::~Table_Pat()
{
	Table::~Table();
}

TS_VOID tssi::Table_Pat::Reset()
{
	Table::Reset();

	m_program_list.clear();

	m_network_pid = 0;
	m_transport_stream_id = 0;
}

TS_BOOL	tssi::Table_Pat::Process(TS_PBYTE pbuf, unsigned in_len)
{
	// process table?

	TS_BOOL new_version = TS_FALSE;
	if (!Table::Process(pbuf, in_len, TS_FALSE, new_version)) 
		return TS_FALSE;
	if (!new_version)
		return TS_TRUE;
	if (m_table_id != 0x00)
		return TS_FALSE;
	
	m_transport_stream_id = PAT_TRANSPORT_STREAM_ID_VALUE(pbuf);

	// one section table
	m_program_list.clear();
	TS_WORD number_of_programs = (m_section_length - 9) / 4;	// max 252

	unsigned x;
	for (x = 0; x < number_of_programs; ++x) {

		PatProgram program;
		program.program_number = PAT_PROGRAM_DESCRIPTOR_PROGRAM_NUMBER_VALUE(pbuf,x);
		program.program_map_pid = PAT_PROGRAM_DESCRIPTOR_PID_VALUE(pbuf,x);

		if (program.program_number == 0x0000) {
			m_network_pid = program.program_map_pid;
			continue;
		}

		m_program_list.push_back(program);
	}

	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
}

TS_WORD	tssi::Table_Pat::GetTransportStreamId() const
{
	return m_transport_stream_id;
}

TS_WORD	tssi::Table_Pat::GetNetworkPid() const
{
	return m_network_pid;
}

unsigned tssi::Table_Pat::GetProgramListLength() const
{
	return static_cast<unsigned> ( m_program_list.size() );
}

TS_WORD	tssi::Table_Pat::GetProgramNumber(unsigned index) const
{
	if (index >= GetProgramListLength()) return 0;
	return m_program_list[index].program_number;
}

TS_WORD	tssi::Table_Pat::GetProgramMapPid(unsigned index) const
{
	if (index >= GetProgramListLength()) return 0;
	return m_program_list[index].program_map_pid;
}
