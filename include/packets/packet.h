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

#ifndef __TSSI_PACKET_H_INCLUDED__
#define __TSSI_PACKET_H_INCLUDED__

#include "tables/table.h"

namespace tssi {

	class TS_EXTERN Packet {
		TS_PBYTE	m_cache;
		TS_WORD		m_cache_length;
		TS_WORD		m_cache_pos;

	protected:
		Table*		m_table;

		virtual TS_VOID ResetTable() = 0;
		virtual TS_VOID CreateTable() = 0;
		virtual TS_VOID DestroyTable() = 0;

		TS_VOID Create();
		TS_VOID Destroy();

	public:
		Packet();
		virtual ~Packet();

		virtual TS_VOID	Reset();
		virtual TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		Table* GetTable() const;

	};

}

#endif // __TSSI_PACKET_H_INCLUDED__