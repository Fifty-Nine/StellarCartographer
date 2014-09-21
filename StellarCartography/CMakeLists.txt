SET(SOURCES
    Algorithms.cpp
    Coordinate.cpp
    Jump.cpp
    Star.cpp
    StarMap.cpp
)

SET(HEADERS
    Algorithms.h
    All.h
    Coordinate.h
    Jump.h
    Star.h
    StarMap.h
)

add_library(StellarCartography
    ${SOURCES}
    ${HEADERS}
)
