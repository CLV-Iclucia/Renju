cmake_minimum_required(VERSION 3.23)
project(Renju CUDA C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CUDA_STANDARD 14)

add_executable(Renju src/main.c src/GamePlay.h src/GamePlay.c src/utils.h src/utils.c src/GameAI.h src/GameCore.h src/GameAI.c src/TUI.h src/TUI.c src/DataStructure.h src/DataStructure.c)
