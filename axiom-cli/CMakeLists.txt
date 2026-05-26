add_executable(axiom
    src/main.cpp
    src/commands.cpp
    src/pipeline.cpp
)

target_include_directories(
    axiom
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    axiom
    PRIVATE
        axiom::core
        axiom::ast
        axiom::lexer
        axiom::parser
        axiom::semantic
        axiom::codegen
        fmt::fmt
        spdlog::spdlog
)

axiom_apply_warnings(axiom)

target_compile_features(axiom PRIVATE cxx_std_23)
