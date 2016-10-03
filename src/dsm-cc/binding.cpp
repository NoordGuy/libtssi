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

#include "dsm-cc/binding.h"
#include "utils/parsemac2.h"

tssi::Binding::Binding()
{
	m_kind_data = 0;
	m_type_id = 0;
	m_object_key = 0;
	m_total_length = 0;

}

tssi::Binding::~Binding()
{
}

TS_VOID tssi::Binding::Reset()
{
	m_id_data.clear();
	m_kind_data = 0;
	m_type_id = 0;
	m_object_key = 0;
	m_total_length = 0;
}

TS_BOOL	tssi::Binding::Process(TS_PBYTE pbuf, unsigned in_len)
{
	Reset();
	TS_PBYTE pbuf_start = pbuf;

	TS_BYTE name_components_count = BYTE_VALUE(pbuf, 0);	
	++pbuf;
	
	if (name_components_count != 1) 
		return TS_FALSE;

	TS_BYTE id_length = BYTE_VALUE(pbuf, 0);
	++pbuf;
	if (id_length) {
		char* id_data = new char[id_length];
		memcpy(id_data, pbuf, id_length);
		m_id_data = TS_STRING(id_data, id_length - 1); // name
		delete[] id_data;

		pbuf += id_length;
	}
	
	TS_BYTE kind_length = BYTE_VALUE(pbuf, 0);
	++pbuf;
	if (kind_length != 4) 
		return TS_FALSE;

	m_kind_data = NTOH_L(DWORD_VALUE(pbuf, 0)); // "fil", "dir", "str", "srg", "ste"
	pbuf += 4;
	
	++pbuf; // binding_type
	
	// IOP::IOR() parsing

	TS_DWORD type_id_length = NTOH_L(DWORD_VALUE(pbuf, 0));
	pbuf += 4;
	if (type_id_length != 4)
		return TS_FALSE;
	m_type_id = NTOH_L(DWORD_VALUE(pbuf, 0));	// "dir", "fil", "str", "srg", "ste"
	pbuf += 4;

	// no alignment gap ( type_id_length % 4 = 0 )
	
	TS_DWORD tagged_profiles_count = NTOH_L(DWORD_VALUE(pbuf, 0));
	pbuf += 4;
	while (tagged_profiles_count > 0) {
		TS_DWORD profile_id_tag = NTOH_L(DWORD_VALUE(pbuf, 0));
		TS_DWORD profile_data_length = NTOH_L(DWORD_VALUE(pbuf, 4));

		switch (profile_id_tag) {
		case kTagBiop: 
			{
				TS_PBYTE pbuf2 = pbuf + 23;
				TS_BYTE object_key_length = BYTE_VALUE(pbuf2, 0);
				++pbuf2;
				if (object_key_length > 0) {
					m_object_key = NTOH_L(DWORD_VALUE(pbuf2, 0));
					if (object_key_length == 1)
						m_object_key = m_object_key & 0x000000ff;
					else if (object_key_length == 2)
						m_object_key = m_object_key & 0x0000ffff;
					else if (object_key_length == 3)
						m_object_key = m_object_key & 0x00ffffff;
					else // dvb allows up to 4 bytes
						m_object_key = m_object_key;

					pbuf2 += object_key_length;
				} else
					return TS_FALSE;
			}			
			break;
		case kTagLiteOptions:
			// ToDo
			break;
		}

		--tagged_profiles_count;
		pbuf += 8 + profile_data_length;
	}
	
	//  IOP::IOR() end
	
	TS_WORD object_info_length = NTOH_S(WORD_VALUE(pbuf, 0));
	pbuf += 2 + object_info_length;
	
    m_total_length = static_cast<TS_DWORD> (pbuf - pbuf_start);

	return TS_TRUE;
}

TS_STRING tssi::Binding::GetIdData() const
{
	return m_id_data;
}

TS_DWORD tssi::Binding::GetKindData() const
{
	return m_kind_data;
}

TS_DWORD tssi::Binding::GetTypeId() const
{
	return m_type_id;
}

TS_DWORD tssi::Binding::GetObjectKey() const
{
	return m_object_key;
}

TS_DWORD tssi::Binding::GetTotalLength() const
{
	return m_total_length;
}

