cmake_minimum_required(VERSION 3.9)
project(fastcsim)

set(SRCS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/models/ModelBase.cc"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/utils/Variant.cc"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/SubCircuit.cc"

	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc"
)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(COMMON_WARNINGS					"-Wall -Wno-unused-function")

set(CMAKE_CXX_FLAGS_DEBUG           "-g ${COMMON_WARNINGS}")
set(CMAKE_CXX_FLAGS_RELEASE         "-O2 ${COMMON_WARNINGS}")
set(CMAKE_CXX_FLAGS_PROFILE         "-O2 -pg ${COMMON_WARNINGS}")

set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "-pg")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "-pg")

add_executable(${PROJECT_NAME} ${SRCS})
