//
// Created by creeper on 22-11-18.
//

#ifndef RENJU_GAMECORE_H
#define RENJU_GAMECORE_H

#include <stdbool.h>
#include <assert.h>
#include "utils.h"
#define EMPTY 0
#define WHITE 2
#define BLACK 3
#define SIZE 15
#define RENJU_LENGTH 5
#define WHITE_RENJU_MASK 682//the bitmask of five white stones in a row is 1010101010
#define BLACK_RENJU_MASK 1023//the bitmask of five white stones in a row is 1111111111
#define ALIGNMENT 16//better alignment for the compiler to optimize
#define DIR_NUM 4
#define ROW 0
#define COL 1
#define DIAG_L 2
#define DIAG_R 3
//assign points for different targets
//how to calc the point for one move: if one move achieves certain following targets then add the corresponding points
#define FORM_RENJU (1 << 16)
#define DESTROY_FOUR (1 << 7)
#define FORM_FOUR (1 << 5)
#define DESTROY_LIVE_THREE (1 << 4)
#define FORM_LIVE_THREE (1 << 2)
#define FORM_TWO 1
/**
 * Use four/six int arrays to store current state of the state.
 * The four arrays store the state in column, row and two diagonals respectively.
 * That is to make use of the bitmask. Storing the state in 4 orders makes it possible
 * to match a certain pattern in 4 directions.
 *
 * I'll implement two versions here to compare the speed in the future.
 * For now I'll use the six-array version because it's easier to implement.
 */
struct State
{
#ifdef STATE_VERSION_TEST
    int diagL[ALIGNMENT << 1];
    int diagR[ALIGNMENT << 1];
#else
    int diagLU[ALIGNMENT];
    int diagLD[ALIGNMENT];
    int diagRU[ALIGNMENT];
    int diagRD[ALIGNMENT];
#endif
    int row[ALIGNMENT];
    int col[ALIGNMENT];
};
/**
 * Cross describes four lines crossing at a coordinates.
 * the indices of the coordinates are stored in four directions.
 * ANY modification to a Cross instance will infect the state it was taken from.
 */
struct Cross
{
    int *line[DIR_NUM], idx[DIR_NUM];
};

struct State* constructState();
/**
 * get the status of (i, j) on the board.
 */
static inline int GET(const struct State* state, int i, int j)
{
    return (state->row[i] >> (j << 1)) & 3;
}
/**
 * take the cross at (i, j) on the board.
 */
static inline struct Cross takeCross(const struct State* state, int i, int j)
{
    struct Cross cross;
    cross.line[ROW] = &(state->row[i]);
    cross.idx[ROW] = j;
    cross.line[COL] = &(state->col[j]);
    cross.idx[COL] = i;
    //I don't know which one is faster, so I'll do a comparison in the future.
#ifdef STATE_VERSION_TEST
    state->diagL[i - j + SIZE - 1] |= (color << ((i > j ? j : i) << 1));
    state->diagR[i + j] |= (color << ((i + j <= SIZE - 1 ? j : SIZE - 1 - i) << 1));
#else
    if(j > i)
    {
        cross.line[DIAG_L] = &(state->diagLU[j - i]);
        cross.idx[DIAG_L] = i;
    }
    else
    {
        cross.line[DIAG_L] = &(state->diagLD[i - j]);
        cross.idx[DIAG_L] = j;
    }
    if(i + j <= SIZE - 1)
    {
        cross.line[DIAG_R] = &(state->diagRU[i + j]);
        cross.idx[DIAG_R] = j;
    }
    else
    {
        cross.line[DIAG_R] = &(state->diagRD[i + j - SIZE + 1]);
        cross.idx[DIAG_R] = SIZE - 1 - i;
    }
    return cross;
#endif
}
/**
 * place a stone of a certain color on (i, j) of the board.
 */
