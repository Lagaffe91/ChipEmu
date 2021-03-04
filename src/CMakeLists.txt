cmake_minimum_required(VERSION 3.19)

add_executable(ChipEmu main.cpp Chip8.h)
target_link_libraries(ChipEmu ${SDL2_LIBRARIES})

add_subdirectory(./sdl)

#May Need to do something more elegant
#https://stackoverflow.com/questions/10914607/how-to-collect-source-files-with-cmake-without-globbing is intresing