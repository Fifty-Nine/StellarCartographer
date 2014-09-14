add_executable(tests
    CoordinateTests.cpp
    TestMain.cpp
    Tests.h
)

link_directories(${StellarCartographer_BINARY_DIR}/StellarCartography)
target_link_libraries(tests
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    StellarCartography
)
