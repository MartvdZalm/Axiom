add_library(axiom_codegen STATIC
    src/cpp_generator.cpp
)

add_library(axiom::codegen ALIAS axiom_codegen)

target_include_directories(
    axiom_codegen
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    axiom_codegen
    PUBLIC
        axiom::core
        axiom::ast
        axiom::semantic
)

axiom_apply_warnings(axiom_codegen)

target_compile_features(axiom_codegen PUBLIC cxx_std_23)
