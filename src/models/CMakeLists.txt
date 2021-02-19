cmake_minimum_required(VERSION 3.9)
project(csimModels)

add_library(
    csimModelRegister SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Resistor.cc"
)

target_link_libraries(csimModelRegister csim)
