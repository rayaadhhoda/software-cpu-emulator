cmake_minimum_required(VERSION 3.15)
project(software_cpu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(software_cpu
    src/alu.cpp
    src/assembler.cpp
    src/bus.cpp
    src/cpu.cpp
    src/isa.cpp
    src/main.cpp
    src/memory.cpp
)

target_include_directories(software_cpu PRIVATE include)