static inline void PLACE(struct State* const state, int i, int j, int color)
{
    state->row[i] |= (color << (j << 1));
    state->col[j] |= (color << (i << 1));
    //I don't know which one is faster, so I'll do a comparison in the future.
#ifdef STATE_VERSION_TEST
    state->diagL[i - j + SIZE - 1] |= (color << ((i > j ? j : i) << 1));
    state->diagR[i + j] |= (color << ((i + j <= SIZE - 1 ? j : SIZE - 1 - i) << 1));
#else
    if(j > i) state->diagLU[j - i] |= (color << (i << 1));
    else state->diagLD[i - j] |= (color << (j << 1));
    if(i + j <= SIZE - 1) state->diagRU[i + j] |= (color << (j << 1));
    else state->diagRD[i + j - SIZE + 1] |= (color << ((SIZE - 1 - i) << 1));
#endif
}
/**
 * place a stone of a certain color on the coordinate defined by a Cross instance
 */
static inline void PLACE_CROSS(struct Cross cross, int color)
{
    for(int i = 0; i < DIR_NUM; i++)
        *(cross.line[i]) |= (color << (cross.idx[i] << 1));
}
/**
 * place a stone of a certain color on the coordinate defined by a Cross instance
 */
static inline void CLEAR_CROSS(struct Cross cross)
{
    for(int i = 0; i < DIR_NUM; i++)
        *(cross.line[i]) &= ~(3 << (cross.idx[i] << 1));
}
/**
 * clear a grid on the board.
 */
static inline void CLEAR(struct State* const state, int i, int j)
{
    state->row[i] &= ~(3 << (j << 1));
    state->col[j] &= ~(3 << (i << 1));
    //I don't know which one is faster, so I'll do a comparison in the future.
#ifdef STATE_VERSION_TEST
    state->diagL[i - j + SIZE - 1] |= (color << ((i > j ? j : i) << 1));
    state->diagR[i + j] |= (color << ((i + j <= SIZE - 1 ? j : SIZE - 1 - i) << 1));
#else
    if(j > i) state->diagLU[j - i] &= ~(3 << (i << 1));
    else state->diagLD[i - j] &= ~(3 << (j << 1));
    if(i + j <= SIZE - 1) state->diagRU[i + j] &= ~(3 << (j << 1));
    else state->diagRD[i + j - SIZE + 1] &= ~(3 << ((SIZE - 1 - i) << 1));
#endif
}
/**
 * check whether there is a certain pattern in the state(in four directions)
 * @param state the pointer to the state to be checked
 * @param len the length of the pattern
 * @param pattern one integer storing the pattern as bitmask
 * @return a bool. Returns true if and only if the pattern is matched in the state.
 */
static inline bool matchPattern(struct State* const state, int len, int pattern)
{
    //The search here can actually be optimized, since only a small number of grids are affected
    //That optimization is left for the future.
    //In fact the countPatternAroundPoint below can be used to replace this...
    //And it can also be parallelized using SIMD. That is also left for the future.
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j <= SIZE - len; j++)
            if(checkGrids(state->row[i], j, len) == pattern)
                return true;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j <= SIZE - len; j++)
            if(checkGrids(state->col[i], j, len) == pattern)
                return true;
    for(int i = 1; i <= SIZE - len; i++)
        for(int j = 0; j <= SIZE - len - i; j++)
            if(checkGrids(state->diagLU[i], j, len) == pattern)
                return true;
    for(int i = 0; i <= SIZE - len; i++)
        for(int j = 0; j <= SIZE - len - i; j++)
            if(checkGrids(state->diagLD[i], j, len) == pattern)
                return true;
    for(int i = len - 1; i <= SIZE - 1; i++)
        for(int j = 0; j <= i + 1 - len; j++)
            if(checkGrids(state->diagRU[i], j, len) == pattern)
                return true;
    for(int i = 1; i <= SIZE - len; i++)
        for(int j = 0; j <= SIZE - len - i; j++)
            if(checkGrids(state->diagRD[i], j, len) == pattern)
                return true;
    return false;
}
/**
 * search a particular black pattern around a given point in 4 directions.
 * @param state the pointer to the state to be checked
 * @param x the row number of the given point
 * @param y the column number of the given point
 * @param len the length of the pattern to be checked
 * @param pattern the pattern
 * @return a bool. Returns true if and only if the pattern is found around the state.
 */
