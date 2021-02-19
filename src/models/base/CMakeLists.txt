cmake_minimum_required(VERSION 3.9)
project(csimModelBase)

set(SRCS
	"${CMAKE_CURRENT_SOURCE_DIR}/ModelBase.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/Variant.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/PropertyBag.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/MComplex.cc"
)

add_library(${PROJECT_NAME} SHARED ${SRCS})
