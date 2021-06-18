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
    csimModelVCVS SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/VCVS.cc"
)
add_library(
    csimModelCCVS SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/CCVS.cc"
)
add_library(
    csimModelVCCS SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/VCCS.cc"
)
add_library(
    csimModelCCCS SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/CCCS.cc"
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
add_library(
    csimModelBSIM3V32 SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/bsim3v32/bsim3v32.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/bsim3v32/bsim3v32-status.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/bsim3v32/bsim3v32-stamp.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/bsim3v32/bsim3v32-model.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/bsim3v32/bsim3v32-conv.cc"
)

target_link_libraries(csimModelResistor csim)
target_link_libraries(csimModelVDC csim)
target_link_libraries(csimModelVAC csim)
target_link_libraries(csimModelIDC csim)
target_link_libraries(csimModelIAC csim)
target_link_libraries(csimModelVCVS csim)
target_link_libraries(csimModelCCVS csim)
target_link_libraries(csimModelVCCS csim)
target_link_libraries(csimModelCCCS csim)
target_link_libraries(csimModelPN csim)
target_link_libraries(csimModelCapacitor csim)
target_link_libraries(csimModelInductor csim)
target_link_libraries(csimModelBSIM3V32 csim)
