cmake_minimum_required(VERSION 3.16)
project(cpp_chunker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.11.1
)
FetchContent_MakeAvailable(pybind11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

pybind11_add_module(chunker_cpp main.cpp chunker/chunker.cpp)

set_target_properties(chunker_cpp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})