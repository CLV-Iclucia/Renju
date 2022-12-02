#ifndef RENJU_GAMEPLAY_H
#define RENJU_GAMEPLAY_H
#include "GameCore.h"
#include "TUI.h"
#define GAME_MODE_PVP 0
#define GAME_MODE_PVE 1
#define FULL_LINE 1073741823
#define ENDING_BLACK_WIN 0
#define ENDING_WHITE_WIN 1
#define ENDING_DRAW 2
#define ENDING_NOT_END 3

//#define PLACE(i, j, COLOR) row[(i)]|=(COLOR<<((j)<<1));\
//                           col[(j)]|=(COLOR<<((i)<<1));\
//                           diagL[(i)+(j)]=(COLOR<<((i)<<1));\
//                           diagR[(i)-(j)+SIZE]=(COLOR<<((i)<<1))//place a chess of COLOR at (i, j), make sure that (i, j) is valid
//#define CLEAR(i, j) row[(i)]&=CLR[(j)];\
//                    col[(j)]&=CLR[(i)];\
//                    diagL[(i)+(j)]&=CLR[(i)];\
//                    diagR[(i)-(j)+SIZE]&=CLR[(i)]//place a chess of COLOR at (i, j), make sure that (i, j) is valid

typedef int ERROR_CODE;
typedef int ENDING_CODE;
void drawBoard(const struct State* state);
void gameLoop(int gameMode, int AIColor);
bool checkForbid(struct State* state, int x, int y);
int checkWinner(struct State* state, int currentColor);
void playerPlace(struct State* state, int color);
#endif
