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

#ifndef __TSSI_DESCRIPTOR_SERVICE_H_INCLUDED__
#define __TSSI_DESCRIPTOR_SERVICE_H_INCLUDED__

#include "descriptors/descriptor.h"

namespace tssi {

	class TS_EXTERN Descriptor_Service : public Descriptor{
	private:
		TS_BYTE	m_service_type;
		TS_PRIVATE_EXPORT(TS_STRING m_provider_name);
		TS_PRIVATE_EXPORT(TS_STRING m_service_name);

	public:
		static const TS_BYTE kDescriptorTag = 0x48;

		Descriptor_Service();
		virtual ~Descriptor_Service();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

        TS_BYTE		GetServiceType() const;
		const TS_STRING& GetProviderName() const;
		const TS_STRING& GetServiceName() const;

	};

}

#endif // __TSSI_DESCRIPTOR_SERVICE_H_INCLUDED__