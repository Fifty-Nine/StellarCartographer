project(StellarCartographer)

cmake_minimum_required(VERSION 2.6)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost
    1.49.0
    COMPONENTS unit_test_framework regex
    REQUIRED
)

include(ExternalProject)

set(pairs_iterator_PREFIX ${StellarCartographer_BINARY_DIR}/Contrib)
set(pairs_iterator_PREFIX ${StellarCartographer_BINARY_DIR}/Contrib)
set(pairs_iterator_TMP_DIR ${pairs_iterator_PREFIX}/tmp)
set(pairs_iterator_STAMP_DIR ${pairs_iterator_PREFIX}/src/pairs_iterator-stamp)
set(pairs_iterator_DOWNLOAD_DIR ${pairs_iterator_PREFIX}/src)
set(pairs_iterator_SOURCE_DIR ${pairs_iterator_PREFIX}/src/pairs_iterator)
set(pairs_iterator_BINARY_DIR ${pairs_iterator_PREFIX}/src/pairs_iterator-build)
set(pairs_iterator_INSTALL_DIR ${pairs_iterator_PREFIX})

ExternalProject_Add(pairs_iterator
    PREFIX ${pairs_iterator_PREFIX}
    GIT_REPOSITORY https://github.com/Fifty-Nine/pairs_iterator.git
    GIT_TAG 1691efc1b62f7751a26edeffd3967f3464a78df3
    INSTALL_COMMAND ""
)

include_directories(
    ${StellarCartographer_SOURCE_DIR} 
    ${Boost_INCLUDE_DIRS} 
    ${pairs_iterator_SOURCE_DIR}
)

add_compile_options("-std=c++11")

add_subdirectory(StellarCartography)
add_subdirectory(UnitTests)
add_subdirectory(Query)

