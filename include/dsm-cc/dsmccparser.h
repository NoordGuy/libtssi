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

#ifndef __TSSI_DSMCCPARSER_H_INCLUDED__
#define __TSSI_DSMCCPARSER_H_INCLUDED__

#include "dsm-cc/module.h"
#include "dsm-cc/biopdecoder.h"

namespace tssi {

	class DsmccParser {
		std::vector<DsmccDownload>	m_download_list;
		BiopDecoder					m_decoder;

		// prevent copy and assignment
		DsmccParser(const DsmccParser&);                 
		DsmccParser& operator=(const DsmccParser&);  
		
	public:
		DsmccParser();
		virtual ~DsmccParser();

		TS_VOID Reset();

		TS_BOOL ProcessDii(const DsmccTransaction& transaction);
		TS_BOOL ProcessDdb(const DsmccDataBlock& data_block, TS_PBYTE pbuf, unsigned in_len);

		unsigned GetDownloadListLength() const;
		TS_BOOL IsDownloadComplete(unsigned index) const;
		TS_BOOL ProcessDownload(unsigned index);
		TS_BOOL Decode(TS_STRING host_directory);

	};

}

#endif // __TSSI_DSMCCPARSER_H_INCLUDED__