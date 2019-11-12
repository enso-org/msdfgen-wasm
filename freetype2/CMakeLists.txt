cmake_minimum_required(VERSION 3.15)
project(msdfgen-wasm32)

file(GLOB_RECURSE FREETYPE2_SOURCES "*.h" "*.c")

set(FREETYPE2_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/objs/.libs/libfreetype.a")

add_custom_command(OUTPUT "${FREETYPE2_LIBRARY}"
    COMMAND bash ./autogen.sh
    COMMAND emconfigure ./configure --disable-shared
    COMMAND emmake make
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

add_custom_target(freetype2-library-file ALL
    DEPENDS "${FREETYPE2_LIBRARY}"
    SOURCES ${FREETYPE2_SOURCES})

add_library(freetype2 STATIC IMPORTED)
target_include_directories(freetype2 INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include")
set_target_properties(freetype2 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/objs/.libs/libfreetype2.a")
