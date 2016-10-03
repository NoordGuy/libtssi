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

#include "packets/packet_ebu.h"
#include "utils/parsemac2.h"

namespace {
	// Tables from dvbsnoop, file: ebu_misc.c
	// http://dvbsnoop.sourceforge.net/

const TS_BYTE kBitReverseLookUpTable[256] = {
0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 
0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 
0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 
0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 
0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 
0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 
0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 
0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 
0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 
0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 
0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 
0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 
0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 
0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 
0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 
0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 
0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 
0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 
0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 
0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 
0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 
0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 
0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 
0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 
0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 
0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 
0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 
0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 
0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 
0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 
0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff, 
};

// -- Hamming 8/4 table
//
//  Bit 1  2  3  4  5  6  7  8    (transmission order)
//      P1 D1 P2 D2 P3 D3 P4 D4
//
//      P = Control/Parity bit,  D = Data bit
//

const TS_BYTE kUnHamming84LookUpTable[256] = {
0x01, 0xff, 0x01, 0x01, 0xff, 0x00, 0x01, 0xff, 
0xff, 0x02, 0x01, 0xff, 0x0a, 0xff, 0xff, 0x07, 
0xff, 0x00, 0x01, 0xff, 0x00, 0x80, 0xff, 0x00, 
0x06, 0xff, 0xff, 0x0b, 0xff, 0x00, 0x03, 0xff, 
0xff, 0x0c, 0x01, 0xff, 0x04, 0xff, 0xff, 0x07, 
0x06, 0xff, 0xff, 0x07, 0xff, 0x07, 0x07, 0x87, 
0x06, 0xff, 0xff, 0x05, 0xff, 0x00, 0x0d, 0xff, 
0x86, 0x06, 0x06, 0xff, 0x06, 0xff, 0xff, 0x07, 
0xff, 0x02, 0x01, 0xff, 0x04, 0xff, 0xff, 0x09, 
0x02, 0x82, 0xff, 0x02, 0xff, 0x02, 0x03, 0xff, 
0x08, 0xff, 0xff, 0x05, 0xff, 0x00, 0x03, 0xff, 
0xff, 0x02, 0x03, 0xff, 0x03, 0xff, 0x83, 0x03, 
0x04, 0xff, 0xff, 0x05, 0x84, 0x04, 0x04, 0xff, 
0xff, 0x02, 0x0f, 0xff, 0x04, 0xff, 0xff, 0x07, 
0xff, 0x05, 0x05, 0x85, 0x04, 0xff, 0xff, 0x05, 
0x06, 0xff, 0xff, 0x05, 0xff, 0x0e, 0x03, 0xff, 
0xff, 0x0c, 0x01, 0xff, 0x0a, 0xff, 0xff, 0x09, 
0x0a, 0xff, 0xff, 0x0b, 0x8a, 0x0a, 0x0a, 0xff, 
0x08, 0xff, 0xff, 0x0b, 0xff, 0x00, 0x0d, 0xff, 
0xff, 0x0b, 0x0b, 0x8b, 0x0a, 0xff, 0xff, 0x0b, 
0x0c, 0x8c, 0xff, 0x0c, 0xff, 0x0c, 0x0d, 0xff, 
0xff, 0x0c, 0x0f, 0xff, 0x0a, 0xff, 0xff, 0x07, 
0xff, 0x0c, 0x0d, 0xff, 0x0d, 0xff, 0x8d, 0x0d, 
0x06, 0xff, 0xff, 0x0b, 0xff, 0x0e, 0x0d, 0xff, 
0x08, 0xff, 0xff, 0x09, 0xff, 0x09, 0x09, 0x89, 
0xff, 0x02, 0x0f, 0xff, 0x0a, 0xff, 0xff, 0x09, 
0x88, 0x08, 0x08, 0xff, 0x08, 0xff, 0xff, 0x09, 
0x08, 0xff, 0xff, 0x0b, 0xff, 0x0e, 0x03, 0xff, 
0xff, 0x0c, 0x0f, 0xff, 0x04, 0xff, 0xff, 0x09, 
0x0f, 0xff, 0x8f, 0x0f, 0xff, 0x0e, 0x0f, 0xff, 
0x08, 0xff, 0xff, 0x05, 0xff, 0x0e, 0x0d, 0xff, 
0xff, 0x0e, 0x0f, 0xff, 0x0e, 0x8e, 0xff, 0x0e, 
};
} // namespace

