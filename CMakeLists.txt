cmake_minimum_required(VERSION 3.20)

project(rwstarter)

# Enable C++17 language standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(version.inc)

set(RWSTARTER_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
file(TO_NATIVE_PATH "${RWSTARTER_RESOURCES_DIR}" RWSTARTER_RESOURCES_DIR_WIN)

add_subdirectory(deps)

add_subdirectory(src)
