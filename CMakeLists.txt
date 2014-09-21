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

set(combinations_iterator_PREFIX ${StellarCartographer_BINARY_DIR}/Contrib)
set(combinations_iterator_PREFIX ${StellarCartographer_BINARY_DIR}/Contrib)
set(combinations_iterator_TMP_DIR ${combinations_iterator_PREFIX}/tmp)
set(combinations_iterator_STAMP_DIR ${combinations_iterator_PREFIX}/src/combinations_iterator-stamp)
set(combinations_iterator_DOWNLOAD_DIR ${combinations_iterator_PREFIX}/src)
set(combinations_iterator_SOURCE_DIR ${combinations_iterator_PREFIX}/src/combinations_iterator)
set(combinations_iterator_BINARY_DIR ${combinations_iterator_PREFIX}/src/combinations_iterator-build)
set(combinations_iterator_INSTALL_DIR ${combinations_iterator_PREFIX})

ExternalProject_Add(combinations_iterator
    PREFIX ${combinations_iterator_PREFIX}
    GIT_REPOSITORY https://github.com/Fifty-Nine/combinations_iterator.git
    GIT_TAG d5699645de8256858fce028982ee06ff78524aac
    INSTALL_COMMAND ""
)

include_directories(
    ${StellarCartographer_SOURCE_DIR} 
    ${Boost_INCLUDE_DIRS} 
    ${combinations_iterator_SOURCE_DIR}
)

add_compile_options("-std=c++11")

add_subdirectory(StellarCartography)
add_subdirectory(UnitTests)
add_subdirectory(Query)

