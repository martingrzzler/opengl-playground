cmake_minimum_required(VERSION 3.14)
set(PROJECT "opengl-udemy")
project(${PROJECT} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(LIBRARY_NAME    "${PROJECT}_lib")
set(MAIN            "${PROJECT}")

set(CMAKE_BUILD_TYPE Debug)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_link_options("/NODEFAULTLIB:MSVCRT")
add_compile_options("/std:c++latest")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/app")
  