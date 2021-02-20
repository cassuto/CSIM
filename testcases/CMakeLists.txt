cmake_minimum_required(VERSION 3.9)
project(csim-tests)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
)

add_library(gtest "${CMAKE_CURRENT_SOURCE_DIR}/googletest/src/gtest-all.cc")
add_library(gtest_main "${CMAKE_CURRENT_SOURCE_DIR}/googletest/src/gtest_main.cc")
target_link_libraries(gtest pthread)

set(TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLoader.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLinearSolver.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLinearSolverGauss.cc"
)

add_executable(tstall ${TEST_SRCS})
target_link_libraries(tstall gtest gtest_main csim)

add_custom_target(check COMMAND tstall --verbose)