tssi::Packet_Ebu::Packet_Ebu()
{
	m_table = 0;

	m_continuity_counter = 0;
	m_pes_packet_length = 0;
	m_current_page = 0;
	memset(m_current_time_page, 0, 8);

	m_missing_page.magazine = 255;

	m_callback = 0;
}


tssi::Packet_Ebu::~Packet_Ebu()
{
	unsigned x;
	for (x = 0; x < m_pes_packet_list.size(); ++x) 
		delete[] m_pes_packet_list[x].buffer;
	m_pes_packet_list.clear();

	std::map<TS_DWORD, EbuPage*>::const_iterator iter;
	for (iter = m_ebu_pages.begin(); iter != m_ebu_pages.end(); m_ebu_pages.erase(iter++)) {
		delete iter->second;
	}
	if (m_current_page) {
		delete m_current_page;
		m_current_page = 0;
	}
	
}

// prevent copy and assignment
tssi::Packet_Ebu::Packet_Ebu( const tssi::Packet_Ebu& other ) {}
tssi::Packet_Ebu& tssi::Packet_Ebu::operator= (const tssi::Packet_Ebu& rhs) { return *this; }

TS_VOID tssi::Packet_Ebu::ResetTable()
{
}

TS_VOID tssi::Packet_Ebu::CreateTable()
{
}

TS_VOID tssi::Packet_Ebu::DestroyTable()
{
}

TS_VOID tssi::Packet_Ebu::Reset()
{
	m_continuity_counter = 0;
	m_pes_packet_length = 0;
	unsigned x;
	for (x = 0; x < m_pes_packet_list.size(); ++x) 
		delete[] m_pes_packet_list[x].buffer;
	m_pes_packet_list.clear();

	std::map<TS_DWORD, EbuPage*>::const_iterator iter;
	for (iter = m_ebu_pages.begin(); iter != m_ebu_pages.end(); m_ebu_pages.erase(iter++)) {
		delete iter->second;
	}
	if (m_current_page) {
		delete m_current_page;
		m_current_page = 0;
	}

	memset(m_current_time_page, 0, 8);
	
	m_callback = 0;
}

TS_BYTE tssi::Packet_Ebu::DeHamming84(TS_BYTE byte_value) const
{
	// check
	return kUnHamming84LookUpTable[byte_value];
}


TS_BYTE tssi::Packet_Ebu::DeHamming84W(TS_BYTE lsb, TS_BYTE msb, TS_BOOL& test) const
{
	TS_BYTE c1, c2;

	c1 = kUnHamming84LookUpTable[lsb];
	c2 = kUnHamming84LookUpTable[msb];
	if ((c1 | c2) & 0x40) 
		test = TS_FALSE;
	else
		test = TS_TRUE;
	return (c2 << 4) | (c1 & 0x0f);	
}

TS_BYTE tssi::Packet_Ebu::DeHamming84W(TS_BYTE lsb, TS_BYTE msb) const
{
	TS_BYTE c1, c2;

	c1 = kUnHamming84LookUpTable[lsb];
	c2 = kUnHamming84LookUpTable[msb];
	return (c2 << 4) | (c1 & 0x0f);
}

TS_BOOL tssi::Packet_Ebu::CheckParity(TS_BYTE test_value) const
{
	TS_BOOL b1 = (test_value >> 7) & 0x01;
	TS_BOOL b2 = (test_value >> 6) & 0x01;
	TS_BOOL b3 = (test_value >> 5) & 0x01;
	TS_BOOL b4 = (test_value >> 4) & 0x01;
	TS_BOOL b5 = (test_value >> 3) & 0x01;
	TS_BOOL b6 = (test_value >> 2) & 0x01;
	TS_BOOL b7 = (test_value >> 1) & 0x01;
	TS_BOOL p  = (test_value >> 0) & 0x01;

	return b1^b2^b3^b4^b5^b6^b7^p;
}

