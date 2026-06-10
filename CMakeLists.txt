cmake_minimum_required(VERSION 3.15)
project(acj_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Use FetchContent to extract the local CGAL tarball during build
include(FetchContent)
FetchContent_Declare(
  CGAL
  URL https://github.com/CGAL/cgal/releases/download/v6.0.1/CGAL-6.0.1.tar.xz
)
FetchContent_Populate(CGAL)

# CGAL is header-only from 5.0+, just add the include directories
add_library(CGAL::CGAL INTERFACE IMPORTED)
set_target_properties(CGAL::CGAL PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${cgal_SOURCE_DIR}/include"
)

# Optional: Find GMP and MPFR if available on the system
find_library(GMP_LIBRARY gmp)
find_library(MPFR_LIBRARY mpfr)
if(GMP_LIBRARY AND MPFR_LIBRARY)
    set_target_properties(CGAL::CGAL PROPERTIES
      INTERFACE_LINK_LIBRARIES "${GMP_LIBRARY};${MPFR_LIBRARY}"
    )
endif()

add_subdirectory(src/acj/core)
