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

#ifndef __TSSI_BIOPDECODER_H_INCLUDED__
#define __TSSI_BIOPDECODER_H_INCLUDED__

#include "dsm-cc/biopmessage_directory.h"
#include "dsm-cc/biopmessage_file.h"
#include "dsm-cc/module.h"

namespace tssi {

	struct DsmccDownload {
		TS_DWORD				transaction_id;
		TS_DWORD				download_id;
		std::vector<Module*>	module_list;
	};

	class BiopDecoder {
		BiopMessage_Directory*				m_service_gateway;
		std::vector<BiopMessage_File*>		m_file_list;
		std::vector<BiopMessage_Directory*> m_directory_list;

		TS_BOOL ProcessModule(TS_PBYTE pbuf, unsigned in_len);
		TS_BOOL ProcessBinding(Binding binding, TS_STRING directory);

	public:
		BiopDecoder();
		virtual ~BiopDecoder();

		TS_VOID Reset();
		TS_BOOL Process( const DsmccDownload& download );
		TS_BOOL Decode( TS_STRING host_directory );

	};
}

#endif // __TSSI_BIOPDECODER_H_INCLUDED__