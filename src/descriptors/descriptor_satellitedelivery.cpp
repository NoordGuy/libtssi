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

#include "descriptors/descriptor_satellitedelivery.h"
#include "utils/parsemac2.h"

tssi::Descriptor_SatelliteDelivery::Descriptor_SatelliteDelivery() : Descriptor() 
{
	m_frequency = 0;
	m_orbital_position = 0;
	m_west_east = 0;
	m_polarization = SatelliteDeliveryPolarization::LINEAR_HORIZONTAL;
	m_modulation = SatelliteDeliveryModulation::NOT_DEFINED;
	m_symbol_rate = 0;
	m_fec_inner = SatelliteDeliveryFec::NOT_DEFINED;
}

tssi::Descriptor_SatelliteDelivery::~Descriptor_SatelliteDelivery()
{
	Descriptor::~Descriptor();
}

TS_VOID tssi::Descriptor_SatelliteDelivery::Reset()
{
	Descriptor::Reset();
	m_frequency = 0;
	m_orbital_position = 0;
	m_west_east = 0;
	m_polarization = SatelliteDeliveryPolarization::LINEAR_HORIZONTAL;
	m_modulation = SatelliteDeliveryModulation::NOT_DEFINED;
	m_symbol_rate = 0;
	m_fec_inner = SatelliteDeliveryFec::NOT_DEFINED;
}

TS_BOOL	tssi::Descriptor_SatelliteDelivery::Process(TS_PBYTE pbuf, unsigned in_len)
{
	if (!Descriptor::Process(pbuf,in_len)) return TS_FALSE;
	if (m_descriptor_tag != kDescriptorTag) return TS_FALSE;


	m_frequency = DESC_SATDELIVERY_FREQUENCY(pbuf);
	m_orbital_position = DESC_SATDELIVERY_ORBITAL_POSITION(pbuf);
	m_west_east = DESC_SATDELIVERY_WEST_EAST(pbuf);
	m_polarization = static_cast<SatelliteDeliveryPolarization::Enum>( DESC_SATDELIVERY_POLARIZATION(pbuf) );
	m_modulation = static_cast<SatelliteDeliveryModulation::Enum>( DESC_SATDELIVERY_MODULATION(pbuf) );
	m_symbol_rate = DESC_SATDELIVERY_SYMBOL_RATE(pbuf);
	m_fec_inner = static_cast<SatelliteDeliveryFec::Enum>( DESC_SATDELIVERY_FEC_INNER(pbuf) );

	return TS_TRUE;
}

TS_DWORD tssi::Descriptor_SatelliteDelivery::GetFrequency() const
{
	TS_DWORD res = 0;

	unsigned nibble;
	unsigned x;
	unsigned fkt = 10000000;
	for (x = 0; x < 8; ++x) {
		nibble = (m_frequency >> (28 - 4*x)) & 0x0f;
		if (nibble > 9) nibble = 9;
		res += nibble * fkt;
		fkt /= 10;
	}

	return res;
}

TS_DWORD tssi::Descriptor_SatelliteDelivery::GetOrbitalPosition() const
{
	TS_DWORD res = 0;

	unsigned nibble;
	unsigned x;
	unsigned fkt = 1000;
	for (x = 0; x < 4; ++x) {
		nibble = (m_orbital_position >> (12 - 4*x)) & 0x0f;
		if (nibble > 9) nibble = 9;
		res += nibble * fkt;
		fkt /= 10;
	}

	return res;
}

TS_BOOL tssi::Descriptor_SatelliteDelivery::GetWestEastFlag() const
{
	return m_west_east;
}

TS_DWORD tssi::Descriptor_SatelliteDelivery::GetSymbolRate() const
{
	TS_DWORD res = 0;

	unsigned nibble;
	unsigned x;
	unsigned fkt = 1000000;
	for (x = 0; x < 7; ++x) {
		nibble = (m_symbol_rate >> (24 - 4*x)) & 0x0f;
		if (nibble > 9) nibble = 9;
		res += nibble * fkt;
		fkt /= 10;
	}

	return res;
}

tssi::SatelliteDeliveryPolarization::Enum tssi::Descriptor_SatelliteDelivery::GetPolarization() const
{
	return m_polarization;
}

tssi::SatelliteDeliveryModulation::Enum tssi::Descriptor_SatelliteDelivery::GetModulation() const
{
	return m_modulation;
}

tssi::SatelliteDeliveryFec::Enum tssi::Descriptor_SatelliteDelivery::GetFecInner() const
{
	return m_fec_inner;
}

