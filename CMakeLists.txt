cmake_minimum_required(VERSION 3.31)
project(wazuh-serdes)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS     OFF)

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
        ${PROJECT_SOURCE_DIR}/include
        ${libcsv_SOURCE_DIR}/include)

target_link_libraries(wazuh-serdes
        PRIVATE
        CLI11::CLI11)
