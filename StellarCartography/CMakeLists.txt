SET(SOURCES
    Coordinate.cpp
    NeighborMap.cpp
    NeighborMapGraphTraits.cpp
    SpatialIndex.cpp
    Star.cpp
)

SET(HEADERS
    All.h
    Coordinate.h
    NeighborMap.h
    NeighborMapGraphTraits.h
    Star.h
    SpatialIndex.h
)

add_library(StellarCartography
    ${SOURCES}
    ${HEADERS}
)
