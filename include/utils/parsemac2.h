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

#ifndef __TSSI_PARSEMAC2_H_INCLUDED__
#define __TSSI_PARSEMAC2_H_INCLUDED__

#include "utils/parsemac.h"

//  ---------------------------------------------------------------------------
//  Network Information Table (NIT) macros

//  pb points to the first byte of section
#define NIT_TABLE_ID_VALUE(pb)                          BYTE_VALUE(pb,0)
#define NIT_SECTION_SYNTAX_INDICATOR_BIT(pb)            PSI_SECTION_SYNTAX_INDICATOR_BIT(pb)
#define NIT_SECTION_LENGTH_VALUE(pb)                    PSI_SECTION_LENGTH_VALUE(pb)
#define NIT_NETWORK_ID_VALUE(pb)						NTOH_S(WORD_VALUE(pb,3))
#define NIT_VERSION_NUMBER_VALUE(pb)                    PSI_VERSION_NUMBER_VALUE(pb)
#define NIT_CURRENT_NEXT_INDICATOR_BIT(pb)              PSI_CURRENT_NEXT_INDICATOR_BIT(pb)
#define NIT_SECTION_NUMBER(pb)                          PSI_SECTION_NUMBER_VALUE(pb)
#define NIT_LAST_SECTION_NUMBER(pb)                     PSI_LAST_SECTION_NUMBER_VALUE(pb)
#define NIT_NETWORK_DESCRIPTORS_LENGTH(pb)              (NTOH_S(WORD_VALUE(pb,8)) & 0x0fff)

// pb points to the first byte after the network descriptors
#define NIT_TRANSPORT_STREAM_LOOP_LENGTH(pb)            (NTOH_S(WORD_VALUE(pb,0)) & 0x0fff)

// pb points to the first byte of the TS loop
#define NIT_TRANSPORT_STREAM_ID(pb)						NTOH_S(WORD_VALUE(pb,0))
#define NIT_ORIGINAL_NETWORD_ID(pb)						NTOH_S(WORD_VALUE(pb,2))
#define NIT_TRANSPORT_DESCRIPTORS_LENGTH(pb)			(NTOH_S(WORD_VALUE(pb,4)) & 0x0fff)

//  ---------------------------------------------------------------------------
//  Service Description Table (SDT) macros

//  pb points to the first byte of section
#define SDT_TABLE_ID_VALUE(pb)                          BYTE_VALUE(pb,0)
#define SDT_SECTION_SYNTAX_INDICATOR_BIT(pb)            PSI_SECTION_SYNTAX_INDICATOR_BIT(pb)
#define SDT_SECTION_LENGTH_VALUE(pb)                    PSI_SECTION_LENGTH_VALUE(pb)
#define SDT_TRANSPORT_STREAM_ID_VALUE(pb)               NTOH_S(WORD_VALUE(pb,3))
#define SDT_VERSION_NUMBER_VALUE(pb)                    PSI_VERSION_NUMBER_VALUE(pb)
#define SDT_CURRENT_NEXT_INDICATOR_BIT(pb)              PSI_CURRENT_NEXT_INDICATOR_BIT(pb)
#define SDT_SECTION_NUMBER(pb)                          PSI_SECTION_NUMBER_VALUE(pb)
#define SDT_LAST_SECTION_NUMBER(pb)                     PSI_LAST_SECTION_NUMBER_VALUE(pb)
#define SDT_ORIGINAL_NETWORK_ID_VALUE(pb)               (NTOH_S(WORD_VALUE(pb,8)))

//  pb points to the service description record (service_id, etc...)
#define SDT_DESCRIPTON_RECORD_SERVICE_ID_VALUE(pb)      (NTOH_S(WORD_VALUE(pb,0)))
#define SDT_DESCRIPTON_RECORD_EIT_SCHEDULE_BIT(pb)      ((BYTE_VALUE(pb,2) >> 1) & 0x01)
#define SDT_DESCRIPTON_RECORD_EIT_P_F_BIT(pb) 			(BYTE_VALUE(pb,2) & 0x01)	// P_F = present following
#define SDT_DESCRIPTON_RECORD_RUNNING_STATUS_VALUE(pb)	((BYTE_VALUE(pb,3) >> 5) & 0x07)
#define SDT_DESCRIPTON_RECORD_FREE_CA_BIT(pb)           ((BYTE_VALUE(pb,3) >> 4) & 0x01)
#define SDT_DESCRIPTON_RECORD_SERVICE_INFO_LENGTH(pb)   (NTOH_S(WORD_VALUE(pb,3)) & 0x0fff)

