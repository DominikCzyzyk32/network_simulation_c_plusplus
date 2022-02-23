cmake_minimum_required(VERSION 3.13)
project(net_simulation)

set(PROJECT_ID net_simulation)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion -Werror=switch)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        src/reports.cpp
        src/simulation.cpp
        )

set(SOURCES_FILES_TESTS
        test/test_package.cpp
        test/test_storage_types.cpp
        test/test_nodes.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        )

include(../SetCommonConfig.cmake)

add_executable(net_simulation ${SOURCE_FILES} main.cpp)