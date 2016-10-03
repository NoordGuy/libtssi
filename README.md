libtssi
=====
libtssi is a DVB and MPEG 2 TS demultiplexer and parser, written to be
- cross-platform
- modular
- easily extensible
- mostly dependency-free (only DSM-CC depends on zlib)
- easy to use

Currently, libtssi supports
- MPEG 2 and DVB table compilation for PAT, PMT, SDT, EIT, TDT, AIT, NIT
- Synchronization via PCR streams
- DSM-CC data and object model processing for MHP and HbbTV applications
- EBU Teletext PES data
- Descriptor processing

See libtssipython ([https://github.com/goforcode-com/libtssipython](https://github.com/goforcode-com/libtssipython)) for examples.

### Dependencies
- zlib ([http://www.zlib.net](http://www.zlib.net))

Usage of CMake ([http://cmake.org](http://cmake.org)) is advisable. A CMake GUI tool may be used to set the dependency paths appropriately. See the cmake help for further information.

### Roadmap
- Add an additional interpretation layer between packets and tables to properly handle sections for a cleaner version and sub-table management.
- Improve Strings implementation. Special characters in EBU teletext or DVB strings are missing and will result in false depiction. More robust TS_STRINGs (CRT).
- Proper stream error handling and protocol
- Expand table and descriptor coverage
- Documentation
- Unit test
 
### Author
Written by Martin Hoernig. Visit [goforcode.com](http://goforcode.com) for more information.

### License
libtssipython is GPL-licensed. Please refer to the LICENSE file for detailed information. 
