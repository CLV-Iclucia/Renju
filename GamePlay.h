#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#define NOTHING_WRONG 0
#define INVALID_INPUT_BUT_PARSEABLE 1
#define INVALID_INPUT_CANNOT_PARSE 2
#define INVALID_INPUT_OUT_OF_RANGE 3
#define INVALID_INPUT_OVERLAP 4
#define EMPTY 0
#define WHITE 2
#define BLACK 3
#define GET(i, j) ((board[(i)]>>((j)<<1))&3) //get the state of (i, j)
#define SIZE 15
#define FULL 1073741823
#define PLACE(i, j, COLOR) board[(i)]|=(COLOR<<((j)<<1))
//#define PLACE(i, j, COLOR) row[(i)]|=(COLOR<<((j)<<1));\
//                           col[(j)]|=(COLOR<<((i)<<1));\
//                           diagL[(i)+(j)]=(COLOR<<((i)<<1));\
//                           diagR[(i)-(j)+SIZE]=(COLOR<<((i)<<1))//place a chess of COLOR at (i, j), make sure that (i, j) is valid
//#define CLEAR(i, j) row[(i)]&=CLR[(j)];\
//                    col[(j)]&=CLR[(i)];\
//                    diagL[(i)+(j)]&=CLR[(i)];\
//                    diagR[(i)-(j)+SIZE]&=CLR[(i)]//place a chess of COLOR at (i, j), make sure that (i, j) is valid
typedef int ERROR_CODE;
ERROR_CODE parseCoord(char*, int*, int*);
void GUI();
void drawBoard(int, int);
void gamesLoop();
#endif
