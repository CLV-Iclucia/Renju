#ifndef RENJU_UTILS_H
#define RENJU_UTILS_H
#include <stdbool.h>
#define UP (-1)
#define DOWN 1
#define CONFIRM 0
#define UNKNOWN 2
int stringstream(const char*, int*);
int getLine(char*, int);
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
#endif