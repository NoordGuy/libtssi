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

#ifndef __TSSI_TABLE_PMT_H_INCLUDED__
#define __TSSI_TABLE_PMT_H_INCLUDED__

#include "tables/table.h"
#include "descriptors/descriptorlist.h"
#include <map>

namespace tssi {

	class TS_EXTERN Table_Pmt: public Table {
		struct EsInformation {
			TS_BYTE			stream_type;
			TS_WORD			elementary_pid;
			DescriptorList	descriptor_list;
		};

		struct PmtInformation {
			TS_WORD							pcr_pid;			
			std::vector<EsInformation>		es_information;
		};

		TS_PRIVATE_EXPORT(std::map<TS_WORD, PmtInformation> m_pmt); // first: program_number

		DescriptorList m_missing_descriptor_list;

	public:
		Table_Pmt();
		virtual ~Table_Pmt();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		unsigned				GetProgramListLength() const;
		TS_WORD					GetProgramNumber(unsigned index) const;
		TS_WORD					GetPcrPid(TS_WORD program) const;
		unsigned				GetEsListLength(TS_WORD program) const;
		TS_BYTE					GetEsType(TS_WORD program, unsigned index_stream) const;
		TS_WORD					GetEsPid(TS_WORD program, unsigned index_stream) const;
		const DescriptorList&	GetEsDescriptorList(TS_WORD program, unsigned index_stream) const;


	};

}

#endif // __TSSI_TABLE_PMT_H_INCLUDED__
