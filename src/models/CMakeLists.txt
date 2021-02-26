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
add_library(
    csimModelVAC SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/VAC.cc"
)
add_library(
    csimModelIDC SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/IDC.cc"
)
add_library(
    csimModelIAC SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/IAC.cc"
)
add_library(
    csimModelPN SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/PN.cc"
)
add_library(
    csimModelCapacitor SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Capacitor.cc"
)
add_library(
    csimModelInductor SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Inductor.cc"
)

target_link_libraries(csimModelResistor csim)
target_link_libraries(csimModelVDC csim)
target_link_libraries(csimModelVAC csim)
target_link_libraries(csimModelIDC csim)
target_link_libraries(csimModelIAC csim)
target_link_libraries(csimModelPN csim)
target_link_libraries(csimModelCapacitor csim)
target_link_libraries(csimModelInductor csim)
