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

#ifndef __TSSI_PACKET_EBU_H_INCLUDED__
#define __TSSI_PACKET_EBU_H_INCLUDED__

#include "packets/packet.h"
#include <vector>
#include <map>

namespace tssi {

	class Packet_Ebu;

	struct TS_EXTERN EbuLine {
		TS_BYTE	packet;
		TS_BYTE	line_data[40];

		TS_STRING GetLineData() const { char char_line[40]; std::copy(line_data, line_data + 40, char_line); return TS_STRING (char_line, 40); }
	};

	struct TS_EXTERN EbuPage {
		TS_BYTE	magazine;
		TS_BYTE	page_number;
		TS_WORD	sub_page_number;
		TS_BOOL	erase_page_flag;
		TS_BOOL	newsflash_flag;
		TS_BOOL	subtitle_flag;
		TS_BOOL	suppress_header_flag;
		TS_BOOL	update_indicator_flag;
		TS_BOOL	interrupted_sequence_flag;
		TS_BOOL	inhibit_display_flag;
		TS_BOOL	magazine_serial_flag;
		TS_BYTE	language_code;
		TS_BYTE	header_data[32];

		TS_STRING GetHeaderData() const { char char_line[32]; std::copy(header_data, header_data + 32, char_line); return TS_STRING (char_line, 32); }

		unsigned GetPageLineListLength() const { return static_cast< unsigned > ( page_data.size() ); };
		const EbuLine& GetPageLine(unsigned index) const { if (index >= GetPageLineListLength()) return m_missing_line; return page_data[index]; }

	private:		
		EbuLine m_missing_line;
		TS_PRIVATE_EXPORT(std::vector<EbuLine> page_data);
		friend class Packet_Ebu;
	};

	class TS_EXTERN Packet_Ebu : public Packet   // EBU Teletext
	{
	protected:
		TS_VOID ResetTable();
		TS_VOID CreateTable();
		TS_VOID DestroyTable();

	private:
		// prevent copy and assignment
		Packet_Ebu( const Packet_Ebu& other );
		Packet_Ebu& operator= (const Packet_Ebu& rhs);
		
		struct PesPacket {
			TS_PBYTE	buffer;
			TS_BYTE		length;
		};		
		TS_WORD m_pes_packet_length;

		// helper
		TS_BYTE	DeHamming84(TS_BYTE byte_value) const;	
		TS_BYTE	DeHamming84W(TS_BYTE lsb, TS_BYTE msb, TS_BOOL& test) const;
		TS_BYTE	DeHamming84W(TS_BYTE lsb, TS_BYTE msb) const;
		TS_BOOL	CheckParity(TS_BYTE test_value) const;
		TS_VOID	DeParity(TS_PBYTE pbuf, unsigned in_len) const;
		TS_VOID	ReverseBits (TS_PBYTE pbuf, unsigned in_len) const;

		TS_BOOL	FilterPes(TS_PBYTE pbuf, unsigned in_len);
		TS_BOOL	FilterDataField(TS_PBYTE pbuf, unsigned in_len);
		TS_BOOL	FilterEbuPacket(TS_BYTE magazine, TS_BYTE packet, TS_PBYTE pbuf); // data length: 40 Byte!
				
		EbuPage* m_current_page;
		EbuPage m_missing_page;
		TS_BYTE m_current_time_page[8];
		TS_BYTE m_continuity_counter;

		TS_CALLBACK m_callback;
		TS_PVOID m_callback_data;

		TS_PRIVATE_EXPORT(std::vector<PesPacket> m_pes_packet_list);
		TS_PRIVATE_EXPORT( (std::map<TS_DWORD, EbuPage*> m_ebu_pages) );

	public:
		Packet_Ebu();
		virtual ~Packet_Ebu();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);
		
		// get information
		unsigned		GetEbuPageListLength() const;
		const EbuPage&	GetEbuPage(unsigned index) const;
		const EbuPage&	GetEbuPage(TS_BYTE magazine, TS_BYTE page_number, TS_WORD sub_page_number) const;
		TS_STRING		GetCurrentTimeHeader() const;

		TS_VOID			SetNewPageCallback(TS_CALLBACK callback, TS_PVOID data);

	};

}

#endif // __TSSI_PACKET_EBU_H_INCLUDED__