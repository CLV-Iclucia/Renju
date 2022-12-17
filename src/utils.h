#ifndef RENJU_UTILS_H
#define RENJU_UTILS_H
#include <stdbool.h>
#include <bits/types/FILE.h>
#define DELETE(p) free(p);\
                    p = NULL
#define SIGNAL_UP 0
#define SIGNAL_DOWN 1
#define SIGNAL_LEFT 2
#define SIGNAL_RIGHT 3
#define SIGNAL_CONFIRM 4
#define SIGNAL_UNKNOWN 5
int stringstream(const char*, int*);
int getLine(char*, int);
int fgetLine(FILE* fp, char*, int);
static inline int min(int a, int b)
{
    return a > b ? b : a;
}
static inline int max(int a, int b)
{
    return a < b ? b : a;
}
/**
 * check the bits between the [pos, pos + len - 1] grids of the line,
 * namely the [2 * pos, 2 * (pos + len - 1)] bits of bit_mask.
 * @return the corresponding bits stored in an int
 */
static inline int checkGrids(int bit_mask, int pos, int len)
{
    return ((1 << (len << 1)) - 1) & (bit_mask >> (pos << 1));
}
void clear_output();
char getchar_crossplatform();
char getchar_no_buf();
int getControlInput();
struct Vec2i
{
    int x, y;
};
int read_num(FILE* fp);
#endif