cmake_minimum_required(VERSION 3.0.0)

set(SOURCES
    Calculation.cpp
)
add_library(
    utility_lib STATIC
    ${SOURCES}
)