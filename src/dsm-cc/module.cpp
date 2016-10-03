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

#include "dsm-cc/module.h"
#include "descriptors/descriptor_compression.h"
#include "utils/parsemac2.h"

extern "C" {
	#include "zlib.h"
}

tssi::Module::Module(const Module&) {
}

tssi::Module& tssi::Module::operator=(const tssi::Module&)
{
	return *this;
}

tssi::Module::Module(const DiiModule& message_desciption, TS_WORD block_size)
{
	m_module_id = message_desciption.GetModuleId();
	m_module_size = message_desciption.GetModuleSize();
	m_module_version = message_desciption.GetModuleVersion();

	m_extracted = TS_FALSE;

	if (block_size > 0) {
		m_block_size = block_size;

		m_buffer = new TS_BYTE[m_module_size];
		TS_DWORD buffer_segments = m_module_size / block_size + 
			(m_module_size % block_size == 0 ? 0 : 1); // closing block

		m_buffer_available.resize( buffer_segments );
		for (unsigned x = 0; x < buffer_segments; ++x) m_buffer_available[x] = TS_FALSE;

		Descriptor_Compression* descriptor_compression = dynamic_cast<Descriptor_Compression*> 
			( message_desciption.GetDescriptorList().GetDescriptorByTag( Descriptor_Compression::kDescriptorTag ) );
		if (descriptor_compression) {
			m_compression = descriptor_compression->GetCompressionSystem();
			m_original_size = descriptor_compression->GetOriginalSize();
		} else {
			m_compression = 0;
			m_original_size = m_module_size;
		}
	} else { // block_size == 0
		m_buffer = 0;
	}
}

tssi::Module::~Module() 
{
	if (m_buffer)
		delete[] m_buffer;
}

TS_VOID tssi::Module::Reset() 
{
	m_module_id = 0;
	m_module_size = 0;
	m_module_version = 0;

	m_compression = 0;
	m_original_size = 0;
	m_block_size = 0;
	m_extracted = TS_FALSE;

	if (m_buffer) {
		delete[] m_buffer;
		m_buffer = 0;
	}
	m_buffer_available.clear();

}

TS_BOOL	tssi::Module::ProcessDdb(const DsmccDataBlock& data_block, TS_PBYTE pbuf, unsigned in_len)
{
	if (!m_buffer) 
		return TS_FALSE;
	if (m_block_size == 0)
		return TS_FALSE;
	if (m_extracted)
		return TS_TRUE;

	TS_BOOL residual_block = (m_module_size % m_block_size) != 0;
	TS_DWORD buffer_segments = m_module_size / m_block_size;
	if (residual_block) ++buffer_segments;

	if (data_block.block_number >= buffer_segments)
		return TS_FALSE;

	if (!residual_block) {
		if (in_len != m_block_size)
			return TS_FALSE;
	} else { // (residual_block)
		if ( (in_len != m_block_size) && (data_block.block_number < buffer_segments - 1) )
			return TS_FALSE;
	}

	if (data_block.block_number * m_block_size + in_len > m_module_size)
		return TS_FALSE;

	memcpy( m_buffer + data_block.block_number * m_block_size, pbuf, in_len );
	m_buffer_available[data_block.block_number] = TS_TRUE;

	return TS_TRUE;
}

TS_BOOL tssi::Module::AutoExtract()
{
	if (m_extracted)
		return TS_TRUE;

	if (! IsBufferComplete() )
		return TS_FALSE;

	// unpack 
	if (m_compression) {

			
		TS_PBYTE pbuf_uncompressed = new TS_BYTE[ m_original_size ];
		TS_DWORD dest_length = m_original_size;

		if ( uncompress( pbuf_uncompressed, &dest_length, m_buffer, m_module_size ) != Z_OK ) {
			delete[] pbuf_uncompressed;
			return TS_FALSE;
		}

		if (dest_length != m_original_size) {
			delete[] pbuf_uncompressed;
			return TS_FALSE;
		}

		delete[] m_buffer;
		m_buffer = pbuf_uncompressed;
		m_extracted = TS_TRUE;
		m_module_size = dest_length;

	}
	return TS_TRUE;
}

TS_WORD	tssi::Module::GetModuleId() const
{
	return m_module_id;
}

TS_DWORD tssi::Module::GetModuleSize() const
{
	return m_module_size;
}

TS_BYTE tssi::Module::GetModuleVersion() const
{
	return m_module_version;
}

TS_BYTE	tssi::Module::GetCompression() const
{
	return m_compression;
}

TS_DWORD tssi::Module::GetOriginalSize() const
{
	return m_original_size;
}

TS_BOOL tssi::Module::IsBufferComplete() const
{
	if (m_extracted)
		return TS_TRUE;

	if (!m_buffer) 
		return TS_FALSE;
	if (m_block_size == 0)
		return TS_FALSE;
	if (m_buffer_available.size() == 0)
		return TS_FALSE;

	unsigned x;
	for (x = 0; x < m_buffer_available.size(); ++x)
		if (! m_buffer_available[x] )
			return TS_FALSE;

	return TS_TRUE;
}

TS_BOOL tssi::Module::IsExtracted() const
{
	return m_extracted;
}

const TS_PBYTE tssi::Module::GetBuffer() const
{
	return m_buffer;
}