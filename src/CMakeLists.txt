cmake_minimum_required(VERSION 3.9)
project(csim)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/utils")

set(SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/ModelBase.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/Variant.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/PropertyBag.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/base/MComplex.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/models/loader/Loader.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/LinearSolver.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/LinearSolverGauss.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/LinearSolverLU.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralCorrector.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralCorrectorGear.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralPredictor.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/solver/IntegralPredictorEuler.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/Analyzers.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerDC.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerAC.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/analyzer/AnalyzerTran.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Circuit.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Netlist.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Dataset.cc"
)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_link_libraries(${PROJECT_NAME} csimUtils)
