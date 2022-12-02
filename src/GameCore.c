//
// Created by creeper on 22-12-1.
//
#include <malloc.h>
#include <string.h>
#include "GameCore.h"
#include "utils.h"

struct State* constructState()
{
    struct State* state = (struct State*)malloc(sizeof(struct State));
    memset(state->row, 0, sizeof(state->row));
    memset(state->col, 0, sizeof(state->col));
#ifdef STATE_VERSION_TEST
    memset(state->diagL, 0, sizeof(state->diagL));
    memset(state->diagR, 0, sizeof(state->diagR));
#else
    memset(state->diagLU, 0, sizeof(state->diagLU));
    memset(state->diagRU, 0, sizeof(state->diagRU));
    memset(state->diagLD, 0, sizeof(state->diagLD));
    memset(state->diagRD, 0, sizeof(state->diagRD));
#endif
    return state;
}

inline int GET(const struct State* state, int i, int j)
{
    return (state->row[i] >> (j << 1)) & 3;
}
inline void PLACE(struct State* const state, int i, int j, int color)
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
inline bool matchPattern(const struct State* state, int len, int pattern)
{
    //The search here can actually be optimized, since only a small number of grids are affected
    //That optimization is left for the future.
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

inline bool searchPatternAroundPoint(const struct State* state, int x, int y, int len, int pattern)
{
    for(int j = 0; j <= SIZE - len; j++)
        if(checkGrids(state->row[x], j, len) == pattern)
            return true;
    for(int j = 0; j <= SIZE - len; j++)
        if(checkGrids(state->col[y], j, len) == pattern)
            return true;
    if(y > x)
    {
        for(int j = 0; j <= SIZE - len - x; j++)
            if(checkGrids(state->diagLU[y - x], j, len) == pattern)
                return true;
    }
    else
    {
        for(int j = 0; j <= SIZE - len - x; j++)
            if(checkGrids(state->diagLD[x - y], j, len) == pattern)
                return true;
    }
    if(x + y <= SIZE - 1)
    {
        for(int j = 0; j <= x + 1 - len; j++)
            if(checkGrids(state->diagRU[x + y], j, len) == pattern)
                return true;
    }
    else
    {
        for(int j = 0; j <= SIZE - len - x; j++)
            if(checkGrids(state->diagRD[x + y - SIZE + 1], j, len) == pattern)
                return true;
    }
    return false;
}