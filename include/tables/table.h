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

#ifndef __TSSI_TABLE_H_INCLUDED__
#define __TSSI_TABLE_H_INCLUDED__

#include "utils/tstypes.h"
#include <vector>

namespace tssi {

	class TS_EXTERN Table {

	protected:
		TS_BYTE	m_table_id;
		TS_BYTE m_section_number;
		TS_BYTE m_last_section_number;
		TS_WORD	m_section_length;

		TS_CALLBACK m_callback;
		TS_PVOID m_callback_data;

		struct SectionVersion {
			TS_BYTE	table_id;
			TS_WORD	table_id_ext;
			TS_BYTE section_number;
			TS_WORD identification_value;
			TS_BYTE	current_version;
		};
		TS_PRIVATE_EXPORT( std::vector<SectionVersion> m_section_list );

		TS_BOOL	CheckCrc32(TS_PBYTE pbuf, unsigned in_len);
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len, TS_BOOL extended_table, TS_BOOL &new_version); 
		// extended_table: including additional identification value after last_section_number (see eit and sdt)
		// new_version: is this a table not seen before?
		
	public:
		Table();
		virtual ~Table();

		virtual TS_VOID Reset();
		virtual TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		TS_VOID SetProcessCallback(TS_CALLBACK callback, TS_PVOID data);
		
	
	};

}

#endif // __TSSI_TABLE_H_INCLUDED__