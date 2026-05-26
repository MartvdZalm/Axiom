add_library(axiom_ir INTERFACE)

add_library(axiom::ir ALIAS axiom_ir)

target_include_directories(
    axiom_ir
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(axiom_ir INTERFACE axiom::core)

target_compile_features(axiom_ir INTERFACE cxx_std_23)

# Milestone 2+: IR definitions and builders.
