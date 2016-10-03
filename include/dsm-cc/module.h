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

#ifndef __TSSI_MODULE_H_INCLUDED__
#define __TSSI_MODULE_H_INCLUDED__

#include "dsm-cc/diimodule.h"
#include <vector>

namespace tssi {

	struct DsmccDataBlock {
		TS_DWORD	download_id;
		TS_WORD		module_id;
		TS_BYTE		module_version;
		TS_WORD		block_number;
	};

	struct DsmccTransaction {
		TS_DWORD				transaction_id;
		TS_DWORD				download_id;
		TS_WORD					block_size;
		std::vector<DiiModule>	module_list;
	};

	class Module {

		TS_WORD					m_module_id;
		TS_DWORD				m_module_size;
		TS_BYTE					m_module_version;

		TS_BYTE					m_compression;
		TS_DWORD				m_original_size;
		TS_BOOL					m_extracted;

		TS_WORD					m_block_size;
		TS_PBYTE				m_buffer;
		std::vector<TS_BOOL>	m_buffer_available;

		// prevent copy and assignment
		Module(const Module&);                 
		Module& operator=(const Module&);  

	public:
		Module(const DiiModule& message_desciption, TS_WORD block_size);
		virtual ~Module();

		TS_VOID	Reset();
		TS_BOOL	ProcessDdb(const DsmccDataBlock& data_block, TS_PBYTE pbuf, unsigned in_len);

		TS_BOOL AutoExtract();

		// get information
		TS_WORD		GetModuleId() const;
		TS_DWORD	GetModuleSize() const;
		TS_BYTE		GetModuleVersion() const;
		TS_BYTE		GetCompression() const;
		TS_DWORD	GetOriginalSize() const;
		TS_BOOL		IsBufferComplete() const;
		TS_BOOL		IsExtracted() const;

		const TS_PBYTE	GetBuffer() const;
	};
}

#endif // __TSSI_MODULE_H_INCLUDED__