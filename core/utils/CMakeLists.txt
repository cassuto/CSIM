cmake_minimum_required(VERSION 3.9)
project(csimUtils)

set(SED_EXECUTABLE sed)

# Generate error code descriptors.
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/errors-generated.h
    COMMAND ${SED_EXECUTABLE} -f ${CMAKE_CURRENT_SOURCE_DIR}/errors.sed "${CMAKE_SOURCE_DIR}/include/csim/utils/errors.h" > ${CMAKE_CURRENT_BINARY_DIR}/errors-generated.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/errors.sed "${CMAKE_SOURCE_DIR}/include/csim/utils/errors.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${PROJECT_NAME}
    ${CMAKE_CURRENT_BINARY_DIR}/errors-generated.h
    ${CMAKE_CURRENT_SOURCE_DIR}/errors.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/string.cc
)
add_coverage(${PROJECT_NAME})