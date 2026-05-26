add_library(axiom_semantic STATIC
    src/analyzer.cpp
    src/symbol_table.cpp
)

add_library(axiom::semantic ALIAS axiom_semantic)

target_include_directories(
    axiom_semantic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    axiom_semantic
    PUBLIC
        axiom::core
        axiom::ast
)

axiom_apply_warnings(axiom_semantic)

target_compile_features(axiom_semantic PUBLIC cxx_std_23)
