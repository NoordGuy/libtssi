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

#ifndef __TSSI_DESCRIPTORLIST_H_INCLUDED__
#define __TSSI_DESCRIPTORLIST_H_INCLUDED__

#include "descriptors/descriptor.h"
#include <vector>

namespace tssi {

	class TS_EXTERN DescriptorList {
		TS_PRIVATE_EXPORT( std::vector<Descriptor*> m_list );

		TS_VOID	CreateDescriptor(Descriptor** ppalloc, TS_BYTE type);
		
	public:
		DescriptorList();
		DescriptorList( const DescriptorList& other );
		virtual ~DescriptorList();
		DescriptorList& operator= (const DescriptorList& rhs);

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);	// a descriptor set

		// get information
        unsigned	GetDescriptorListLength () const; // number of descriptors
		TS_BYTE		GetDescriptorLength (unsigned index) const; // length of descriptor #index
		TS_BYTE		GetDescriptorTag (unsigned index) const;
		unsigned	GetDescriptorListLength (TS_BYTE tag) const;
		Descriptor* GetDescriptor (unsigned index) const;
		Descriptor* GetDescriptorByTag (TS_BYTE tag) const;
		Descriptor* GetDescriptorByTag (TS_BYTE tag, unsigned index) const;
		
	};
    

}

#endif // __TSSI_DESCRIPTORLIST_H_INCLUDED__