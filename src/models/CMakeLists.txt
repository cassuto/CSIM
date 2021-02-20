cmake_minimum_required(VERSION 3.9)
project(csimModels)

add_library(
    csimModelResistor SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Resistor.cc"
)
add_library(
    csimModelVDC SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/VDC.cc"
)

target_link_libraries(csimModelResistor csim)
target_link_libraries(csimModelVDC csim)
