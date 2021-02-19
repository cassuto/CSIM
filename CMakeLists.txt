cmake_minimum_required(VERSION 3.9)
project(fastcsim)

set(COMMON_WARNINGS					"-Wall -Wno-unused-function")

set(CMAKE_CXX_FLAGS_DEBUG           "-g ${COMMON_WARNINGS}")
set(CMAKE_CXX_FLAGS_RELEASE         "-O2 ${COMMON_WARNINGS}")
set(CMAKE_CXX_FLAGS_PROFILE         "-O2 -pg ${COMMON_WARNINGS}")

set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "-pg")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "-pg")

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/src"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/models/base")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/models")