//  ---------------------------------------------------------------------------
//  Event Information Table (EIT) macros

//  pb points to the first byte of section
#define EIT_TABLE_ID_VALUE(pb)                          BYTE_VALUE(pb,0)
#define EIT_SECTION_SYNTAX_INDICATOR_BIT(pb)            PSI_SECTION_SYNTAX_INDICATOR_BIT(pb)
#define EIT_SECTION_LENGTH_VALUE(pb)                    PSI_SECTION_LENGTH_VALUE(pb)
#define EIT_SERVICE_ID_VALUE(pb)						NTOH_S(WORD_VALUE(pb,3))
#define EIT_VERSION_NUMBER_VALUE(pb)                    PSI_VERSION_NUMBER_VALUE(pb)
#define EIT_CURRENT_NEXT_INDICATOR_BIT(pb)              PSI_CURRENT_NEXT_INDICATOR_BIT(pb)
#define EIT_SECTION_NUMBER(pb)                          PSI_SECTION_NUMBER_VALUE(pb)
#define EIT_LAST_SECTION_NUMBER(pb)                     PSI_LAST_SECTION_NUMBER_VALUE(pb)
#define EIT_TRANSPORT_STREAM_ID_VALUE(pb)               NTOH_S(WORD_VALUE(pb,8))
#define EIT_ORIGINAL_NETWORK_ID_VALUE(pb)               NTOH_S(WORD_VALUE(pb,10))
#define EIT_SEGMENT_LAST_SECTION_ID_VALUE(pb)           BYTE_VALUE(pb,12)
#define EIT_LAST_TABLE_ID_VALUE(pb)						BYTE_VALUE(pb,13)

//  pb points to the event description record
#define EIT_EVENT_ID_VALUE(pb)							NTOH_S(WORD_VALUE(pb,0))
#define EIT_START_TIME_VALUE(pb)						(NTOH_LL(QWORD_VALUE(pb,2)) >> 24)
#define EIT_DURATION_VALUE(pb)							(NTOH_LL(QWORD_VALUE(pb,2)) & 0x00FFFFFF)
#define EIT_RUNNING_STATUS_VALUE(pb)					(BYTE_VALUE(pb,10) >> 5)
#define EIT_FREE_CA_MODE_BIT(pb)						((BYTE_VALUE(pb,10) >> 4) & 0x01)
#define EIT_DESCRIPTORS_LOOP_LENGTH_VALUE(pb)			(NTOH_S(WORD_VALUE(pb,10)) & 0x0FFF)

//  ---------------------------------------------------------------------------
//  Application Information Table (AIT) macros

//  pb points to the first byte of section
#define AIT_TABLE_ID_VALUE(pb)                          BYTE_VALUE(pb,0)
#define AIT_SECTION_SYNTAX_INDICATOR_BIT(pb)            PSI_SECTION_SYNTAX_INDICATOR_BIT(pb)
#define AIT_SECTION_LENGTH_VALUE(pb)                    PSI_SECTION_LENGTH_VALUE(pb)
#define AIT_TEST_APPLICATION_BIT(pb)					(BYTE_VALUE(pb,3) >> 7)
#define AIT_APPLICATION_TYPE_VALUE(pb)					(NTOH_S(WORD_VALUE(pb,3)) & 0x7FFF)
#define AIT_VERSION_NUMBER_VALUE(pb)                    PSI_VERSION_NUMBER_VALUE(pb)
#define AIT_CURRENT_NEXT_INDICATOR_BIT(pb)              PSI_CURRENT_NEXT_INDICATOR_BIT(pb)
#define AIT_SECTION_NUMBER(pb)                          PSI_SECTION_NUMBER_VALUE(pb)
#define AIT_LAST_SECTION_NUMBER(pb)                     PSI_LAST_SECTION_NUMBER_VALUE(pb)
#define AIT_COMMON_DESCRIPTORS_LENGTH_VALUE(pb)         (NTOH_S(WORD_VALUE(pb,8)) & 0x0FFF)

