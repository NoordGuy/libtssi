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

#ifndef __TSSI_DESCRIPTOR_DVBJAPPLICATION_H_INCLUDED__
#define __TSSI_DESCRIPTOR_DVBJAPPLICATION_H_INCLUDED__

#include "descriptors/descriptor.h"
#include <vector>

namespace tssi {

	class TS_EXTERN Descriptor_DvbJApplication : public Descriptor	{
	private:
		TS_PRIVATE_EXPORT( std::vector<TS_STRING> m_parameter );
		TS_PRIVATE_EXPORT( TS_STRING m_missing_string );

	public:
		static const TS_BYTE kDescriptorTag = 0x03;

		Descriptor_DvbJApplication();
		virtual ~Descriptor_DvbJApplication();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		unsigned GetParameterListLength() const;
		const TS_STRING& GetParameter(unsigned index) const;

	};

}

#endif // __TSSI_DESCRIPTOR_DVBJAPPLICATION_H_INCLUDED__