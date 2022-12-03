#ifndef RENJU_GAMEPLAY_H
#define RENJU_GAMEPLAY_H
#include "GameCore.h"
#include "TUI.h"
#define GAME_MODE_PVP 0
#define GAME_MODE_PVE 1
#define FULL_LINE 1073741823
#define ENDING_BLACK_WIN 0
#define ENDING_WHITE_WIN_RENJU 1
#define ENDING_WHITE_WIN_FORBID 2
#define ENDING_DRAW 3
#define ENDING_NOT_END 4

void drawBoard(const struct State* state);
void gameLoop(int gameMode, int AIColor);
bool checkForbid(struct State* state, int x, int y);
int checkWinner(struct State* state, int currentColor);
void playerPlace(struct State* state, int color);
#endif
