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

#ifndef __TSSI_BIOPMESSAGE_DIRECTORY_H_INCLUDED__
#define __TSSI_BIOPMESSAGE_DIRECTORY_H_INCLUDED__

#include "dsm-cc/biopmessage.h"
#include "dsm-cc/binding.h"
#include <vector>

namespace tssi {
	class BiopMessage_Directory : public BiopMessage {
	private:
		std::vector<Binding> m_binding_list;

	public:
		BiopMessage_Directory();
		virtual ~BiopMessage_Directory();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		const std::vector<Binding>& GetBindingList() const;

	};
}

#endif // __TSSI_BIOPMESSAGE_DIRECTORY_H_INCLUDED__