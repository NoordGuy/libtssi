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

#include "descriptors/descriptorlist.h"

#include "descriptors/descriptor_iso639.h"
#include "descriptors/descriptor_service.h"
#include "descriptors/descriptor_teletext.h"
#include "descriptors/descriptor_compression.h"
#include "descriptors/descriptor_satellitedelivery.h"
#include "descriptors/descriptor_networkname.h"
#include "descriptors/descriptor_shortevent.h"
#include "descriptors/descriptor_extendedevent.h"
#include "descriptors/descriptor_content.h"
#include "descriptors/descriptor_component.h"
#include "descriptors/descriptor_pdc.h"
#include "descriptors/descriptor_framerate.h"
#include "descriptors/descriptor_applicationsignalling.h"
#include "descriptors/descriptor_application.h"
#include "descriptors/descriptor_applicationname.h"
#include "descriptors/descriptor_transportprotocol.h"
#include "descriptors/descriptor_dvbjapplication.h"
#include "descriptors/descriptor_dvbjapplicationlocation.h"
#include "utils/parsemac2.h"

TS_VOID tssi::DescriptorList::CreateDescriptor(Descriptor** ppalloc, TS_BYTE type)
{
	switch (type) {
		case Descriptor_Application::kDescriptorTag : 
			*ppalloc = new Descriptor_Application(); 
			break;
		case Descriptor_ApplicationName::kDescriptorTag : 
			*ppalloc = new Descriptor_ApplicationName(); 
			break;
		case Descriptor_TransportProtocol::kDescriptorTag : 
			*ppalloc = new Descriptor_TransportProtocol(); 
			break;
		case Descriptor_DvbJApplication::kDescriptorTag : 
			*ppalloc = new Descriptor_DvbJApplication(); 
			break;
		case Descriptor_DvbJApplicationLocation::kDescriptorTag : 
			*ppalloc = new Descriptor_DvbJApplicationLocation(); 
			break;
		case Descriptor_Compression::kDescriptorTag : 
			*ppalloc = new Descriptor_Compression(); 
			break;
		case Descriptor_Iso639::kDescriptorTag : 
			*ppalloc = new Descriptor_Iso639(); 
			break;
			
		case Descriptor_NetworkName::kDescriptorTag : 
			*ppalloc = new Descriptor_NetworkName(); 
			break;
		case Descriptor_SatelliteDelivery::kDescriptorTag : 
			*ppalloc = new Descriptor_SatelliteDelivery();
			break;
		case Descriptor_Service::kDescriptorTag : 
			*ppalloc = new Descriptor_Service(); 
			break;
		case Descriptor_ShortEvent::kDescriptorTag : 
			*ppalloc = new Descriptor_ShortEvent();
			break;
		case Descriptor_ExtendedEvent::kDescriptorTag : 
			*ppalloc = new Descriptor_ExtendedEvent(); 
			break;
		case Descriptor_Component::kDescriptorTag : 
			*ppalloc = new Descriptor_Component(); 
			break;
		case Descriptor_Content::kDescriptorTag : 
			*ppalloc = new Descriptor_Content(); 
			break;
		case Descriptor_Teletext::kDescriptorTag : 
			*ppalloc = new Descriptor_Teletext(); 
			break;
		case Descriptor_Pdc::kDescriptorTag : 
			*ppalloc = new Descriptor_Pdc(); 
			break;
		case Descriptor_ApplicationSignalling::kDescriptorTag : 
			*ppalloc = new Descriptor_ApplicationSignalling(); 
			break;
		case Descriptor_FrameRate::kDescriptorTag : 
			*ppalloc = new Descriptor_FrameRate(); 
			break;
		default : *ppalloc = new Descriptor(); break;
	} 
}


