cmake_minimum_required(VERSION 3.15)
project(acj_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Release mode for optimizations if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required packages
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(CGAL REQUIRED)

# NUEVA RUTA: Apunta a la carpeta src/
add_subdirectory(src/acj/core)
