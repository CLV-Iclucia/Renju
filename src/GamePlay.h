#ifndef RENJU_GAMEPLAY_H
#define RENJU_GAMEPLAY_H
#include "GameCore.h"
#include "TUI.h"
#define GAME_MODE_PVP 0
#define GAME_MODE_PVE 1
#define NOTHING_WRONG 0
#define INVALID_INPUT_BUT_PARSEABLE 1
#define INVALID_INPUT_CANNOT_PARSE 2
#define INVALID_INPUT_OUT_OF_RANGE 3
#define INVALID_INPUT_OVERLAP 4
#define FULL_LINE 1073741823
//#define PLACE(i, j, COLOR) row[(i)]|=(COLOR<<((j)<<1));\
//                           col[(j)]|=(COLOR<<((i)<<1));\
//                           diagL[(i)+(j)]=(COLOR<<((i)<<1));\
//                           diagR[(i)-(j)+SIZE]=(COLOR<<((i)<<1))//place a chess of COLOR at (i, j), make sure that (i, j) is valid
//#define CLEAR(i, j) row[(i)]&=CLR[(j)];\
//                    col[(j)]&=CLR[(i)];\
//                    diagL[(i)+(j)]&=CLR[(i)];\
//                    diagR[(i)-(j)+SIZE]&=CLR[(i)]//place a chess of COLOR at (i, j), make sure that (i, j) is valid

typedef int ERROR_CODE;
ERROR_CODE parseCoord(const int* , char*, int*, int*);
void drawBoard(const int*, int, int);
void gameLoop(int, int);
int checkWinner(const int*);
void playerPlace(const int*);
#endif
