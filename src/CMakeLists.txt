cmake_minimum_required(VERSION 3.9)
project(csim)

set(SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/ModelBase.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/Variant.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/PropertyBag.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/MComplex.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/loader/Loader.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Circuit.cc"
)

add_library(${PROJECT_NAME} SHARED ${SRCS})
