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

#ifndef __TSSI_PACKET_NIT_H_INCLUDED__
#define __TSSI_PACKET_NIT_H_INCLUDED__

#include "packets/packet.h"
#include "tables/table_nit.h"

namespace tssi {

	class TS_EXTERN Packet_Nit: public Packet 
	{
	protected:
		TS_VOID ResetTable();
		TS_VOID CreateTable();
		TS_VOID DestroyTable();

	public:
		Packet_Nit();
		virtual ~Packet_Nit();

		const Table_Nit& GetTableNit() const;

	};

}

#endif // __TSSI_PACKET_NIT_H_INCLUDED__


