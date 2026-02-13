cmake_minimum_required(VERSION 4.2.3)
project(testproject)

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

add_definitions(-DSOME_DEFINITION)

add_executable(my_app ${sources})

target_compile_options(my_app PRIVATE
    -g
    -O0
    -fsanitize=address,undefined
    -fno-omit-frame-pointer
)

target_link_options(my_app PRIVATE
    -fsanitize=address,undefined
)

target_include_directories(my_app PUBLIC "${CMAKE_SOURCE_DIR}/include")

