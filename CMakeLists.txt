project(StellarCartographer)

cmake_minimum_required(VERSION 2.6)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost
    1.49.0
    COMPONENTS unit_test_framework
    REQUIRED
)

include_directories(
    ${StellarCartographer_SOURCE_DIR} 
    ${Boost_INCLUDE_DIRS} 
)

add_compile_options("-std=c++11")

add_subdirectory(StellarCartography)
add_subdirectory(UnitTests)
