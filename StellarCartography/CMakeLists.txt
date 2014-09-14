SET(SOURCES
    Coordinate.cpp
    NeighborMap.cpp
    SpatialIndex.cpp
)

SET(HEADERS
    All.h
    Coordinate.h
    NeighborMap.h
    Star.h
    SpatialIndex.h
)

add_library(StellarCartography
    ${SOURCES}
    ${HEADERS}
)
