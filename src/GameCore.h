//
// Created by creeper on 22-11-18.
//

#ifndef RENJU_GAMECORE_H
#define RENJU_GAMECORE_H

#include <stdbool.h>
#define EMPTY 0
#define WHITE 2
#define BLACK 3
#define GET(board, i, j) ((board[(i)]>>((j)<<1))&3) //get the state of (i, j)
#define PLACE(board, i, j, COLOR) board[(i)]|=(COLOR<<((j)<<1))
#define SIZE 15
#define ALIGNMENT 16//better alignment for the compiler to optimize
#define TRUE 1
#define FALSE 0
#endif //RENJU_GAMECORE_H