TS_VOID	tssi::Packet_Ebu::DeParity(TS_PBYTE pbuf, unsigned in_len) const
{
	unsigned x;
	for (x = 0; x < in_len; ++x)
	{
		pbuf[x] = pbuf[x] & 0x7F;
	}
}

TS_VOID	tssi::Packet_Ebu::ReverseBits (TS_PBYTE pbuf, unsigned in_len) const
{
	unsigned x;
	for (x = 0; x < in_len; ++x) 
	{
		pbuf[x] = kBitReverseLookUpTable[ pbuf[x] ];
	}
}

TS_BOOL	tssi::Packet_Ebu::Process(TS_PBYTE pbuf, unsigned in_len)
{
	TS_BOOL payload_unit_start_indicator = PAYLOAD_UNIT_START_INDICATOR_BIT(pbuf);
	TS_BYTE adaptation_field_control = ADAPTATION_FIELD_CONTROL_VALUE(pbuf);

	TS_BYTE continuity_counter = CONTINUITY_COUNTER_VALUE(pbuf);
	if (m_continuity_counter == continuity_counter) return TS_TRUE;
	m_continuity_counter = continuity_counter;

	TS_BOOL result_value = TS_TRUE;
	if (payload_unit_start_indicator) {
		// is a complete PES packet available?

		if (m_pes_packet_list.size() > 0) {
			unsigned total_length = 0;
			unsigned x;
			for (x = 0; x < m_pes_packet_list.size(); ++x) 
				total_length += m_pes_packet_list[x].length;

			if ((m_pes_packet_length == total_length) || (m_pes_packet_length == 0)) {

				TS_PBYTE unified_buffer = new TS_BYTE[total_length];
				unsigned pos = 0;
				for (x = 0; x < m_pes_packet_list.size(); ++x)
				{
					memcpy(unified_buffer + pos, m_pes_packet_list[x].buffer, m_pes_packet_list[x].length);
					pos += m_pes_packet_list[x].length;
				}

				if (PES_PACKET_START_CODE_PREFIX_VALUE(unified_buffer) == 0x01) {
					result_value = FilterPes(unified_buffer, total_length);
				} 
				delete[] unified_buffer;
			}

			m_pes_packet_length = 0;
			for (x = 0; x < m_pes_packet_list.size(); ++x) 
				delete[] m_pes_packet_list[x].buffer;
			m_pes_packet_list.clear();

		}

	}

	if ((adaptation_field_control == 0x01) || (adaptation_field_control == 0x03)) {
		
		unsigned residual_size = 184;
		pbuf += 4;

		if (adaptation_field_control==0x03) {
			TS_BYTE adaptation_field_length = ADAPTATION_FIELD_LENGTH_VALUE(pbuf);
			pbuf += adaptation_field_length + 1;
			residual_size -= adaptation_field_length + 1;
		}

		TS_DWORD packet_start_code_prefix = PES_PACKET_START_CODE_PREFIX_VALUE(pbuf);

		if ((packet_start_code_prefix == 0x01) && (payload_unit_start_indicator))
		{
			// new stream starts here
			unsigned x;
			for (x = 0; x < m_pes_packet_list.size(); ++x) 
				delete[] m_pes_packet_list[x].buffer;
			m_pes_packet_list.clear();


			TS_BYTE stream_id = PES_STREAM_ID_VALUE(pbuf);
			m_pes_packet_length = PES_PACKET_LENGTH_VALUE(pbuf);

			if ((residual_size > m_pes_packet_length) && (m_pes_packet_length > 0)) residual_size = m_pes_packet_length;

			PesPacket pes;
			pes.buffer = new TS_BYTE[residual_size];
			memcpy(pes.buffer, pbuf, residual_size);
			pes.length = residual_size;

			m_pes_packet_list.push_back(pes);

		} else {
			// an open stream continues
			unsigned total_length = 0;
			unsigned x;
			for (x = 0; x < m_pes_packet_list.size(); ++x) 
				total_length += m_pes_packet_list[x].length;

			if (m_pes_packet_length > 0)
				if (residual_size > (m_pes_packet_length - total_length)) 
					residual_size = m_pes_packet_length - total_length;

			PesPacket pes;
			pes.buffer = new TS_BYTE[residual_size];
			memcpy(pes.buffer, pbuf, residual_size);
			pes.length = residual_size;

			m_pes_packet_list.push_back(pes);
		}
	}

	return result_value;

}

