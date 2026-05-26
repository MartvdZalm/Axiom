add_library(axiom_transform INTERFACE)

add_library(axiom::transform ALIAS axiom_transform)

target_include_directories(
    axiom_transform
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    axiom_transform
    INTERFACE
        axiom::core
        axiom::ir
)

target_compile_features(axiom_transform INTERFACE cxx_std_23)
