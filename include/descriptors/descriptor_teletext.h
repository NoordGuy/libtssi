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

#ifndef __TSSI_DESCRIPTOR_TELETEXT_H_INCLUDED__
#define __TSSI_DESCRIPTOR_TELETEXT_H_INCLUDED__

#include "descriptors/descriptor.h"
#include <vector>

namespace tssi {

	class TS_EXTERN Descriptor_Teletext : public Descriptor {
	private:
		struct TeletextInformation {
			TS_DWORD	language;
			TS_BYTE		type;
			TS_BYTE		magazine_number;
			TS_BYTE		page_number;
		};

		TS_PRIVATE_EXPORT( std::vector<TeletextInformation> m_teletext );
		
	public:
		static const TS_BYTE kDescriptorTag = 0x56;

		Descriptor_Teletext();
		virtual ~Descriptor_Teletext();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		unsigned	GetTeletextListLength() const;
		TS_DWORD	GetTeletextLanguage(unsigned index) const;
		TS_BYTE		GetTeletextType(unsigned index) const;
		TS_BYTE		GetTeletextMagazineNumber(unsigned index) const;
		TS_BYTE		GetTeletextPageNumber(unsigned index) const;

	};

}

#endif // __TSSI_DESCRIPTOR_TELETEXT_H_INCLUDED__

