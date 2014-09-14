SET(SOURCES
    Coordinate.cpp
)

SET(HEADERS
    All.h
    Coordinate.h
)

add_library(StellarCartography
    ${SOURCES}
    ${HEADERS}
)
