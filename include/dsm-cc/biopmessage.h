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

#ifndef __TSSI_BIOPMESSAGE_H_INCLUDED__
#define __TSSI_BIOPMESSAGE_H_INCLUDED__

#include "utils/tstypes.h"

namespace tssi {

	class BiopMessage {
	protected:
		TS_DWORD		m_magic_code;
		TS_BYTE			m_major_version;
		TS_BYTE			m_minor_version;
		TS_BYTE			m_message_type;
		TS_DWORD		m_message_size;
		TS_BYTE			m_object_key_length;
		TS_DWORD		m_object_key;
		TS_DWORD		m_object_kind;
		TS_WORD			m_object_info_length;

		TS_DWORD		m_total_length;

	public:
		static const TS_DWORD kBiopMagic = 0x42494F50;
		static const TS_DWORD kBiopFil   = 0x66696C00;
		static const TS_DWORD kBiopDir   = 0x64697200;
		static const TS_DWORD kBiopSrg   = 0x73726700;

		BiopMessage();
		virtual ~BiopMessage();

		virtual TS_VOID	Reset();
		virtual TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		TS_BYTE		GetMessageType() const;
		TS_DWORD	GetMessageSize() const;
		TS_DWORD	GetObjectKey() const;
		TS_DWORD	GetObjectKind() const;
		TS_WORD		GetObjectInfoLength() const;
		TS_DWORD	GetTotalLength() const;

	};
}

#endif // __TSSI_BIOPMESSAGE_H_INCLUDED__