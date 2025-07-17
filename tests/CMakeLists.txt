# SerDes core tests
file(GLOB SERDES_TEST_SOURCES
        serdes/serialize_tests.cpp
        serdes/deserialize_tests.cpp
        serdes/validate_params_tests.cpp
        serdes/integration_tests.cpp
)
add_executable(serdes-tests ${SERDES_TEST_SOURCES})
target_include_directories(serdes-tests PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_link_libraries(serdes-tests
        PRIVATE
        libserdes
        libcommand
        gtest_main
)
add_test(NAME SerDesCoreTests COMMAND serdes-tests)

# Command layer tests
file(GLOB COMMAND_TEST_SOURCES
        commands/serialize_cmd_tests.cpp
        commands/deserialize_cmd_tests.cpp
)
add_executable(command-tests ${COMMAND_TEST_SOURCES})
target_include_directories(command-tests PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_link_libraries(command-tests
        PRIVATE
        libcommand
        libserdes
        gtest_main
)
add_test(NAME CommandLayerTests COMMAND command-tests)

# CLI parser tests
file(GLOB CLI_TEST_SOURCES
        cli/cli_parse_tests.cpp
)
add_executable(cli-tests ${CLI_TEST_SOURCES})
target_include_directories(cli-tests PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_link_libraries(cli-tests
        PRIVATE
        libcli
        gtest_main
)
add_test(NAME CliParseTests COMMAND cli-tests)