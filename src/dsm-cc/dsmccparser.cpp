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

#include "dsm-cc/dsmccparser.h"

tssi::DsmccParser::DsmccParser()
{
}

tssi::DsmccParser::DsmccParser(const tssi::DsmccParser&)
{
}

tssi::DsmccParser& tssi::DsmccParser::operator=(const tssi::DsmccParser&)
{
	return *this;
}

tssi::DsmccParser::~DsmccParser()
{
	unsigned x, y;
	for (x = 0; x < m_download_list.size(); ++x) {
		for (y = 0; y < m_download_list[x].module_list.size(); ++y) {
			delete m_download_list[x].module_list[y];
		}
		m_download_list[x].module_list.clear();
	}
}

TS_VOID tssi::DsmccParser::Reset()
{
	unsigned x, y;
	for (x = 0; x < m_download_list.size(); ++x) {
		for (y = 0; y < m_download_list[x].module_list.size(); ++y) {
			delete m_download_list[x].module_list[y];
		}
		m_download_list[x].module_list.clear();
	}
	m_download_list.clear();
	m_decoder.Reset();
}

TS_BOOL tssi::DsmccParser::ProcessDii(const DsmccTransaction& transaction)
{
	unsigned x, y;
	for (x = 0; x < m_download_list.size(); ++x) {
		if (((m_download_list[x].transaction_id & 0xfffe)     == (transaction.transaction_id & 0xfffe)) && // same id
			((m_download_list[x].transaction_id & 0x3fff0000) != (transaction.transaction_id & 0x3fff0000)) ) { // different version
			// transaction update -> delete download
			for (y = 0; y < m_download_list[x].module_list.size(); ++y) 
				delete m_download_list[x].module_list[y];
			m_download_list[x].module_list.clear();
			m_download_list.erase( m_download_list.begin() + x);
			break;
		}
	}

	// add download
	DsmccDownload download;
	download.transaction_id = transaction.transaction_id;
	download.download_id = transaction.download_id;
	for (x = 0; x < transaction.module_list.size(); ++x) {
		Module* module = new Module( transaction.module_list[x], transaction.block_size );
		download.module_list.push_back(module);
	}

	m_download_list.push_back(download);

	return TS_TRUE;
}

TS_BOOL tssi::DsmccParser::ProcessDdb(const DsmccDataBlock& data_block, TS_PBYTE pbuf, unsigned in_len)
{
	unsigned x, y;
	for (x = 0; x < m_download_list.size(); ++x) {
		if (m_download_list[x].download_id == data_block.download_id) {
			for (y = 0; y < m_download_list[x].module_list.size(); ++y) {
				if ((m_download_list[x].module_list[y]->GetModuleId() == data_block.module_id) &&
					(m_download_list[x].module_list[y]->GetModuleVersion() == data_block.module_version))
					return m_download_list[x].module_list[y]->ProcessDdb( data_block, pbuf, in_len );
			}			
		}
	}

	return TS_TRUE;
}

unsigned tssi::DsmccParser::GetDownloadListLength() const
{
	return static_cast<unsigned> ( m_download_list.size() );
}

TS_BOOL tssi::DsmccParser::IsDownloadComplete(unsigned index) const
{
	if (index >= GetDownloadListLength() ) return TS_FALSE;

	unsigned x;
	for (x = 0; x < m_download_list[index].module_list.size(); ++x) {
		if (! m_download_list[index].module_list[x]->IsBufferComplete() )
			return TS_FALSE;
	}

	return TS_TRUE;
}

TS_BOOL tssi::DsmccParser::ProcessDownload(unsigned index) 
{
	if (index >= GetDownloadListLength() ) return TS_FALSE;
	return m_decoder.Process( m_download_list[index] );
}

TS_BOOL tssi::DsmccParser::Decode(TS_STRING host_directory) 
{
	return m_decoder.Decode( host_directory );
}

