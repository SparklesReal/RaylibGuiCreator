cmake_minimum_required(VERSION 3.20)

project(RaylibGuiCreator)

set(CMAKE_CXX_STANDARD 20) # Use C++ 20

# Enable Release mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)
	find_package(raylib CONFIG REQUIRED) # For VCPKG
else()
	find_package(raylib REQUIRED) # For Linux/Unix
endif()

file(GLOB RAYLIB_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(RaylibGuiCreator ${RAYLIB_SOURCES})
target_include_directories(RaylibGuiCreator PRIVATE ${raylib_INCLUDE_DIRS})
target_link_libraries(RaylibGuiCreator PRIVATE ${raylib_LIBRARIES})

cmake_minimum_required(VERSION 3.20)

project(RaylibGuiCreator)

set(CMAKE_CXX_STANDARD 20) # Use C++ 20

# Apply Release mode specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Add optimization flags
    target_compile_options(RaylibGuiCreator PRIVATE -O3)
    
    # Optionally, strip symbols to reduce binary size
    if (NOT WIN32)
        target_link_options(RaylibGuiCreator PRIVATE -s)
    endif()
endif()
