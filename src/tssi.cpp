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

#include "tssi.h"
#include "utils/parsemac2.h"

tssi::Parser::Parser() 
{
	m_pid_dsmcc = 0xffff;
	m_pid_ait = 0xffff;
	m_pid_ebu = 0xffff;
	m_pid_pcr = 0xffff;

	m_processed_packets = 0;
	m_processing_errors = 0;
}

tssi::Parser::~Parser()
{
}

TS_VOID tssi::Parser::Reset() 
{
	m_pid_dsmcc = 0xffff;
	m_pid_ait = 0xffff;
	m_pid_ebu = 0xffff;
	m_pid_pcr = 0xffff;

	m_processed_packets = 0;
	m_processing_errors = 0;

	m_fragment_length = 0;

	m_packet_ait.Reset();
	m_packet_dsmcc.Reset();
	m_packet_ebu.Reset();
	m_packet_eit.Reset();
	m_packet_nit.Reset();
	m_packet_pat.Reset();
	m_packet_pcr.Reset();
	m_packet_pmt.Reset();
	m_packet_sdt.Reset();
	m_packet_tdt.Reset();
}

TS_BOOL	tssi::Parser::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (in_len < 376) return TS_FALSE;

   	unsigned i = 0;
    if (m_fragment_length > 0 && 
		in_len >= 188 - m_fragment_length) {

        if (in_len >= 752 && 
			pbuf[0] == 0x47 && 
			pbuf[188] == 0x47 &&
            pbuf[376] == 0x47 && 
			pbuf[564] == 0x47) {
				m_fragment_length = 0;
        } else {
            if (in_len == 188 - m_fragment_length || 
				(in_len > 188 - m_fragment_length && 
				pbuf[188 - m_fragment_length] == 0x47)) {
					memcpy(m_packet_buffer + m_fragment_length, pbuf, 188 - m_fragment_length);
					if (! FilterTsPacket(m_packet_buffer) )
						return TS_FALSE;
					i += 188 - m_fragment_length;
            }
        }
    }
    while (i + 188 < in_len) {
        if (pbuf[i] == 0x47 && 
			pbuf[i + 188] == 0x47) {
				if (! FilterTsPacket(pbuf + i) )
					return TS_FALSE;
				i += 188;
        } else {
            ++i; 
        }
    }

	if (i < in_len && 
		i + 188 > in_len && 
		pbuf[i] == 0x47) {
			m_fragment_length = in_len - i;
			memcpy(m_packet_buffer, pbuf + i, m_fragment_length);
	} else {

		if (i + 188 == in_len && 
			pbuf[i] == 0x47 &&  
			pbuf[i-188] == 0x47) {
				if (! FilterTsPacket(pbuf + i) )
					return TS_FALSE;
				i += 188;
		}
		m_fragment_length = 0;
	}
	return TS_TRUE;
}

TS_BOOL tssi::Parser::FilterTsPacket(TS_PBYTE pbuf)
{
	// send the data to the pid processors
	TS_WORD pid = PID_VALUE(pbuf);

	unsigned x;
	unsigned program_count = m_packet_pat.GetTablePat().GetProgramListLength();

	for (x = 0; x < program_count; ++x) {
		if ( pid == m_packet_pat.GetTablePat().GetProgramMapPid(x) )
			pid = 0xefff; // pmt pseudo pid
	}
	
	TS_BOOL result = TS_TRUE;
	TS_BOOL processed = TS_FALSE;

	switch (pid) {
		case 0x0000: result *= m_packet_pat.Process(pbuf, 188); processed = TS_TRUE; break; 
		case 0x0010: result *= m_packet_nit.Process(pbuf, 188); processed = TS_TRUE; break;
		case 0x0011: result *= m_packet_sdt.Process(pbuf, 188); processed = TS_TRUE; break;
		case 0xeFFF: result *= m_packet_pmt.Process(pbuf, 188); processed = TS_TRUE; break;
		case 0x0012: result *= m_packet_eit.Process(pbuf, 188); processed = TS_TRUE; break;
		case 0x0014: result *= m_packet_tdt.Process(pbuf, 188); processed = TS_TRUE; break;
	}

	if (pid == m_pid_dsmcc)	result *= m_packet_dsmcc.Process(pbuf, 188); processed = TS_TRUE;
	if (pid == m_pid_ait)	result *= m_packet_ait.Process(pbuf, 188); processed = TS_TRUE;  
	if (pid == m_pid_pcr)	result *= m_packet_pcr.Process(pbuf, 188); processed = TS_TRUE;  
	if (pid == m_pid_ebu)	result *= m_packet_ebu.Process(pbuf, 188); processed = TS_TRUE;  

	// protocol results
	if (processed) {
		++m_processed_packets;
		if (!result)
			++m_processing_errors;
	}

	// always return true, we do not want to interrupt parsing
	return TS_TRUE;
}

TS_VOID tssi::Parser::SetPidDsmcc(TS_WORD pid_value)
{
	m_pid_dsmcc = pid_value;
}

TS_VOID tssi::Parser::SetPidAit(TS_WORD pid_value)
{
	m_pid_ait = pid_value;
}

TS_VOID tssi::Parser::SetPidPcr(TS_WORD pid_value)
{
	m_pid_pcr = pid_value;
}

TS_VOID tssi::Parser::SetPidEbu(TS_WORD pid_value)
{
	m_pid_ebu = pid_value;
}

TS_DWORD tssi::Parser::ProcessingErrors() const
{
	return m_processing_errors;
}

TS_DWORD tssi::Parser::PacketsProcessed() const
{
	return m_processed_packets;
}


const tssi::Packet_Ebu& tssi::Parser::PacketEbu() const
{
	return m_packet_ebu;
}

const tssi::Packet_Pcr& tssi::Parser::PacketPcr() const
{
	return m_packet_pcr;
}

const tssi::Table_Ait& tssi::Parser::TableAit() const
{
	return m_packet_ait.GetTableAit();
}

const tssi::Table_Dsmcc& tssi::Parser::TableDsmcc() const
{
	return m_packet_dsmcc.GetTableDsmcc();
}

const tssi::Table_Eit& tssi::Parser::TableEit() const
{
	return m_packet_eit.GetTableEit();
}

const tssi::Table_Nit& tssi::Parser::TableNit() const
{
	return m_packet_nit.GetTableNit();
}

const tssi::Table_Pat& tssi::Parser::TablePat() const
{
	return m_packet_pat.GetTablePat();
}

const tssi::Table_Pmt& tssi::Parser::TablePmt() const
{
	return m_packet_pmt.GetTablePmt();
}

const tssi::Table_Sdt& tssi::Parser::TableSdt() const
{
	return m_packet_sdt.GetTableSdt();
}

const tssi::Table_Tdt& tssi::Parser::TableTdt() const
{
	return m_packet_tdt.GetTableTdt();
}