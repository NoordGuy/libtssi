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

#ifndef __TSSI_DESCRIPTOR_ISO639_H_INCLUDED__
#define __TSSI_DESCRIPTOR_ISO639_H_INCLUDED__

#include "descriptors/descriptor.h"
#include <vector>

namespace tssi {

	class TS_EXTERN Descriptor_Iso639 : public Descriptor{
	private:
		TS_PRIVATE_EXPORT( std::vector<TS_DWORD>	m_language_codes );
		TS_PRIVATE_EXPORT( std::vector<TS_BYTE>		m_language_types );

	public:
		static const TS_BYTE kDescriptorTag = 0x0a;

		Descriptor_Iso639();
		virtual ~Descriptor_Iso639();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		// get information
		unsigned	GetLanguageListLength() const;
		TS_DWORD	GetLanguageCode(unsigned index) const;
		TS_BYTE		GetLanguageType(unsigned index) const;

	};

}

#endif // __TSSI_DESCRIPTOR_ISO639_H_INCLUDED__