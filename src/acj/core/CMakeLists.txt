cmake_minimum_required(VERSION 3.15)
project(acj_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
# CGAL target is now provided by FetchContent in the parent CMakeLists.txt

pybind11_add_module(acj_core 
    src/bindings.cpp 
    src/spatial_index.cpp 
    src/graph_simplify.cpp
)

target_link_libraries(acj_core PRIVATE CGAL::CGAL)

set_target_properties(acj_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Install the extension so scikit-build-core includes it in the wheel
install(TARGETS acj_core DESTINATION .)
