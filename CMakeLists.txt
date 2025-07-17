cmake_minimum_required(VERSION 3.14)
project(wazuh-serdes VERSION 0.1.0 LANGUAGES CXX)

# Compiler configuration
set(CMAKE_CXX_STANDARD        20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS      OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Generated version header
set(VERSION_HEADER_IN   "${CMAKE_SOURCE_DIR}/cmake/version.hpp.in")
set(VERSION_HEADER_OUT  "${CMAKE_BINARY_DIR}/include/app/version.hpp")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include/app")
configure_file("${VERSION_HEADER_IN}" "${VERSION_HEADER_OUT}" @ONLY)

# External dependencies
include(FetchContent)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.5.0
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Common warning flags
set(PROJECT_WARNINGS
        -Wall -Wextra -Wpedantic -Werror
        -Wshadow -Wnon-virtual-dtor -Wold-style-cast
        -Wcast-align -Wunused -Wformat=2
)

# libserdes: core serialization/deserialization
add_library(libserdes STATIC
        src/serdes/serdes.cpp
)
target_include_directories(libserdes PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_compile_options(libserdes PRIVATE ${PROJECT_WARNINGS})

# libcommand: ICommand implementations
add_library(libcommand STATIC
        src/command/serialize_cmd.cpp
        src/command/deserialize_cmd.cpp
)
target_link_libraries(libcommand PUBLIC libserdes)
target_include_directories(libcommand PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_compile_options(libcommand PRIVATE ${PROJECT_WARNINGS})

# libcli: CLI parser (CLI11)
add_library(libcli STATIC
        src/options/cli/cli.cpp
)
target_link_libraries(libcli PUBLIC CLI11::CLI11)
target_include_directories(libcli PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_compile_options(libcli PRIVATE ${PROJECT_WARNINGS})

# libapp: application coordinator
add_library(libapp STATIC
        src/serdes/serdes_app.cpp
)
target_link_libraries(libapp PUBLIC libcommand libcli)
target_include_directories(libapp PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_compile_options(libapp PRIVATE ${PROJECT_WARNINGS})

# Main executable
add_executable(wazuh-serdes
        src/main.cpp
)
target_link_libraries(wazuh-serdes PRIVATE libapp)
target_include_directories(wazuh-serdes PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)
target_compile_options(wazuh-serdes PRIVATE ${PROJECT_WARNINGS})

#Documentation (Doxygen)
find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
    set(DOXYFILE_IN     ${CMAKE_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs)

    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

    add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )
else()
    message(WARNING "Doxygen not found, 'docs' target will be unavailable")
endif()

#clang-format integration
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if (CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_CLANG_FORMAT_FILES
            "${CMAKE_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/*.hpp"
            "${CMAKE_SOURCE_DIR}/include/*.hpp"
    )

    add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXE}
            -i
            -style=file
            ${ALL_CLANG_FORMAT_FILES}
            COMMENT "Applying clang-format to the entire codebase"
            VERBATIM
    )
else()
    message(WARNING "clang-format not found: target 'format' will not be available")
endif()

foreach(f IN LISTS ALL_CLANG_FORMAT_FILES)
    message(STATUS "→ clang-format will process: ${f}")
endforeach()


# clang-tidy Integration
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if (CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
            "${CLANG_TIDY_EXE};
         -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;
         -header-filter=include/.*"
    )
else()
    message(WARNING "clang-tidy not found: static analysis disabled")
endif()

# tests
add_subdirectory(tests)
