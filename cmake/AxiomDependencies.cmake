find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

if(AXIOM_BUILD_TESTS)
    find_package(Catch2 3 CONFIG REQUIRED)
endif()
