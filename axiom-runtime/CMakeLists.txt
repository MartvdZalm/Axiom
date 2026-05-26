add_library(axiom_runtime INTERFACE)

add_library(axiom::runtime ALIAS axiom_runtime)

target_include_directories(
    axiom_runtime
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(axiom_runtime INTERFACE cxx_std_23)
