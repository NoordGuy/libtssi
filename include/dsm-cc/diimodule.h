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

#ifndef __TSSI_DIIMODULE_H_INCLUDED__
#define __TSSI_DIIMODULE_H_INCLUDED__

#include "descriptors/descriptorlist.h"

namespace tssi {

	class DiiModule {

		TS_WORD			m_module_id;
		TS_DWORD		m_module_size;
		TS_BYTE			m_module_version;
		TS_BYTE			m_module_info_length;

		TS_DWORD		m_module_time_out;
		TS_DWORD		m_block_time_out;
		TS_DWORD		m_min_block_time;
		DescriptorList	m_descriptor_list;
		
	public:
		DiiModule();
		virtual ~DiiModule();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		TS_WORD					GetModuleId() const;
		TS_DWORD				GetModuleSize() const;
		TS_BYTE					GetModuleVersion() const;
		TS_BYTE					GetModuleInfoLength() const;

		TS_DWORD				GetModuleTimeOut() const;
		TS_DWORD				GetBlockTimeOut() const;
		TS_DWORD				GetMinBlockTime() const;
		const DescriptorList&	GetDescriptorList() const;

	};

}

#endif // __TSSI_DIIMODULE_H_INCLUDED__