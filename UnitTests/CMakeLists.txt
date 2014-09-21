add_executable(tests
    AlgorithmTests.cpp
    CoordinateTests.cpp
    StarMapTests.cpp
    TestMain.cpp
    Tests.cpp
    Tests.h
)

link_directories(${StellarCartographer_BINARY_DIR}/StellarCartography)
target_link_libraries(tests
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    StellarCartography
)
