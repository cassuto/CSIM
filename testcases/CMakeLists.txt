cmake_minimum_required(VERSION 3.9)
project(csim-tests)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
)

set(TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLoader.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLinearSolver.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLinearSolverGauss.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstLinearSolverLU.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstOPLinearCircuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstOPNonlinearCircuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstDCLinearCircuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstCapacitorOP.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstInductorOP.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstVACInOP.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstACLinearCircuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstIntegralCorrector.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstIntegralPredictor.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstTransientAnalysis.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstOPSources.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstTransientNonlinearCircuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstGearTruncationError.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstHSPICEParser.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstAlgebraic.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/tstBSIM3v32.cc"
)

add_executable(tstall
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/src/gtest-all.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/src/gtest_main.cc"
    ${TEST_SRCS}
)
target_link_libraries(tstall pthread csim)
if (enable_coverage)
    target_compile_options(tstall PRIVATE
                                -fprofile-arcs
                                -ftest-coverage)
endif ()

add_custom_target(check COMMAND tstall
                        DEPENDS tstall)

if (enable_coverage)
    add_custom_target(run_cov
        COMMAND  tstall
        COMMENT "Running tests..."
        DEPENDS tstall)

    add_custom_target(gen_cov_report
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND gcovr -r .. -e '../testcases/googletest/' --html-details -o coverage.html
        COMMENT "Generating code coverage reports..."
        DEPENDS run_cov)

    add_custom_target(coverage DEPENDS gen_cov_report)
endif ()