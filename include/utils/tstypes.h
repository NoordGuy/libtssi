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

#ifndef __TSSI_TSTYPES_H_INCLUDED__
#define __TSSI_TSTYPES_H_INCLUDED__

#include <string>

typedef void					TS_VOID;
typedef unsigned				TS_BOOL;
typedef unsigned char			TS_BYTE;
typedef unsigned short			TS_WORD;
typedef unsigned long			TS_DWORD;
typedef unsigned long long		TS_QWORD;
typedef double					TS_DOUBLE;
typedef std::string				TS_STRING;
typedef TS_BYTE*				TS_PBYTE;
typedef TS_VOID*				TS_PVOID;

#define TS_TRUE					1
#define TS_FALSE				0

typedef struct _TS_TIME {
	TS_WORD year;
	TS_WORD month;
	TS_WORD day_of_week;
	TS_WORD day;
	TS_WORD hour;
	TS_WORD minute;
	TS_WORD second;
	TS_WORD milliseconds;
} TS_TIME, *PTS_TIME;

const TS_TIME TS_TIME_EMPTY = {0};

// itanium alignment
#if defined(__ia64) || defined(__itanium__) || defined(_M_IA64)
#  define UNALIGNED __unaligned
#else
#  define UNALIGNED
#endif

#ifdef _WIN32
#	ifdef TS_DLLEXPORT
#		define TS_EXTERN __declspec( dllexport )
#	else
#		define TS_EXTERN __declspec( dllimport )
#	endif
#else
#  define TS_EXTERN
#endif

// No dllexport check for privat members
#ifdef _MSC_VER
#	define TS_PRIVATE_EXPORT(x) \
		__pragma(warning(push)) __pragma(warning(disable: 4251)) (x); __pragma(warning(pop))
#else
#	define TS_PRIVATE_EXPORT(x) (x)
#endif

typedef TS_VOID (*TS_CALLBACK) (TS_PVOID);

#endif // __TSSI_TSTYPES_H_INCLUDED__ 