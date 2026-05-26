add_library(axiom_lexer STATIC
    src/lexer.cpp
    src/token.cpp
)

add_library(axiom::lexer ALIAS axiom_lexer)

target_include_directories(
    axiom_lexer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(axiom_lexer PUBLIC axiom::core)

axiom_apply_warnings(axiom_lexer)

target_compile_features(axiom_lexer PUBLIC cxx_std_23)
