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

#ifndef __TSSI_BINDING_H_INCLUDED__
#define __TSSI_BINDING_H_INCLUDED__

#include "utils/tstypes.h"

namespace tssi {

	class Binding {
		TS_STRING	m_id_data;
		TS_DWORD	m_kind_data;
		TS_DWORD	m_type_id;
		TS_DWORD	m_object_key;

		TS_DWORD	m_total_length;


	public:
		static const TS_DWORD kTagBiop			= 0x49534F06;
		static const TS_DWORD kTagLiteOptions	= 0x49534F05;

		Binding();
		virtual ~Binding();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len); // in_len 0: allowed
				
		TS_STRING	GetIdData() const;
		TS_DWORD	GetKindData() const;
		TS_DWORD	GetTypeId() const;
		TS_DWORD	GetObjectKey() const;
		TS_DWORD	GetTotalLength() const;

	};

}


#endif // __TSSI_BINDING_H_INCLUDED__