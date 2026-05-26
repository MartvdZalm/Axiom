add_library(axiom_ast STATIC
    src/printer.cpp
)

add_library(axiom::ast ALIAS axiom_ast)

target_include_directories(
    axiom_ast
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(axiom_ast PUBLIC axiom::core fmt::fmt)

axiom_apply_warnings(axiom_ast)

target_compile_features(axiom_ast PUBLIC cxx_std_23)
