cmake_minimum_required(VERSION 3.9)
project(fastcsim)

option(enable_testcases "Build all of tests." ON)

set(COMMON_WARNINGS					"-Wall -Wno-unused-function")

set(CMAKE_CXX_FLAGS_DEBUG           "-g ${COMMON_WARNINGS}")
set(CMAKE_CXX_FLAGS_RELEASE         "-O2 ${COMMON_WARNINGS}")
set(CMAKE_CXX_FLAGS_PROFILE         "-O2 -pg ${COMMON_WARNINGS}")

set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "-pg")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "-pg")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#
# Check platform
#
if (WIN32)
	add_definitions(-DCSIM_IN_WIN32)
elseif (UNIX)
	add_definitions(-DCSIM_IN_UNIX)
else ()
	message(FATAL_ERROR "Unsupported platform ${CMAKE_SYSTEM}")
endif ()
if (CYGWIN)
add_definitions(-DCSIM_IN_CYGWIN)
endif ()

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/src"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/models")

if (enable_testcases)
	enable_testing()
	add_subdirectory("${PROJECT_SOURCE_DIR}/testcases")
endif ()
