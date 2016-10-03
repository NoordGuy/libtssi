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

#ifndef __TSSI_TABLE_DSMCC_H_INCLUDED__
#define __TSSI_TABLE_DSMCC_H_INCLUDED__

#include "tables/table.h"
#include "dsm-cc/dsmccparser.h"
#include <list>

namespace tssi {
	
	class TS_EXTERN Table_Dsmcc : public Table {
		
		TS_PRIVATE_EXPORT( std::list<DsmccTransaction> m_transactions );
		TS_PRIVATE_EXPORT( DsmccParser					m_parser );

	public:
		Table_Dsmcc();
		virtual ~Table_Dsmcc();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		unsigned GetDownloadListLength() const;
		TS_BOOL IsDownloadComplete(unsigned index) const;
		TS_BOOL ProcessDownload(unsigned index);
		TS_BOOL Decode(const TS_STRING& host_directory);

	};

}

#endif // __TSSI_TABLE_DSMCC_H_INCLUDED__