cmake_minimum_required(VERSION 3.20)

project(RaylibGuiCreator)

# Broken btw
if (WIN32 OR MINGW)
    include(${CMAKE_SOURCE_DIR}/toolchain/mingw_toolchain.cmake)
    set(BUILD_DESKTOP ON CACHE BOOL "" FORCE)
    set(USE_WAYLAND OFF CACHE BOOL "" FORCE)
    set(USE_X11 OFF CACHE BOOL "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20) # Use C++ 20

# Enable Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


set(BUILD_SHARED_LIBS ON)
set(BUILD_EXAMPLES OFF)

add_subdirectory(extern/raylib)
include_directories(extern/raylib/src) 

include_directories(./extern/RaylibAdditions) 
file(GLOB RAYLIB_ADDITIONS_SOURCES ./extern/RaylibAdditions/*.cpp)

include_directories(./extern/libtinyfiledialogs) 
add_library(tinyfiledialogs STATIC ./extern/libtinyfiledialogs/tinyfiledialogs.c)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(RaylibGuiCreator ${SOURCES} ${RAYLIB_ADDITIONS_SOURCES})
target_link_libraries(RaylibGuiCreator PRIVATE raylib tinyfiledialogs)

project(RaylibGuiCreator)

# Apply Release mode specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Add optimization flags
    target_compile_options(RaylibGuiCreator PRIVATE -O3)
    
    # Optionally, strip symbols to reduce binary size
    if (NOT WIN32)
        target_link_options(RaylibGuiCreator PRIVATE -s)
    endif()
endif()
