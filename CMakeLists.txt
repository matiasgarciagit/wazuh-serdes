cmake_minimum_required(VERSION 3.31)
project(wazuh-serdes VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS     OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Define and configure VERSION_HEADER
set(VERSION_HEADER_IN   "${CMAKE_SOURCE_DIR}/cmake/version.hpp.in")
set(VERSION_HEADER_OUT  "${CMAKE_BINARY_DIR}/include/app/version.hpp")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include/app")
configure_file(
        "${VERSION_HEADER_IN}"
        "${VERSION_HEADER_OUT}"
        @ONLY
)

include(FetchContent)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.5.0
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(cli11)

add_executable(wazuh-serdes src/main.cpp
        include/options/cli/cli.hpp
        include/options/options.hpp
        src/options/cli/cli.cpp
        src/serdes/serdes_app.cpp
        include/serdes/serdes_app.hpp
        include/command/command.hpp
        src/command/serialize_cmd.cpp
        src/command/deserialize_cmd.cpp
        include/command/deserialize_cmd.hpp
        include/command/serialize_cmd.hpp
        include/serdes/serdes.hpp
        src/serdes/serdes.cpp)

target_include_directories(wazuh-serdes
        PRIVATE
        ${CMAKE_BINARY_DIR}/include
        ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(wazuh-serdes
        PRIVATE
        CLI11::CLI11)

target_compile_options(wazuh-serdes PRIVATE
        -Wall -Wextra -Wpedantic -Werror
        -Wshadow -Wnon-virtual-dtor -Wold-style-cast
        -Wcast-align -Wunused -Wformat=2
)