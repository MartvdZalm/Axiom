option(AXIOM_UNITY_BUILD "Enable unity builds for Axiom targets" OFF)
option(AXIOM_ENABLE_SANITIZERS "Enable ASan/UBSan when supported" OFF)

if(AXIOM_UNITY_BUILD)
    set(CMAKE_UNITY_BUILD ON)
    set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)
endif()

if(AXIOM_ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()
