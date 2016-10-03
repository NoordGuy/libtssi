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

#ifndef __TSSI_DESCRIPTOR_COMPONENT_H_INCLUDED__
#define __TSSI_DESCRIPTOR_COMPONENT_H_INCLUDED__

#include "descriptors/descriptor.h"

namespace tssi {
	class TS_EXTERN Descriptor_Component : public Descriptor	{
	private:
		TS_PRIVATE_EXPORT( TS_STRING m_text );
		TS_BYTE m_stream_content;
		TS_BYTE m_component_type;
		TS_BYTE m_component_tag;
		TS_DWORD m_language_code;

	public:
		static const TS_BYTE kDescriptorTag = 0x50;

		Descriptor_Component();
		virtual ~Descriptor_Component();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		const TS_STRING& GetText() const;
		TS_BYTE		GetStreamContent() const;
		TS_BYTE		GetComponentType() const;
		TS_BYTE		GetComponentTag() const;
		TS_DWORD	GetLanguageCode() const;
	};
}

#endif // __TSSI_DESCRIPTOR_COMPONENT_H_INCLUDED__