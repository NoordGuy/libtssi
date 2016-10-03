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

#include "dsm-cc/biopmessage_directory.h"
#include "utils/parsemac2.h"

tssi::BiopMessage_Directory::BiopMessage_Directory() : BiopMessage()
{
}

tssi::BiopMessage_Directory::~BiopMessage_Directory()
{
	BiopMessage::~BiopMessage();
	m_binding_list.clear();
}

TS_VOID tssi::BiopMessage_Directory::Reset()
{
	BiopMessage::Reset();
	m_binding_list.clear();

}

TS_BOOL	tssi::BiopMessage_Directory::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (! BiopMessage::Process( pbuf, in_len ) ) return TS_FALSE;
	
	pbuf += 23 + m_object_key_length;

	if (m_object_info_length > 0)
	{
		// ToDo: parse object info data
		pbuf += m_object_info_length;
	}

	TS_BYTE service_context_list_count = BYTE_VALUE(pbuf, 0);
	++pbuf;

	while (service_context_list_count > 0)
	{
		TS_WORD entry_length = NTOH_S(WORD_VALUE(pbuf,4)) + 6;

		--service_context_list_count;
		pbuf += entry_length;
	}

	TS_DWORD message_body_length = NTOH_L(DWORD_VALUE(pbuf, 0));
	TS_WORD bindings_count = NTOH_S(WORD_VALUE(pbuf,4));
	pbuf += 6;

	unsigned i;					
	for (i = 0; i < bindings_count; ++i)
	{
        Binding binding;
		if (!binding.Process(pbuf, 0)) 
		{
			Reset();
			return TS_FALSE;
		}
		pbuf += binding.GetTotalLength();

		m_binding_list.push_back(binding);
	}

	return TS_TRUE;

}

const std::vector<tssi::Binding>& tssi::BiopMessage_Directory::GetBindingList() const
{
	return m_binding_list;
}


