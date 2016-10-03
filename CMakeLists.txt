# ----------------------------------------------------------------------------
# Root CMake file for libtssi
#
#   Invoke
#      $ cmake <PATH_TO_LIBTSSI_ROOT>
#   from off-tree build directory
#
# ----------------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8.8)
project (libtssi)
set(CMAKE_BUILD_TYPE Release)

add_definitions(-DTS_DLLEXPORT)

find_package(ZLIB REQUIRED)
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
endif( ZLIB_FOUND )

include_directories(
    ${libtssi_SOURCE_DIR}/include
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(libtssi SHARED ${SOURCES})
set_property(TARGET libtssi PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(libtssi ${ZLIB_LIBRARIES} )



