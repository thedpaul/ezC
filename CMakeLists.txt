# Minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Project name
project(ezC C)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)

# Add flags for all builds
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Werror -D_XOPEN_SOURCE=700")

# Export compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
