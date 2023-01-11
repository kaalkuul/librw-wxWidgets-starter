cmake_minimum_required(VERSION 3.20)

project(rwstarter)

# Enable C++17 language standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(version.inc)

set(RWSTARTER_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
file(TO_NATIVE_PATH "${RWSTARTER_RESOURCES_DIR}" RWSTARTER_RESOURCES_DIR_WIN)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extdeps/usr/local")

if (EXISTS "${CMAKE_PREFIX_PATH}/")
    message(STATUS "Found prefix path ${CMAKE_PREFIX_PATH}")
else()
    message(FATAL_ERROR "Failed to find external dependencies.\n"
        "Prefix path ${CMAKE_PREFIX_PATH} not found.\n"
        "Please build external dependencies first." )
endif()

add_subdirectory(deps)

add_subdirectory(src)