TS_BOOL	tssi::Packet_Ebu::FilterPes(TS_PBYTE pbuf, unsigned in_len) 
{
	TS_BYTE stream_id = PES_STREAM_ID_VALUE(pbuf);
	if (stream_id != 0xBD) return TS_FALSE;	// private_stream_1

	TS_WORD residual_length = PES_PACKET_LENGTH_VALUE(pbuf);

	pbuf += 6;
	residual_length -= 6;

	TS_BOOL data_align = PES_DATA_ALIGNMENT_INDICATOR_BIT(pbuf);
	TS_BYTE header_length = PES_HEADER_DATA_LENGTH_VALUE(pbuf);
	if (header_length != 0x24) return TS_FALSE;

	pbuf += 3;
	residual_length -= 3;	
	pbuf += header_length;
	residual_length -= header_length;

	return FilterDataField(pbuf, residual_length);
}

TS_BOOL	tssi::Packet_Ebu::FilterDataField(TS_PBYTE pbuf, unsigned in_len)
{
	TS_BYTE id = EBU_DATA_IDENTIFIER_VALUE(pbuf);
	if ((id < 0x10) || (id > 0x1F)) return TS_FALSE;

	++pbuf;
	unsigned residual_length = in_len - 1;

	while (residual_length >= 46) {
		TS_BYTE du_id = EBU_DATA_UNIT_ID_VALUE(pbuf);
		TS_BYTE du_len = EBU_DATA_UNIT_LENGTH_VALUE(pbuf);
		//if (du_len != 0x2C) return FALSE;		

		TS_BOOL parity = EBU_FIELD_PARITY_BIT(pbuf);
		TS_BYTE line = EBU_LINE_OFFSET_VALUE(pbuf);

		ReverseBits(pbuf, 46);

		TS_BYTE framing = EBU_FRAMING_CODE_VALUE(pbuf);
		TS_BYTE mag = EBU_MAGAZINE_VALUE(pbuf);
		TS_BYTE packet = EBU_PACKET_NUMBER_VALUE(pbuf);
		//if (framing != 0x27) continue;

		pbuf += 6;
		residual_length -= 6;

		TS_BOOL result = TS_FALSE;
		TS_BYTE x = DeHamming84W(mag, packet, result);
		if (result) {
			packet = (x >> 3) & 0x1F;
			mag = x & 7;
			if (mag == 0) mag = 8;

			if (((line < 0x07) || (line > 0x16)) && (line != 0x00)) {
				// bad line
			} else {
				if (! FilterEbuPacket(mag, packet, pbuf) )
					return TS_FALSE;
			}
		}
		pbuf += 40;
		residual_length -= 40;		
	}

	return TS_TRUE;
}

