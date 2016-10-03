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

#include "dsm-cc/diimodule.h"
#include "utils/parsemac2.h"

tssi::DiiModule::DiiModule() 
{
	m_module_id = 0;
	m_module_size = 0;
	m_module_version = 0;
	m_module_info_length = 0;

	m_module_time_out = 0;
	m_block_time_out = 0;
	m_min_block_time = 0;
}

tssi::DiiModule::~DiiModule() 
{
}

TS_VOID tssi::DiiModule::Reset() 
{	
	m_module_id = 0;
	m_module_size = 0;
	m_module_version = 0;
	m_module_info_length = 0;

	m_module_time_out = 0;
	m_block_time_out = 0;
	m_min_block_time = 0;
	m_descriptor_list.Reset();
}

TS_BOOL	tssi::DiiModule::Process(TS_PBYTE pbuf, unsigned in_len)
{
	Reset();

	unsigned residual_length = in_len;

	m_module_id = NTOH_S(WORD_VALUE(pbuf,0));
	m_module_size = NTOH_L(DWORD_VALUE(pbuf,2));
	m_module_version = BYTE_VALUE(pbuf,6);
	m_module_info_length = BYTE_VALUE(pbuf,7);		
	
	pbuf += 8;
	residual_length -= 8;
		
	if (m_module_info_length > 0) {
		if (residual_length < 14)
			return TS_FALSE;

		m_module_time_out = NTOH_L(DWORD_VALUE(pbuf, 0));
		m_block_time_out = NTOH_L(DWORD_VALUE(pbuf, 4));
		m_min_block_time = NTOH_L(DWORD_VALUE(pbuf, 8));

		TS_BYTE taps_count = BYTE_VALUE(pbuf, 12);

		pbuf += 13;
		pbuf += taps_count * 7;

		TS_BYTE user_info_length = BYTE_VALUE(pbuf, 0);
		++pbuf;
		
		if (user_info_length > 0)
			return m_descriptor_list.Process(pbuf, user_info_length);
		else
			return TS_TRUE;
		
	} // m_module_info_length > 0

	return TS_TRUE;
}

TS_WORD	tssi::DiiModule::GetModuleId() const
{
	return m_module_id;
}

TS_DWORD tssi::DiiModule::GetModuleSize() const
{
	return m_module_size;
}

TS_BYTE tssi::DiiModule::GetModuleVersion() const
{
	return m_module_version;
}

TS_BYTE	tssi::DiiModule::GetModuleInfoLength() const
{
	return m_module_info_length;
}

TS_DWORD tssi::DiiModule::GetModuleTimeOut() const
{
	return m_module_time_out;
}

TS_DWORD tssi::DiiModule::GetBlockTimeOut() const
{
	return m_block_time_out;
}

TS_DWORD tssi::DiiModule::GetMinBlockTime() const
{
	return m_min_block_time;
}

const tssi::DescriptorList& tssi::DiiModule::GetDescriptorList() const
{
	return m_descriptor_list;
}


