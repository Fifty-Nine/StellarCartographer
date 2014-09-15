add_executable(scq
    main.cpp
)

link_directories(${StellarCartographer_BINARY_DIR}/StellarCartography)
target_link_libraries(scq StellarCartography ${Boost_LIBRARIES})
