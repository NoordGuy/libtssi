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

#include "tables/table_pmt.h"
#include "utils/parsemac2.h"

tssi::Table_Pmt::Table_Pmt() : Table()
{
}

tssi::Table_Pmt::~Table_Pmt()
{
	Table::~Table();
	m_pmt.clear();

}

TS_VOID tssi::Table_Pmt::Reset()
{
	Table::Reset();
	m_pmt.clear();

}

TS_BOOL	tssi::Table_Pmt::Process(TS_PBYTE pbuf, unsigned in_len)
{
	// process table?

	TS_BOOL new_version = TS_FALSE;
	if (!Table::Process(pbuf, in_len, TS_FALSE, new_version)) 
		return TS_FALSE;
	if (!new_version)
		return TS_TRUE;
	if (m_table_id != 0x02)
		return TS_FALSE;

	// a PMT should only use only one section...

	TS_WORD prg_nr = PMT_PROGRAM_NUMBER_VALUE(pbuf);
	TS_WORD desc_len = PMT_PROGRAM_INFO_LENGTH_VALUE(pbuf);

	PmtInformation pmt_information;
	pmt_information.pcr_pid = PMT_PCR_PID_VALUE(pbuf);
	
	TS_PBYTE pstart_es  = pbuf + 12 + desc_len; // points to the stream_type of the 1st elementary stream
	TS_PBYTE pend_es    = pbuf + m_section_length + 3 - 4;   // points to the byte before CRC_32
	TS_PBYTE ptmp;

    TS_BYTE es_number = 0;
    unsigned offset = 0;

	for (ptmp = pstart_es; ptmp < pend_es; ptmp += offset) {

		offset = 5 + PMT_STREAM_RECORD_ES_INFO_LENGTH(ptmp); // 5 + ES_info_length

		EsInformation elementary_stream;

		elementary_stream.elementary_pid = PMT_STREAM_RECORD_ELEMENTARY_PID(ptmp);
		elementary_stream.stream_type = PMT_STREAM_RECORD_STREAM_TYPE_VALUE(ptmp);

		if (! elementary_stream.descriptor_list.Process(ptmp + 5, offset - 5) )
			return TS_FALSE;

		pmt_information.es_information.push_back(elementary_stream);
		
	}
	m_pmt[prg_nr] = pmt_information;

	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
}

unsigned tssi::Table_Pmt::GetProgramListLength() const
{
	return static_cast<unsigned> ( m_pmt.size() );
}

TS_WORD	tssi::Table_Pmt::GetProgramNumber(unsigned index) const
{
	if (index >= GetProgramListLength()) return 0;
	std::map<TS_WORD, PmtInformation>::const_iterator iter;
	unsigned x = 0;
	for (iter = m_pmt.begin(); iter != m_pmt.end(); ++iter, ++x) {
		if (x == index)
			return iter->first;
	}
	return 0;
}

TS_WORD	tssi::Table_Pmt::GetPcrPid(TS_WORD program) const
{
	std::map<TS_WORD, PmtInformation>::const_iterator iter = m_pmt.find(program);
	if (iter != m_pmt.end())
		return iter->second.pcr_pid;
	else
		return 0;
}

unsigned tssi::Table_Pmt::GetEsListLength(TS_WORD program) const
{
	std::map<TS_WORD, PmtInformation>::const_iterator iter = m_pmt.find(program);
	if (iter != m_pmt.end())
		return static_cast<unsigned> ( iter->second.es_information.size() );
	else
		return 0;
}

TS_BYTE	tssi::Table_Pmt::GetEsType(TS_WORD program, unsigned index_stream) const
{
	std::map<TS_WORD, PmtInformation>::const_iterator iter = m_pmt.find(program);
	if (iter != m_pmt.end()) {
		if ( index_stream >= iter->second.es_information.size() )
			return 0;
		else
			return iter->second.es_information[index_stream].stream_type;		
	} else
		return 0;
}

TS_WORD	tssi::Table_Pmt::GetEsPid(TS_WORD program, unsigned index_stream) const
{
	std::map<TS_WORD, PmtInformation>::const_iterator iter = m_pmt.find(program);
	if (iter != m_pmt.end()) {
		if ( index_stream >= iter->second.es_information.size() )
			return 0;
		else
			return iter->second.es_information[index_stream].elementary_pid;		
	} else
		return 0;
}

const tssi::DescriptorList& tssi::Table_Pmt::GetEsDescriptorList(TS_WORD program, unsigned index_stream) const
{
	std::map<TS_WORD, PmtInformation>::const_iterator iter = m_pmt.find(program);
	if (iter != m_pmt.end()) {
		if ( index_stream >= iter->second.es_information.size() )
			return m_missing_descriptor_list;
		else
			return iter->second.es_information[index_stream].descriptor_list;		
	} else
		return m_missing_descriptor_list;
}
