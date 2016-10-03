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

#include "dsm-cc/biopdecoder.h"
#include <fstream>

typedef int mode_t;

#ifdef _WIN32
#include <direct.h>
#endif // _WIN32
#include <cerrno>

tssi::BiopDecoder::BiopDecoder()
{
	m_service_gateway = 0;
}

tssi::BiopDecoder::~BiopDecoder()
{
	if (m_service_gateway)
		delete m_service_gateway;

	unsigned x;
	for (x = 0; x < m_file_list.size(); ++x)
		delete m_file_list[x];
	m_file_list.clear();

	for (x = 0; x < m_directory_list.size(); ++x)
		delete m_directory_list[x];
	m_directory_list.clear();
}

TS_VOID tssi::BiopDecoder::Reset()
{
	if (m_service_gateway) {
		delete m_service_gateway;
		m_service_gateway = 0;
	}

	unsigned x;
	for (x = 0; x < m_file_list.size(); ++x)
		delete m_file_list[x];
	m_file_list.clear();

	for (x = 0; x < m_directory_list.size(); ++x)
		delete m_directory_list[x];
	m_directory_list.clear();
}

TS_BOOL tssi::BiopDecoder::Process( const tssi::DsmccDownload& download )
{
	unsigned x;

	// unpack
	for (x = 0; x < download.module_list.size(); ++x)
	{
		if (! download.module_list[x]->AutoExtract() ) 
			return TS_FALSE;		
		// if module is not compressed, it is checked for completeness
	}

	// parse
	for (x = 0; x < download.module_list.size(); ++x)
	{
		if (! ProcessModule( download.module_list[x]->GetBuffer(), download.module_list[x]->GetModuleSize() ) )
			return TS_FALSE;		
	}
	return TS_TRUE;
}

TS_BOOL tssi::BiopDecoder::Decode( TS_STRING host_directory )
{
	if (! m_service_gateway )
		return TS_FALSE;

	TS_STRING directory = host_directory;
	if ( (directory.back() == '/' ) || (directory.back() == '\\' ) )
		directory.pop_back();

	// create directory
	mode_t nMode = 0733; // UNIX style permissions
	int nError = 0;
	#if defined(_WIN32)
		nError = _mkdir( directory.c_str() ); // can be used on Windows
	#else 
		nError = mkdir( directory.c_str(), nMode ); // can be used on non-Windows
	#endif
	if (nError != 0) {
		if (errno == ENOENT)
			return TS_FALSE;
		// else: folder already exists
	}
	directory += TS_STRING("/");

	unsigned x;
	for (x = 0; x < m_service_gateway->GetBindingList().size(); ++x)
		if (! ProcessBinding( m_service_gateway->GetBindingList()[x], directory ) ) 
			return TS_FALSE;

	return TS_TRUE;
}

TS_BOOL tssi::BiopDecoder::ProcessModule(TS_PBYTE pbuf, unsigned in_len)
{
	BiopMessage message;

	while (in_len > 0)
	{
		if (! message.Process(pbuf, in_len) )
			return TS_FALSE;

		switch ( message.GetObjectKind() ) {

		case BiopMessage::kBiopFil:
				{
					BiopMessage_File* biop = new BiopMessage_File();
					biop->Process(pbuf, in_len);

                    m_file_list.push_back(biop);
					break;
				}
		case BiopMessage::kBiopDir:
				{
					BiopMessage_Directory* biop = new BiopMessage_Directory();
					biop->Process(pbuf, in_len);

					m_directory_list.push_back(biop);
					break;
				}
		case BiopMessage::kBiopSrg:
				{
					BiopMessage_Directory* biop = new BiopMessage_Directory();
					biop->Process(pbuf, in_len);

					if (m_service_gateway) delete m_service_gateway;
					m_service_gateway = biop;
					break;

				}
		}
		pbuf += message.GetTotalLength();
		in_len -= message.GetTotalLength();
	}

	return TS_TRUE;
}

TS_BOOL tssi::BiopDecoder::ProcessBinding(Binding binding, TS_STRING directory)
{
	switch ( binding.GetKindData() ) {
	case BiopMessage::kBiopFil:
		{
			

			unsigned x;
			for (x = 0; x < m_file_list.size(); ++x) {
				if ( m_file_list[x]->GetObjectKey() != binding.GetObjectKey() )
					continue;

				TS_STRING filename = directory + binding.GetIdData();

				std::ofstream output( filename, std::ios::binary );
				output.write( reinterpret_cast <char const*> ( m_file_list[x]->GetContentData() ), m_file_list[x]->GetContentLength() );
				output.close();
				break;
			}
			if ( x == m_file_list.size() ) {
				// file not found
				// ToDo
				// std::cout << "dsmcc error - file not cached: " + binding.GetIdData() << std::endl;
			}

			break;
		}
	case BiopMessage::kBiopDir:
		{
			directory += binding.GetIdData();

			// create directory
			mode_t nMode = 0733; // UNIX style permissions
			int nError = 0;
			#if defined(_WIN32)
				nError = _mkdir( directory.c_str() ); // can be used on Windows
			#else 
				nError = mkdir( directory.c_str(), nMode ); // can be used on non-Windows
			#endif
			if (nError != 0) {
				if (errno == ENOENT)
					return TS_FALSE;
				// else: folder already exists
			}

			directory += TS_STRING("/");

			TS_BOOL result = TS_TRUE;
			unsigned x;
			for (x = 0; x < m_directory_list.size(); ++x) {
				if ( m_directory_list[x]->GetObjectKey() != binding.GetObjectKey() )
					continue;

				unsigned y;
				for (y = 0; y < m_directory_list[x]->GetBindingList().size(); ++y) {
					result *= ProcessBinding( m_directory_list[x]->GetBindingList()[y], directory );
				}
				break;
			}
			if (!result)
				return TS_FALSE;

			if ( x == m_directory_list.size() ) {
				// directory not found
				// ToDo
				// std::cout << "dsmcc error - directory not cached " << binding.GetObjectKey() << ": " + binding.GetIdData() << std::endl;
			}
			
			break;
		}

	}
	return TS_TRUE;
}





















