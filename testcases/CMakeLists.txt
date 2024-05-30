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
add_coverage(tstall)


add_custom_target(check COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/tstall
                        COMMENT "Running tests..."
                        DEPENDS tstall)

if (enable_coverage)
    find_program(CURL_EXECUTABLE curl)

    if (NOT CURL_EXECUTABLE)
        message(FATAL_ERROR "Coveralls: curl not found! Aborting")
    endif()

    set(COVERALLS_FILE ${CMAKE_BINARY_DIR}/coveralls.json)

    add_custom_target(clear_cov
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/clear_coverage.cmake"
        COMMENT "Clearing code coverage reports..."
        DEPENDS check)

    add_custom_target(gen_cov_report
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND gcovr -r .. -e '../testcases/googletest/' --html-details -o coverage.html
        COMMENT "Generating code coverage reports..."
        DEPENDS clear_cov)
    
    add_custom_target(gen_coveralls
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND gcovr -r .. -e '../testcases/googletest/' --coveralls -o ${COVERALLS_FILE}
        COMMENT "Generating code coverage for coveralls..."
        DEPENDS clear_cov)
    
    add_custom_target(coveralls_upload
        # Upload the JSON to coveralls.
        COMMAND ${CURL_EXECUTABLE}
                -S -F json_file=@${COVERALLS_FILE}
                https://coveralls.io/api/v1/jobs

        DEPENDS gen_coveralls

        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Uploading coveralls output...")

    add_custom_target(coverage DEPENDS gen_cov_report)

    add_custom_target(coveralls DEPENDS coveralls_upload)
endif ()