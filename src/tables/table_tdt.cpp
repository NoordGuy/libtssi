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

#include "tables/table_tdt.h"
#include "utils/parsemac2.h"

tssi::Table_Tdt::Table_Tdt() : Table()
{
	m_ts_time = TS_TIME_EMPTY;
	m_snapshot_time = 0;
}

tssi::Table_Tdt::~Table_Tdt()
{
	Table::~Table();
}

TS_VOID tssi::Table_Tdt::Reset()
{
	Table::Reset();
	m_ts_time = TS_TIME_EMPTY;
	m_snapshot_time = 0;
}

TS_BOOL	tssi::Table_Tdt::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (in_len < 8) return TS_FALSE;
	if (pbuf[0] != 0x70)
		return TS_FALSE;	// invalid table ID
	
	TS_BOOL section_syntax_indicator = PSI_SECTION_SYNTAX_INDICATOR_BIT(pbuf);
	TS_WORD section_length = PSI_SECTION_LENGTH_VALUE(pbuf);

	if (section_syntax_indicator != TS_FALSE)
		return TS_FALSE;

	if (section_length < 5)
		return TS_FALSE;

	//TS_QWORD utc_time = NTOH_LL(QWORD_VALUE(pbuf, 0)) & 0x000000ffffffffff; // 40bit
		
	// time is coded in 5 Bytes pPacket[3]-[7]
	int mjd = (pbuf[3] << 8 ) | pbuf[4];
	// see Annex C, EN 300 468 V1.5.1
	int y, m, d, k, m1, y1;

	int sec =  (pbuf[7] & 0xf) + ((pbuf[7] >> 4) & 0xf) *10;
	int min =  (pbuf[6] & 0xf) + ((pbuf[6] >> 4) & 0xf) *10;
	int hour = (pbuf[5] & 0xf) + ((pbuf[5] >> 4) & 0xf) *10;

	int weekday = ((mjd + 2) % 7 ) +1;
	if (weekday == 7) weekday = 0;

	y1 = static_cast<int> (( mjd - 15078.2f ) / 365.25f);
	m1 = static_cast<int> ( ( mjd - 14956.1f - static_cast<int> (y1 * 365.25f) ) / 30.6001f );
	d = mjd - 14956 - static_cast<int> (y1 * 365.25f) - static_cast<int> (m1 * 30.6001f );
	if ( ( m1 == 14 ) || ( m1 == 15 ) ) 
		k = 1; 
	else 
		k = 0;
	y = y1 + k;
	m = m1 - 1 - k * 12;

	y += 1900;

	m_ts_time.day =				static_cast<TS_WORD> (d);
	m_ts_time.day_of_week =		static_cast<TS_WORD> (weekday);
	m_ts_time.hour =			static_cast<TS_WORD> (hour);
	m_ts_time.milliseconds =	0;
	m_ts_time.minute =			static_cast<TS_WORD> (min);
	m_ts_time.month =			static_cast<TS_WORD> (m);
	m_ts_time.second =			static_cast<TS_WORD> (sec);
	m_ts_time.year =			static_cast<TS_WORD> (y);

	m_snapshot_time = time(NULL);
	
	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
}

TS_TIME tssi::Table_Tdt::GetTime() const
{
	return m_ts_time;
}

time_t tssi::Table_Tdt::GetSnapshotTime() const
{
	return m_snapshot_time;
}

