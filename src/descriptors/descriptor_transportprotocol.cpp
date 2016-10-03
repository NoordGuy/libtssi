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

#include "descriptors/descriptor_transportprotocol.h"
#include "utils/parsemac2.h"

tssi::Descriptor_TransportProtocol::Descriptor_TransportProtocol() : Descriptor()
{
	m_protocol_id = 0;
}

tssi::Descriptor_TransportProtocol::~Descriptor_TransportProtocol()
{
	Descriptor::~Descriptor();
}

TS_VOID tssi::Descriptor_TransportProtocol::Reset()
{
	Descriptor::Reset();
	m_protocol_id = 0;
}

TS_BOOL	tssi::Descriptor_TransportProtocol::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;

	m_protocol_id = NTOH_S(WORD_VALUE(pbuf,2));

	return TS_TRUE;
}

TS_WORD tssi::Descriptor_TransportProtocol::GetProtocolId() const
{
	return m_protocol_id;
}