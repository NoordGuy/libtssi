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

#ifndef __TSSI_DESCRIPTOR_SATELLITEDELIVERY_H_INCLUDED__
#define __TSSI_DESCRIPTOR_SATELLITEDELIVERY_H_INCLUDED__

#include "descriptors/descriptor.h"

namespace tssi {

	namespace SatelliteDeliveryPolarization {
		enum Enum {
			LINEAR_HORIZONTAL = 0x00,
			LINEAR_VERTICAL,
			CIRCULAR_HORIZONTAL,
			CIRCULAR_VERTICAL,
		};
	}

	namespace SatelliteDeliveryModulation {
		enum Enum {
			NOT_DEFINED = 0x00,
			QPSK,
			M8PSK,
			M16_QAM
		};
	}

	namespace SatelliteDeliveryFec {
		enum Enum {
			NOT_DEFINED = 0x00,
			F1_2,
			F2_3,
			F3_4,
			F5_6,
			F7_8,
			F8_9,
			NO = 0x0f
		};
	}

	class TS_EXTERN Descriptor_SatelliteDelivery : public Descriptor {

	private:
		TS_DWORD	m_frequency;
		TS_DWORD	m_orbital_position;
		TS_BOOL		m_west_east;
		TS_DWORD	m_symbol_rate;
		SatelliteDeliveryPolarization::Enum m_polarization;
		SatelliteDeliveryModulation::Enum	m_modulation;		
		SatelliteDeliveryFec::Enum			m_fec_inner;

	public:
		static const TS_BYTE kDescriptorTag = 0x43;

		Descriptor_SatelliteDelivery();
		virtual ~Descriptor_SatelliteDelivery();

		TS_VOID	Reset();
		TS_BOOL	Process(TS_PBYTE pbuf, unsigned in_len);

		TS_DWORD	GetFrequency() const;			// in 0.01 MHz -> e.g. 1175725 means 11757.25 MHz
		TS_DWORD	GetOrbitalPosition() const;		// in 0.1 degree -> e.g. 192 means 19.2°
		TS_BOOL		GetWestEastFlag() const;		// if TS_TRUE, eastern position, TS_FALSE indicates western position
		TS_DWORD	GetSymbolRate() const;			// in 0.0001 Msymbols/s -> e.g. 274500 means 27.4500 Msymbols/s
		SatelliteDeliveryPolarization::Enum GetPolarization() const;
		SatelliteDeliveryModulation::Enum	GetModulation() const; 
		SatelliteDeliveryFec::Enum			GetFecInner() const;		


	};

}

#endif // __TSSI_DESCRIPTOR_SATELLITEDELIVERY_H_INCLUDED__