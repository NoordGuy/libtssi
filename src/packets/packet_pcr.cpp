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

#include "packets/packet_pcr.h"
#include "utils/parsemac2.h"

tssi::Packet_Pcr::Packet_Pcr() 
{
	m_table = 0;
	m_pcr = 0;

	m_callback = 0;
}

tssi::Packet_Pcr::~Packet_Pcr()
{
}

TS_VOID tssi::Packet_Pcr::ResetTable()
{
}

TS_VOID tssi::Packet_Pcr::CreateTable()
{
}

TS_VOID tssi::Packet_Pcr::DestroyTable()
{
}

TS_VOID	tssi::Packet_Pcr::Reset()
{
	m_pcr = 0;

	m_callback = 0;
}

TS_BOOL	tssi::Packet_Pcr::Process(TS_PBYTE pbuf, unsigned in_len)
{

	// Is there a payload? -> adaptation_field_control
	TS_BYTE adaptation_field_control = ADAPTATION_FIELD_CONTROL_VALUE(pbuf);
	if ((adaptation_field_control==0x00) || (adaptation_field_control==0x01)) return TS_TRUE; // no adaptation field

	pbuf += 4;

	if (ADAPTATION_FIELD_LENGTH_VALUE(pbuf) == 0) return TS_TRUE; // adaptation field is empty

	TS_BOOL pcr_flag = PCR_FLAG_BIT(pbuf);

	if (!pcr_flag) return TS_TRUE; // no pcr data

	pbuf += 2;

	TS_QWORD pcr_base = PCR_BASE_VALUE(pbuf);
	TS_DWORD pcr_ext = PCR_EXT_VALUE(pbuf);

	TS_QWORD pcr = pcr_base * 300 + pcr_ext;
	m_pcr = static_cast<TS_DOUBLE> ( pcr ) / 27000000.;

	if (m_callback)
		m_callback(m_callback_data);
	
	return TS_TRUE;
}

TS_DOUBLE tssi::Packet_Pcr::GetPcr() const
{
	return m_pcr;
}

TS_VOID tssi::Packet_Pcr::SetPcrCallback(TS_CALLBACK callback, TS_PVOID data)
{
	m_callback = callback;
	m_callback_data = data;
}


