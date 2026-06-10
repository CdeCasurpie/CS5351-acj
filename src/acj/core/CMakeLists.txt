cmake_minimum_required(VERSION 3.15)
project(acj_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
# CGAL target is now provided by FetchContent in the parent CMakeLists.txt

pybind11_add_module(acj_core 
    src/bindings.cpp 
    src/spatial_index.cpp 
    src/graph_simplify.cpp
)

target_link_libraries(acj_core PRIVATE CGAL::CGAL)

if(WIN32)
    find_package(Boost REQUIRED COMPONENTS headers)
else()
    find_package(Boost REQUIRED)
endif()
if(TARGET Boost::headers)
    target_link_libraries(acj_core PRIVATE Boost::headers)
endif()
if(WIN32)
    file(GLOB BOOST_CONFIG_HPP_PATH 
        "C:/vcpkg/installed/x64-windows/include/boost/config.hpp"
        "C:/vcpkg/installed/x64-windows/include/*/boost/config.hpp"
    )
    if(BOOST_CONFIG_HPP_PATH)
        list(GET BOOST_CONFIG_HPP_PATH 0 FIRST_BOOST_CONFIG)
        get_filename_component(BOOST_DIR "${FIRST_BOOST_CONFIG}" DIRECTORY)
        get_filename_component(BOOST_INC_ROOT "${BOOST_DIR}" DIRECTORY)
        target_include_directories(acj_core PRIVATE "${BOOST_INC_ROOT}")
        message(WARNING "CORE: FORCE INJECTED BOOST INCLUDE ROOT: ${BOOST_INC_ROOT}")
    else()
        message(FATAL_ERROR "CORE: ABSOLUTELY COULD NOT FIND boost/config.hpp IN VCPKG!")
    endif()
endif()

set_target_properties(acj_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Install the extension so scikit-build-core includes it in the wheel
install(TARGETS acj_core DESTINATION .)
