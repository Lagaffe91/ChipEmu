cmake_minimum_required(VERSION 3.19)

project(ChipEmulator)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})


add_subdirectory(src)