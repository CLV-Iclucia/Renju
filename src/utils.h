#ifndef RENJU_UTILS_H
#define RENJU_UTILS_H
int stringstream(const char*, int*);
int getLine(char*, int);
/**
 * check the bits between the [pos, pos + len - 1] grids of the line,
 * namely the [2 * pos, 2 * (pos + len - 1)] bits of bit_mask.
 * @return the corresponding bits stored in an int
 */
inline int checkGrids(int bit_mask, int pos, int len)
{
    return ((1 << (len << 1)) - 1) & (bit_mask >> (pos << 1));
}
void clear_output();
struct Vec2i
{
    int x, y;
};
#endif