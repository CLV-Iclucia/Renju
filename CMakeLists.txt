cmake_minimum_required(VERSION 3.23)
project(Renju CUDA C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CUDA_STANDARD 14)

add_executable(Renju main.cu GamePlay.h Gameplay.c utils.h utils.c)
