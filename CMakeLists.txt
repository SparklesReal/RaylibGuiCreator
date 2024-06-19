cmake_minimum_required(VERSION 3.20)

project(RaylibGuiCreator)

set(CMAKE_CXX_STANDARD 20) # Use C++ 20

# Enable Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


set(BUILD_SHARED_LIBS ON)
set(BUILD_EXAMPLES OFF)

add_subdirectory(extern/raylib)
include_directories(extern/raylib/src) 
include_directories(extern/raylib/src/external)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(RaylibGuiCreator ${SOURCES})
target_link_libraries(RaylibGuiCreator PRIVATE raylib)

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
