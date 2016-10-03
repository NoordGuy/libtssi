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

#include "tables/table_dsmcc.h"
#include "utils/parsemac2.h"

tssi::Table_Dsmcc::Table_Dsmcc() : Table()
{
}

tssi::Table_Dsmcc::~Table_Dsmcc()
{
	Table::~Table();

}

TS_VOID tssi::Table_Dsmcc::Reset()
{
	Table::Reset();

	m_transactions.clear();
	m_parser.Reset();
}

TS_BOOL	tssi::Table_Dsmcc::Process(TS_PBYTE pbuf, unsigned in_len)
{
	m_table_id = PSI_TABLE_ID_VALUE(pbuf);
	
	TS_BOOL current_next_indicator = PSI_CURRENT_NEXT_INDICATOR_BIT(pbuf);
	if (current_next_indicator == TS_FALSE)
		return TS_FALSE; // only process applicable tables
		
	TS_BOOL section_syntax_indicator = PSI_SECTION_SYNTAX_INDICATOR_BIT(pbuf);
	if (section_syntax_indicator) {
		if (!CheckCrc32(pbuf, in_len)) 
			return TS_FALSE;
	}
	
	pbuf += 8;

	if (m_table_id == 0x3b) { // User Network Message (DSI, DII)
		// no version management, version is always 0!

		TS_WORD message_id = NTOH_S(WORD_VALUE(pbuf, 2));
		TS_DWORD transaction_id = NTOH_L(DWORD_VALUE(pbuf, 4));

		switch (message_id) {
		case 0x1002: // DII (Download Info Indication)
			{
				std::list<DsmccTransaction>::const_iterator iter;
				for (iter = m_transactions.begin(); iter != m_transactions.end(); ++iter) {
					if (iter->transaction_id == transaction_id)
						return TS_TRUE; // this transaction is already processed
				}

				for (iter = m_transactions.begin(); iter != m_transactions.end(); ++iter) {
					if (((iter->transaction_id & 0xfffe)     == (transaction_id & 0xfffe)) && // same id
						((iter->transaction_id & 0x3fff0000) != (transaction_id & 0x3fff0000)) ) { // different version
						m_transactions.erase(iter); // this item is getting updated
						break;
					}
				}

				// jump to the 1st byte after the dsmcc message header
				TS_BYTE adaptation_length = BYTE_VALUE(pbuf, 9);
				pbuf += 12;
				pbuf += adaptation_length;

				DsmccTransaction transaction;
				transaction.transaction_id = transaction_id;
				transaction.download_id = NTOH_L(DWORD_VALUE(pbuf, 0));
				transaction.block_size = NTOH_S(WORD_VALUE(pbuf, 4));

				TS_BYTE compatibility_desc_length = BYTE_VALUE(pbuf, 17);
				pbuf += 18 + compatibility_desc_length;

				TS_WORD module_count = NTOH_S(WORD_VALUE(pbuf, 0));
				pbuf += 2;

				unsigned x;
				for (x = 0; x < module_count; ++x) {
					DiiModule module;
					if (! module.Process(pbuf, 0) )
						return TS_FALSE;

					transaction.module_list.push_back(module);

					pbuf += 8 + module.GetModuleInfoLength();
				}
			
				m_transactions.push_back(transaction);

				if (! m_parser.ProcessDii(transaction) )
					return TS_FALSE;
			}
			break;
		case 0x1005: // DCM (Download Control Messages)
			break;
		case 0x1006: // DSI (Download Server Initiate)
			break;
		}


	}  else if (m_table_id == 0x3c) {	//  Download Data Block

		// version checking for data blocks within module.h		

		DsmccDataBlock data_block;

		data_block.download_id = NTOH_L(DWORD_VALUE(pbuf,4));
		TS_WORD message_length = NTOH_S(WORD_VALUE(pbuf,10));

		TS_BYTE adaptation_length = BYTE_VALUE(pbuf, 9);

		pbuf += 12 + adaptation_length;

		data_block.module_id = NTOH_S(WORD_VALUE(pbuf,0));
		data_block.module_version = BYTE_VALUE(pbuf,2);
		data_block.block_number = NTOH_S(WORD_VALUE(pbuf,4));		

		//	block_data_length = message_length - 6;

		TS_WORD block_data_length = 0;
		
		if (message_length > 6)
			block_data_length = message_length - 6;

		pbuf += 6;

		if (block_data_length > 0) {
			if (! m_parser.ProcessDdb(data_block, pbuf, block_data_length) )
				return TS_FALSE;
		}

	}

	if (m_callback)
		m_callback(m_callback_data);
	return TS_TRUE;
	
}

unsigned tssi::Table_Dsmcc::GetDownloadListLength() const
{
	return m_parser.GetDownloadListLength();
}

TS_BOOL tssi::Table_Dsmcc::IsDownloadComplete(unsigned index) const
{
	return m_parser.IsDownloadComplete(index);
}

TS_BOOL tssi::Table_Dsmcc::ProcessDownload(unsigned index) 
{
	return m_parser.ProcessDownload(index);
}

TS_BOOL tssi::Table_Dsmcc::Decode(const TS_STRING& host_directory) 
{
	return m_parser.Decode(host_directory);
}