static inline bool searchPatternAroundPointGreedy(struct State* const state, int x, int y, int len, int pattern)
{
    for(int j = 0; j <= SIZE - len; j++)
        if(checkGrids(state->row[x], j, len) == pattern)
            return true;
    for(int j = 0; j <= SIZE - len; j++)
        if(checkGrids(state->col[y], j, len) == pattern)
            return true;
    if(y > x)
    {
        for(int j = 0; j <= SIZE - len - y + x; j++)
            if(checkGrids(state->diagLU[y - x], j, len) == pattern)
                return true;
    }
    else
    {
        for(int j = 0; j <= SIZE - len - x + y; j++)
            if(checkGrids(state->diagLD[x - y], j, len) == pattern)
                return true;
    }
    if(x + y <= SIZE - 1)
    {
        for(int j = 0; j <= x + y + 1 - len; j++)
            if(checkGrids(state->diagRU[x + y], j, len) == pattern)
                return true;
    }
    else
    {
        for(int j = 0; j <= (SIZE << 1) - 1 - len - x - y; j++)
            if(checkGrids(state->diagRD[x + y - SIZE + 1], j, len) == pattern)
                return true;
    }
    return false;
}

static inline bool searchPatternAroundPoint(struct State* const state, int x, int y, int len, int pattern)
{
    for(int j = 0; j <= SIZE - len; j++)
        if(checkGrids(state->row[x], j, len) == pattern
            && (!j || checkGrids(state->row[x], j - 1, 1) != BLACK)
            && (j == SIZE - len || checkGrids(state->row[x], j + len, 1) != BLACK))
            return true;
    for(int j = 0; j <= SIZE - len; j++)
        if(checkGrids(state->col[y], j, len) == pattern
            && (!j || checkGrids(state->col[y], j - 1, 1) != BLACK)
            && (j == SIZE - len || checkGrids(state->col[y], j + len, 1) != BLACK))
            return true;
    if(y > x)
    {
        for(int j = 0; j <= SIZE - len - y + x; j++)
            if(checkGrids(state->diagLU[y - x], j, len) == pattern
                && (!j || checkGrids(state->diagLU[y - x], j - 1, 1) != BLACK)
                && (j == SIZE - len || checkGrids(state->diagLU[y - x], j + len, 1) != BLACK))
                return true;
    }
    else
    {
        for(int j = 0; j <= SIZE - len - x + y; j++)
            if(checkGrids(state->diagLD[x - y], j, len) == pattern
                && (!j || checkGrids(state->diagLD[x - y], j - 1, 1) != BLACK)
                && (j == SIZE - len || checkGrids(state->diagLD[x - y], j + len, 1) != BLACK))
                return true;
    }
    if(x + y <= SIZE - 1)
    {
        for(int j = 0; j <= x + y + 1 - len; j++)
            if(checkGrids(state->diagRU[x + y], j, len) == pattern
                && (!j || checkGrids(state->diagRU[x + y], j - 1, 1) != BLACK)
                && (j == SIZE - len || checkGrids(state->diagRU[x + y], j + len, 1) != BLACK))
                return true;
    }
    else
    {
        for(int j = 0; j <= (SIZE << 1) - 1 - len - x - y; j++)
            if(checkGrids(state->diagRD[x + y - SIZE + 1], j, len) == pattern
                && (!j || checkGrids(state->diagRD[x + y - SIZE + 1], j - 1, 1) != BLACK)
                && (j == SIZE - len || checkGrids(state->diagRD[x + y - SIZE + 1], j + len, 1) != BLACK))
                return true;
    }
    return false;
}
#endif //RENJU_GAMECORE_H
