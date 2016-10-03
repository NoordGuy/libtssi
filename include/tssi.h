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

#ifndef __TSSI_TSSI_H_INCLUDED__
#define __TSSI_TSSI_H_INCLUDED__

#include "utils/tsversion.h"

#include "packets/packet_ait.h"
#include "packets/packet_dsmcc.h"
#include "packets/packet_ebu.h"
#include "packets/packet_eit.h"
#include "packets/packet_nit.h"
#include "packets/packet_pat.h"
#include "packets/packet_pcr.h"
#include "packets/packet_pmt.h"
#include "packets/packet_sdt.h"
#include "packets/packet_tdt.h"

#include "descriptors/descriptor_iso639.h"
#include "descriptors/descriptor_service.h"
#include "descriptors/descriptor_teletext.h"
#include "descriptors/descriptor_compression.h"
#include "descriptors/descriptor_satellitedelivery.h"
#include "descriptors/descriptor_networkName.h"
#include "descriptors/descriptor_shortEvent.h"
#include "descriptors/descriptor_extendedEvent.h"
#include "descriptors/descriptor_content.h"
#include "descriptors/descriptor_component.h"
#include "descriptors/descriptor_pdc.h"
#include "descriptors/descriptor_framerate.h"
#include "descriptors/descriptor_applicationsignalling.h"
#include "descriptors/descriptor_application.h"
#include "descriptors/descriptor_applicationname.h"
#include "descriptors/descriptor_transportprotocol.h"
#include "descriptors/descriptor_dvbjapplication.h"
#include "descriptors/descriptor_dvbjapplicationlocation.h"

namespace tssi {

	class TS_EXTERN Parser {

		unsigned			m_fragment_length;
	    TS_BYTE				m_packet_buffer[188];
		
		volatile TS_WORD	m_pid_dsmcc;
		volatile TS_WORD	m_pid_ait;
		volatile TS_WORD	m_pid_ebu;
		volatile TS_WORD	m_pid_pcr;

		TS_DWORD			m_processed_packets;
		TS_DWORD			m_processing_errors;

		Packet_Ait			m_packet_ait;
		Packet_Dsmcc		m_packet_dsmcc;
		Packet_Ebu			m_packet_ebu;
		Packet_Eit			m_packet_eit;
		Packet_Nit			m_packet_nit;
		Packet_Pat			m_packet_pat;
		Packet_Pcr			m_packet_pcr;
		Packet_Pmt			m_packet_pmt;
		Packet_Sdt			m_packet_sdt;
		Packet_Tdt			m_packet_tdt;

		TS_BOOL	FilterTsPacket(TS_PBYTE pbuf);

	public:
		Parser();
		virtual ~Parser();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		TS_VOID SetPidDsmcc(TS_WORD pid_value);
		TS_VOID SetPidAit(TS_WORD pid_value);
		TS_VOID SetPidPcr(TS_WORD pid_value);
		TS_VOID SetPidEbu(TS_WORD pid_value);

		TS_DWORD ProcessingErrors() const;
		TS_DWORD PacketsProcessed() const;

		const Packet_Ebu& PacketEbu() const;
		const Packet_Pcr& PacketPcr() const;

		const Table_Ait& TableAit() const;
		const Table_Dsmcc& TableDsmcc() const;
		const Table_Eit& TableEit() const;
		const Table_Nit& TableNit() const;
		const Table_Pat& TablePat() const;
		const Table_Pmt& TablePmt() const;
		const Table_Sdt& TableSdt() const;
		const Table_Tdt& TableTdt() const;		
		
	};
    
}

#endif // __TSSI_TSSI_H_INCLUDED__