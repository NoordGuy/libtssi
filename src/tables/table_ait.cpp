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

#include "tables/table_ait.h"
#include "utils/parsemac2.h"

tssi::Table_Ait::Table_Ait() : Table()
{
	m_test_application_flag = 0;
	m_application_type = 0;
	m_version_number = 0xff;
}

tssi::Table_Ait::~Table_Ait()
{
	Table::~Table();

}

TS_VOID tssi::Table_Ait::Reset()
{
	Table::Reset();
	m_test_application_flag = 0;
	m_application_type = 0;
	m_version_number = 0xff;
	m_descriptor_list.Reset();
	
	m_application_list.clear();
}

TS_BOOL tssi::Table_Ait::Process(TS_PBYTE pbuf, unsigned in_len)
{	
	TS_BOOL new_version = TS_FALSE;
	if (!Table::Process(pbuf, in_len, TS_FALSE, new_version)) 
		return TS_FALSE;
	if (!new_version)
		return TS_TRUE;
	if (m_table_id != 0x74) 
		return TS_FALSE;

	m_test_application_flag = AIT_TEST_APPLICATION_BIT(pbuf);
	m_application_type = AIT_APPLICATION_TYPE_VALUE(pbuf);

	TS_WORD desc_length = AIT_COMMON_DESCRIPTORS_LENGTH_VALUE(pbuf);

	TS_BYTE version_number = AIT_VERSION_NUMBER_VALUE(pbuf);
	if (m_version_number != version_number) {
		// new version, new descriptors
		m_descriptor_list.Reset();
		m_application_list.clear();
		m_version_number = version_number;
	}
	
	pbuf += 10;
	m_descriptor_list.Process(pbuf, desc_length);
	pbuf += desc_length;

	unsigned loop_length = (NTOH_S(WORD_VALUE(pbuf,0)) & 0x0fff);
	pbuf += 2;

	while (loop_length > 0) {
		AitApplication application;
		application.organisation_id = AIT_ORGANISATION_ID_VALUE(pbuf);
		application.application_id = AIT_APPLICATION_ID_VALUE(pbuf);
		application.application_control_code = AIT_APPLICATION_CONTROL_CODE_VALUE(pbuf);

		TS_WORD desc_len = AIT_APPLICATION_DESCRIPTORS_LENGTH_VALUE(pbuf);

		pbuf += 9;
		loop_length -= 9;		

		// is a similar item stored?
		unsigned x;
		for (x = 0; x < m_application_list.size(); ++x) {
			if ((m_application_list[x].organisation_id == application.organisation_id) &&
				(m_application_list[x].application_id == application.application_id)) {
					if (! m_application_list[x].application_descriptor_list.Process(pbuf, desc_len) )
						return TS_FALSE;
					break;
			}
		}

		// not found? add
		if (x == m_application_list.size()) {
			if (! application.application_descriptor_list.Process(pbuf, desc_len) )
				return TS_FALSE;
			m_application_list.push_back(application);	
		}

		pbuf += loop_length;
		loop_length -= loop_length;
			
	}

	if (m_callback)
		m_callback( m_callback_data );
	return TS_TRUE;
}

TS_BOOL	tssi::Table_Ait::GetTestApplicationFlag() const
{
	return m_test_application_flag;
}

TS_WORD	tssi::Table_Ait::GetApplicationType() const
{
	return m_application_type;
}

const tssi::DescriptorList& tssi::Table_Ait::GetCommonDescriptorList() const
{
	return m_descriptor_list;
}

unsigned tssi::Table_Ait::GetApplicationListLength() const
{
	return static_cast<unsigned> ( m_application_list.size() );
}

TS_DWORD tssi::Table_Ait::GetOrganisationId(unsigned index) const
{
	if (index >= GetApplicationListLength()) return 0;
	return m_application_list[index].organisation_id;
}

TS_WORD	tssi::Table_Ait::GetApplicationId(unsigned index) const
{
	if (index >= GetApplicationListLength()) return 0;
	return m_application_list[index].application_id;
}

TS_BYTE	tssi::Table_Ait::GetApplicationControlCode(unsigned index) const
{
	if (index >= GetApplicationListLength()) return 0;
	return m_application_list[index].application_control_code;
}

const tssi::DescriptorList& tssi::Table_Ait::GetApplicationDescriptorList(unsigned index) const
{
	if (index >= GetApplicationListLength()) return m_missing_descriptor_list;
	return m_application_list[index].application_descriptor_list;
}
