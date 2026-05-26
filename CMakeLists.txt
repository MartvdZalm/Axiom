cmake_minimum_required(VERSION 3.30)

project(
    Axiom
    VERSION 0.1.0
    DESCRIPTION "Reactive model-driven engineering framework"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AxiomOptions)
include(AxiomWarnings)
include(AxiomDependencies)

# Library modules (dependency order)
add_subdirectory(axiom-core)
add_subdirectory(axiom-ast)
add_subdirectory(axiom-lexer)
add_subdirectory(axiom-parser)
add_subdirectory(axiom-semantic)
add_subdirectory(axiom-ir)
add_subdirectory(axiom-transform)
add_subdirectory(axiom-codegen)
add_subdirectory(axiom-runtime)
add_subdirectory(axiom-cli)

option(AXIOM_BUILD_TESTS "Build Axiom unit tests" ON)
if(AXIOM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

option(AXIOM_BUILD_EXAMPLES "Build Axiom examples" ON)
if(AXIOM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
