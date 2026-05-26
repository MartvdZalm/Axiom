add_library(axiom_parser STATIC
    src/parser.cpp
)

add_library(axiom::parser ALIAS axiom_parser)

target_include_directories(
    axiom_parser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    axiom_parser
    PUBLIC
        axiom::core
        axiom::ast
        axiom::lexer
)

axiom_apply_warnings(axiom_parser)

target_compile_features(axiom_parser PUBLIC cxx_std_23)
