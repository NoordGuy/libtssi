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

#ifndef __TSSI_DESCRIPTOR_COMPRESSION_H_INCLUDED__
#define __TSSI_DESCRIPTOR_COMPRESSION_H_INCLUDED__

#include "descriptors/descriptor.h"

namespace tssi {

	class TS_EXTERN Descriptor_Compression : public Descriptor{
	private:
		TS_BYTE		m_compression;
		TS_DWORD	m_original_size;
	public:
		static const TS_BYTE kDescriptorTag = 0x09;

		Descriptor_Compression();
		virtual ~Descriptor_Compression();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

        TS_BYTE		GetCompressionSystem() const;
		TS_DWORD	GetOriginalSize() const;

	};

}

#endif // __TSSI_DESCRIPTOR_COMPRESSION_H_INCLUDED__