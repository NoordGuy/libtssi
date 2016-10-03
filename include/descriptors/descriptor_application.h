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

#ifndef __TSSI_DESCRIPTOR_APPLICATION_H_INCLUDED__
#define __TSSI_DESCRIPTOR_APPLICATION_H_INCLUDED__

#include "descriptors/descriptor.h"
#include <vector>

namespace tssi {

	class TS_EXTERN Descriptor_Application : public Descriptor	{
	private:
		struct AppVersion {
			TS_WORD		application_profile;
			TS_BYTE		version_major;
			TS_BYTE		version_minor;
			TS_BYTE		version_micro;
		};

		TS_PRIVATE_EXPORT( std::vector<AppVersion> m_app_version );
		TS_BOOL		m_service_bound;
		TS_BYTE		m_visibility;
		TS_BYTE		m_application_priority;

	public:
		static const TS_BYTE kDescriptorTag = 0x00;

		Descriptor_Application();
		virtual ~Descriptor_Application();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);		

		unsigned GetVersionListLength() const;
		TS_WORD	GetApplicationProfile(unsigned index) const;
		TS_BYTE	GetVersionMajor(unsigned index) const;
		TS_BYTE	GetVersionMinor(unsigned index) const;
		TS_BYTE	GetVersionMicro(unsigned index) const;
		TS_BOOL	GetServiceBound() const;
		TS_BYTE	GetVisibility() const;
		TS_BYTE	GetApplicationPriority() const;
		

	};

}

#endif // __TSSI_DESCRIPTOR_APPLICATION_H_INCLUDED__