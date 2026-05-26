include(CTest)

find_package(Catch2 CONFIG REQUIRED)

add_executable(axiom_tests
    lexer_tests.cpp
    parser_tests.cpp
    semantic_tests.cpp
    codegen_tests.cpp
)

target_link_libraries(
    axiom_tests
    PRIVATE
        axiom::core
        axiom::ast
        axiom::lexer
        axiom::parser
        axiom::semantic
        axiom::codegen
        Catch2::Catch2WithMain
)

target_include_directories(axiom_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_compile_definitions(
    axiom_tests
    PRIVATE
        AXIOM_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

axiom_apply_warnings(axiom_tests)

include(CTest)

# instead of catch_discover_tests:
add_test(NAME axiom_tests COMMAND axiom_tests)