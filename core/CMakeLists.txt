cmake_minimum_required(VERSION 3.9)
project(csim)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/utils")

set(SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/ModelBase.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/Variant.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/PropertyBag.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/MComplex.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/Environment.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/compact/SPICE-Compatible.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/compact/SPICE-Devsup.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/loader/Loader.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralHistory.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/LinearSolver.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/LinearSolverGauss.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/LinearSolverLU.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralCorrector.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralCorrectorGear.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralPredictor.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralPredictorEuler.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralPredictorGear.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/optimizer/GminOptimizer.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/Analyzers.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerOP.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerDC.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerAC.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerTran.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/Sweep.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/SweepLinear.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/SweepLogarithm.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/SweepDecade.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/SweepOctave.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/parser/ast-hspice.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/parser/algebraic.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Circuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Netlist.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Dataset.cc"
)

# Generate lexior
set(FLEX_HSPICE_OUT
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex-hspice.cc )
add_custom_command(
    OUTPUT ${FLEX_HSPICE_OUT}
    COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FLEX_HSPICE_OUT}
            ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex-hspice.lpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex-hspice.lpp
)
set_source_files_properties(${FLEX_HSPICE_OUT} PROPERTIES GENERATED TRUE)


# Generate parser
set(BISON_HSPICE_OUT
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-hspice.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-hspice.h )
add_custom_command(
    OUTPUT ${BISON_HSPICE_OUT}
    COMMAND ${BISON_EXECUTABLE}
                --defines=${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-hspice.h
                --output=${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-hspice.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-hspice.ypp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-hspice.ypp
)
set_source_files_properties(${BISON_HSPICE_OUT} PROPERTIES GENERATED TRUE)



# Generate lexior
set(FLEX_ALGEBRAIC_OUT
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex-algebraic.cc )
add_custom_command(
    OUTPUT ${FLEX_ALGEBRAIC_OUT}
    COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FLEX_ALGEBRAIC_OUT}
            ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex-algebraic.lpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/lex-algebraic.lpp
)
set_source_files_properties(${FLEX_ALGEBRAIC_OUT} PROPERTIES GENERATED TRUE)


# Generate parser
set(BISON_ALGEBRAIC_OUT
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-algebraic.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-algebraic.h )
add_custom_command(
    OUTPUT ${BISON_ALGEBRAIC_OUT}
    COMMAND ${BISON_EXECUTABLE}
                --defines=${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-algebraic.h
                --output=${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-algebraic.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-algebraic.ypp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/parse-algebraic.ypp
)
set_source_files_properties(${BISON_ALGEBRAIC_OUT} PROPERTIES GENERATED TRUE)


add_library(${PROJECT_NAME} SHARED
    ${SRCS}
    ${FLEX_HSPICE_OUT}
    ${BISON_HSPICE_OUT}
    ${FLEX_ALGEBRAIC_OUT}
    ${BISON_ALGEBRAIC_OUT}
)

target_link_libraries(${PROJECT_NAME} csimUtils)