TS_BOOL	tssi::Packet_Ebu::FilterEbuPacket(TS_BYTE magazine, TS_BYTE packet, TS_PBYTE pbuf)
{
	if (packet == 0) {
		
		TS_BYTE page_nr     = (DeHamming84(*(pbuf+0)) & 0xf) | ( (DeHamming84(*(pbuf+1))  & 0xf) << 4);
		TS_WORD sub_page_nr = (DeHamming84W(*(pbuf+2),*(pbuf+3)) | (DeHamming84W(*(pbuf+4),*(pbuf+5)) << 8) ) & 0x3f7f;

		// add old packet
		if (m_current_page) {
			TS_DWORD code = 
				static_cast<TS_DWORD> ( m_current_page->magazine ) * 10000000 +
				static_cast<TS_DWORD> ( m_current_page->page_number ) * 100000 +
				static_cast<TS_DWORD> ( m_current_page->sub_page_number );

			// update
			if (m_ebu_pages.find(code) != m_ebu_pages.end())
				delete m_ebu_pages[code];
			m_ebu_pages[code] = m_current_page;

			// can we use the time code?
			if (!m_current_page->suppress_header_flag)
				memcpy(m_current_time_page, m_current_page->header_data + 24, 8);

			if (m_callback)
				m_callback(m_callback_data);
		}
		// create new packet
		m_current_page = new EbuPage();
		
		// page header packet (X/0)
		m_current_page->page_number = page_nr;
		m_current_page->sub_page_number = sub_page_nr;
		m_current_page->magazine = magazine;
				
		TS_BYTE _x;
		TS_BOOL c4,c5,c6,c7,c8,c9,c10,c11;

		c4 = *(pbuf+3) & 0x80;		// bit 8
		_x  = *(pbuf+5);
		c5 = _x & 0x20;				// bit 6
		c6 = _x & 0x80;				// bit 7
		_x  = *(pbuf+6);
		c7 = _x & 0x02;				// bit 2
		c8 = _x & 0x08;				// bit 4
		c9 = _x & 0x20;				// bit 6
		c10= _x & 0x80;				// bit 8
		c11  = *(pbuf+7) & 0x02;	// bit 2

		TS_BYTE lang;
		lang = (DeHamming84(*(pbuf+7)) >> 1) & 7;	// unhammed bits 4, 6, 8

		m_current_page->erase_page_flag = c4;
		m_current_page->newsflash_flag = c5;
		m_current_page->subtitle_flag = c6;
		m_current_page->suppress_header_flag = c7;
		m_current_page->update_indicator_flag = c8;
		m_current_page->interrupted_sequence_flag = c9;
		m_current_page->inhibit_display_flag = c10;
		m_current_page->magazine_serial_flag = c11;
		m_current_page->language_code = lang;

		pbuf += 8;
		memcpy(m_current_page->header_data, pbuf, 32);
		DeParity(m_current_page->header_data, 32); 

	} else {
		if (m_current_page) {
			// continue an open page
			EbuLine line;
			line.packet = packet;
			memcpy(line.line_data, pbuf, 40);
			DeParity(line.line_data, 40);
			m_current_page->page_data.push_back(line);

		}
	}

	return TS_TRUE;
}

unsigned tssi::Packet_Ebu::GetEbuPageListLength() const
{
	return static_cast<unsigned> ( m_ebu_pages.size() );
}

const tssi::EbuPage& tssi::Packet_Ebu::GetEbuPage(unsigned index) const
{
	if (index >= GetEbuPageListLength()) return m_missing_page;
	std::map<TS_DWORD, EbuPage*>::const_iterator iter;
	unsigned x = 0;
	for (iter = m_ebu_pages.begin(); iter != m_ebu_pages.end(); ++iter, ++x) {
		if (x == index)
			return *(iter->second);
	}
	return m_missing_page;
}

const tssi::EbuPage& tssi::Packet_Ebu::GetEbuPage(TS_BYTE magazine, TS_BYTE page_number, TS_WORD sub_page_number) const
{
	TS_DWORD code = 
		static_cast<TS_DWORD> ( magazine ) * 10000000 +
		static_cast<TS_DWORD> ( page_number ) * 100000 +
		static_cast<TS_DWORD> ( sub_page_number );

	std::map<TS_DWORD, EbuPage*>::const_iterator iter = m_ebu_pages.find(code);
	if (iter == m_ebu_pages.end())
		return m_missing_page;
	else
		return *(iter->second);
}


TS_STRING tssi::Packet_Ebu::GetCurrentTimeHeader() const
{
	TS_STRING result( reinterpret_cast<char const*>(m_current_time_page), sizeof(m_current_time_page) ) ;
	return result;
}

TS_VOID tssi::Packet_Ebu::SetNewPageCallback(TS_CALLBACK callback, TS_PVOID data)
{
	m_callback = callback;
	m_callback_data = data;
}


