add_executable(ChipEmu)

#Need to do something more elegant
target_sources(ChipEmu PUBLIC main.cpp PUBLIC Chip8.cpp PUBLIC Chip8.h)