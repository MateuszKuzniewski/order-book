cmake_minimum_required(VERSION 4.2.3)
project(orderbook LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE sources
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE test_sources
    "${CMAKE_SOURCE_DIR}/test/*.h"
    "${CMAKE_SOURCE_DIR}/test/*.cpp"
)

add_definitions(-DSOME_DEFINITION)

list(FILTER sources EXCLUDE REGEX ".*main\\.cpp$")

add_library(orderbook_lib STATIC ${sources})

add_executable(build_source src/main.cpp)
add_executable(build_test ${test_sources})

target_link_libraries(build_source PRIVATE orderbook_lib)
target_link_libraries(build_test PRIVATE orderbook_lib)

target_include_directories(orderbook_lib PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_include_directories(build_test PRIVATE "${CMAKE_SOURCE_DIR}/tests")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    target_compile_options(orderbook_lib PUBLIC 
        -g
        -O0
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(orderbook_lib PUBLIC -fsanitize=address,undefined)
endif()


