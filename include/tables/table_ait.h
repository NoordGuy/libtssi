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

#ifndef __TSSI_TABLE_AIT_H_INCLUDED__
#define __TSSI_TABLE_AIT_H_INCLUDED__

#include "tables/table.h"
#include "descriptors/descriptorlist.h"

namespace tssi {

	class TS_EXTERN Table_Ait : public Table {

		struct AitApplication {
			TS_DWORD		organisation_id;
			TS_WORD			application_id;
			TS_BYTE			application_control_code;
			DescriptorList	application_descriptor_list;
		};

		TS_BOOL			m_test_application_flag;
		TS_WORD			m_application_type;
		TS_BYTE			m_version_number;
		DescriptorList	m_descriptor_list;

		TS_PRIVATE_EXPORT( std::vector<AitApplication> m_application_list );
		DescriptorList m_missing_descriptor_list;

	public:
		Table_Ait();
		virtual ~Table_Ait();

		TS_VOID Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		TS_BOOL					GetTestApplicationFlag() const;
		TS_WORD					GetApplicationType() const;
		const DescriptorList&	GetCommonDescriptorList() const;
		unsigned				GetApplicationListLength() const;
		TS_DWORD				GetOrganisationId(unsigned index) const;
		TS_WORD					GetApplicationId(unsigned index) const;
		TS_BYTE					GetApplicationControlCode(unsigned index) const;
		const DescriptorList&	GetApplicationDescriptorList(unsigned index) const;
		

	};

}

#endif // __TSSI_TABLE_AIT_H_INCLUDED__