tssi::DescriptorList& tssi::DescriptorList::operator= (const tssi::DescriptorList& rhs)
{
	Reset();

	m_list.resize(rhs.GetDescriptorListLength());
	for (unsigned i = 0; i < rhs.GetDescriptorListLength(); ++i) {
		const TS_BYTE tag = rhs.GetDescriptorTag(i);
		CreateDescriptor( &m_list[i], tag );
		
		switch (tag) {
			case Descriptor_Application::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Application*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Application*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_ApplicationName::kDescriptorTag : 
				*(dynamic_cast<Descriptor_ApplicationName*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_ApplicationName*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_TransportProtocol::kDescriptorTag : 
				*(dynamic_cast<Descriptor_TransportProtocol*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_TransportProtocol*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_DvbJApplication::kDescriptorTag : 
				*(dynamic_cast<Descriptor_DvbJApplication*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_DvbJApplication*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_DvbJApplicationLocation::kDescriptorTag : 
				*(dynamic_cast<Descriptor_DvbJApplicationLocation*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_DvbJApplicationLocation*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Compression::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Compression*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Compression*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Iso639::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Iso639*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Iso639*>( rhs.GetDescriptor(i) ));
				break;
			
			case Descriptor_NetworkName::kDescriptorTag : 
				*(dynamic_cast<Descriptor_NetworkName*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_NetworkName*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_SatelliteDelivery::kDescriptorTag : 
				*(dynamic_cast<Descriptor_SatelliteDelivery*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_SatelliteDelivery*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Service::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Service*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Service*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_ShortEvent::kDescriptorTag : 
				*(dynamic_cast<Descriptor_ShortEvent*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_ShortEvent*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_ExtendedEvent::kDescriptorTag : 
				*(dynamic_cast<Descriptor_ExtendedEvent*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_ExtendedEvent*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Component::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Component*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Component*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Content::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Content*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Content*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Teletext::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Teletext*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Teletext*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_Pdc::kDescriptorTag : 
				*(dynamic_cast<Descriptor_Pdc*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_Pdc*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_ApplicationSignalling::kDescriptorTag : 
				*(dynamic_cast<Descriptor_ApplicationSignalling*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_ApplicationSignalling*>( rhs.GetDescriptor(i) ));
				break;
			case Descriptor_FrameRate::kDescriptorTag : 
				*(dynamic_cast<Descriptor_FrameRate*>( m_list[i] )) = 
					*(dynamic_cast<Descriptor_FrameRate*>( rhs.GetDescriptor(i) ));
				break;
			default : 
				*(m_list[i]) = *(rhs.GetDescriptor(i));
				break;
		} 		
	}	
	return *this;
}

tssi::DescriptorList::DescriptorList()
{
}

tssi::DescriptorList::~DescriptorList()
{
	for (std::vector<Descriptor*>::iterator iter = m_list.begin() ; iter != m_list.end(); ++iter)
	{
		delete (*iter);
	} 
	m_list.clear();
}

tssi::DescriptorList::DescriptorList( const tssi::DescriptorList& other )
{
	if (this != &other) {
		this->operator=(other);
	}
}


TS_VOID tssi::DescriptorList::Reset()
{
	for (std::vector<Descriptor*>::iterator iter = m_list.begin() ; iter != m_list.end(); ++iter)
	{
		delete (*iter);
	} 
	m_list.clear();
}

TS_BOOL tssi::DescriptorList::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (in_len < 2) return TS_FALSE;

	while (in_len >= 2) {

		Descriptor* desc;

		TS_BYTE tag = DESC_TAG(pbuf);

		CreateDescriptor(&desc, tag);

		if (!desc->Process(pbuf,in_len)) {
			return TS_FALSE;
		}

		in_len -= 2;
		in_len -= desc->GetDescriptorLength();

		pbuf += 2;
		pbuf += desc->GetDescriptorLength();

		m_list.push_back(desc);
	}
	
	return TS_TRUE;
}

unsigned tssi::DescriptorList::GetDescriptorListLength() const
{
	return static_cast<unsigned> ( m_list.size() );
}

TS_BYTE tssi::DescriptorList::GetDescriptorLength(unsigned index) const
{
	if (index >= m_list.size()) return 0;
	return m_list[index]->GetDescriptorLength();
}

TS_BYTE tssi::DescriptorList::GetDescriptorTag(unsigned index) const
{
	if (index >= m_list.size()) return 0;
	return m_list[index]->GetDescriptorTag();
}

tssi::Descriptor* tssi::DescriptorList::GetDescriptor(unsigned index) const
{
	if (index >= m_list.size()) return 0;
	return m_list[index];
}

tssi::Descriptor* tssi::DescriptorList::GetDescriptorByTag(TS_BYTE tag) const
{	
	unsigned x;
	for (x = 0; x < m_list.size(); ++x) {
		if (m_list[x]->GetDescriptorTag() == tag)
		{
			return m_list[x];
		}
	}
	return 0;
}

tssi::Descriptor* tssi::DescriptorList::GetDescriptorByTag(TS_BYTE tag, unsigned index) const
{
	unsigned cnt = 0;
	unsigned x;
	for (x = 0; x < m_list.size(); ++x) {
		if (m_list[x]->GetDescriptorTag() == tag)
		{
			if ( index == cnt)
				return m_list[x];
			else
				++cnt;
		}
	}
	return 0;
}

unsigned tssi::DescriptorList::GetDescriptorListLength(TS_BYTE tag) const
{
	unsigned cnt = 0;
	unsigned x;
	for (x = 0; x < m_list.size(); ++x) {
		if (m_list[x]->GetDescriptorTag() == tag)
		{
			++cnt;
		}
	}
	return cnt;
}

