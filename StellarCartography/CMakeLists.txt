SET(SOURCES
    Coordinate.cpp
    Jump.cpp
    NeighborMap.cpp
    NeighborMapGraphTraits.cpp
    Star.cpp
    StarMap.cpp
)

SET(HEADERS
    All.h
    Coordinate.h
    Jump.h
    NeighborMap.h
    NeighborMapGraphTraits.h
    Star.h
    StarMap.h
)

add_library(StellarCartography
    ${SOURCES}
    ${HEADERS}
)