// pb points to the application description record
#define AIT_ORGANISATION_ID_VALUE(pb)					NTOH_L(DWORD_VALUE(pb,0))
#define AIT_APPLICATION_ID_VALUE(pb)					NTOH_S(WORD_VALUE(pb,4))
#define AIT_APPLICATION_CONTROL_CODE_VALUE(pb)			BYTE_VALUE(pb,6)
#define AIT_APPLICATION_DESCRIPTORS_LENGTH_VALUE(pb)	(NTOH_S(WORD_VALUE(pb,7)) & 0x0FFF)

//  ---------------------------------------------------------------------------
//  EBU Teletext macros

//  pb points to the first byte of the PES data field
#define EBU_DATA_IDENTIFIER_VALUE(pb)					BYTE_VALUE(pb,0)

// pb points to the data field record
#define EBU_DATA_UNIT_ID_VALUE(pb)						BYTE_VALUE(pb,0)
#define EBU_DATA_UNIT_LENGTH_VALUE(pb)					BYTE_VALUE(pb,1)
#define EBU_FIELD_PARITY_BIT(pb)						((BYTE_VALUE(pb,2) >> 5) & 0x01)
#define EBU_LINE_OFFSET_VALUE(pb)						(BYTE_VALUE(pb,2) & 0x1F)
#define EBU_FRAMING_CODE_VALUE(pb)						BYTE_VALUE(pb,3)
#define EBU_MAGAZINE_VALUE(pb)							BYTE_VALUE(pb,4)
#define EBU_PACKET_NUMBER_VALUE(pb)						BYTE_VALUE(pb,5)

//  ============================================================================
//      descriptor parsing
//          pb points to the first byte of a descriptor or to a special pount
//  ============================================================================

// pb points to the whole descriptor
#define DESC_TAG(pb)                                    BYTE_VALUE(pb,0)
#define DESC_LENGTH(pb)                                 BYTE_VALUE(pb,1)

// pb points to the first point in the inner array
#define DESC_ISO639_LANGUAGE_CODE(pb)					(NTOH_L(DWORD_VALUE(pb,0)) >> 8)
#define DESC_ISO639_AUDIO_TYPE(pb)						BYTE_VALUE(pb,3)

// pb points to the first point in the inner array
#define DESC_TELETEXT_LANGUAGE_CODE(pb)					(NTOH_L(DWORD_VALUE(pb,0)) >> 8)
#define DESC_TELETEXT_AUDIO_TYPE(pb)					((BYTE_VALUE((pb),3) >> 3) & 0x1f)
#define DESC_TELETEXT_MAGAZINE_NUMBER(pb)				((BYTE_VALUE((pb),3) >> 5) & 0x07)
#define DESC_TELETEXT_PAGE_NUMBER(pb)					((((BYTE_VALUE((pb),4) >> 4) & 0x0f) * 10) + (BYTE_VALUE(pb,4) & 0x0f))

// pb points to the whole descriptor
#define DESC_SERVICE_TYPE(pb)							BYTE_VALUE(pb,2)
#define DESC_SERVICE_PROVIDER_NAME_LENGTH(pb)			BYTE_VALUE(pb,3)
#define DESC_SERVICE_NAME_LENGTH(pb)					BYTE_VALUE(pb,DESC_SERVICE_PROVIDER_NAME_LENGTH(pb)+4)

// pb points to the whole descriptor
#define DESC_SATDELIVERY_FREQUENCY(pb)					NTOH_L(DWORD_VALUE(pb,2))
#define DESC_SATDELIVERY_ORBITAL_POSITION(pb)			NTOH_S(WORD_VALUE(pb,6))
#define DESC_SATDELIVERY_WEST_EAST(pb)					(BYTE_VALUE(pb,8) >> 7)
#define DESC_SATDELIVERY_POLARIZATION(pb)				((BYTE_VALUE(pb,8) >> 5) & 0x03)
#define DESC_SATDELIVERY_MODULATION(pb)					(BYTE_VALUE(pb,8) & 0x1f)
#define DESC_SATDELIVERY_SYMBOL_RATE(pb)				(NTOH_L(DWORD_VALUE(pb,9)) >> 4)
#define DESC_SATDELIVERY_FEC_INNER(pb)					(BYTE_VALUE(pb,12) & 0x0f)

#endif // __TSSI_PARSEMAC2_H_INCLUDED__