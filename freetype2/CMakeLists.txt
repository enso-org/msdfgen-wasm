cmake_minimum_required(VERSION 3.12)
project(msdfgen-wasm32)

include_guard(GLOBAL)

file(GLOB_RECURSE FREETYPE_SOURCES "*.h" "*.c")

set(FREETYPE_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/objs/.libs/libfreetype.a")

add_custom_command(OUTPUT "${FREETYPE_LIBRARY}"
    COMMAND bash ./autogen.sh
    COMMAND emconfigure ./configure --disable-shared
    COMMAND emmake make
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

add_custom_target(freetype-library-file ALL
    DEPENDS "${FREETYPE_LIBRARY}"
    SOURCES ${FREETYPE_SOURCES})

add_library(freetype STATIC IMPORTED GLOBAL)
target_include_directories(freetype SYSTEM INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include")
set_target_properties(freetype PROPERTIES
    IMPORTED_LOCATION "${FREETYPE_LIBRARY}")
