//
// Created by creeper on 22-11-18.
//

#ifndef RENJU_GAMECORE_H
#define RENJU_GAMECORE_H

#include <stdbool.h>
#define EMPTY 0
#define WHITE 2
#define BLACK 3
#define SIZE 15
#define RENJU_LENGTH 5
#define ALIGNMENT 16//better alignment for the compiler to optimize
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

struct State* constructState();
inline int GET(const struct State* state, int i, int j);
inline void PLACE(struct State* state, int i, int j, int color);
/**
 * check whether there is a certain pattern in the state(in four directions)
 * @param state the pointer to the state to be checked
 * @param len the length of the pattern
 * @param pattern one integer storing the pattern as bitmask
 * @return a bool. Returns true if and only if the pattern is matched in the state.
 */
inline bool matchPattern(const struct State* state, int len, int pattern);
/**
 * search a particular pattern around a given point in 4 directions
 * @param state the pointer to the state to be checked
 * @param x the row number of the given point
 * @param y the column number of the given point
 * @param len the length of the pattern to be checked
 * @param pattern the pattern
 * @return a bool. Returns true if and only if the pattern is found around the state.
 */
inline bool searchPatternAroundPoint(const struct State* state, int x, int y, int len, int pattern);
#endif //RENJU_GAMECORE_H
