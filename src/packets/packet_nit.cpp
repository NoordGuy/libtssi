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

#include "packets/packet_nit.h"

tssi::Packet_Nit::Packet_Nit() : Packet()
{
	CreateTable();
}

tssi::Packet_Nit::~Packet_Nit()
{
	Packet::~Packet();
	DestroyTable();
}

TS_VOID tssi::Packet_Nit::ResetTable()
{
	m_table->Reset();
}

TS_VOID tssi::Packet_Nit::CreateTable()
{
	DestroyTable();
	m_table = new Table_Nit();
}

TS_VOID tssi::Packet_Nit::DestroyTable()
{
	if (m_table) delete m_table;
	m_table = 0;
}

const tssi::Table_Nit& tssi::Packet_Nit::GetTableNit() const
{
	return *dynamic_cast<Table_Nit*> (m_table);
}