add_library(axiom_core STATIC
    src/diagnostic.cpp
    src/source_manager.cpp
)

add_library(axiom::core ALIAS axiom_core)

target_include_directories(
    axiom_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(axiom_core PUBLIC fmt::fmt)

axiom_apply_warnings(axiom_core)

target_compile_features(axiom_core PUBLIC cxx_std_23)
