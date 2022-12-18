//
// Created by creeper on 22-12-3.
//
#include "ForbidCheck.h"
#define isEmpty(x,y) (!(state->row[(x)]&(3<<(y<<1))))
#define BLACK_LONG_MASK 4095
#define BBB 63//three black stones in a row, one possible situation for a live three
#define BBBB 255//one possible situation for a four
#define EBBEBE 828
#define EBEBBE 972//another two possible situations
#define BBEBB 975
#define BEBBB 1011
#define BBBEB 831

static const int BLACK_DOUBLE_FOUR_SAME_LINE[3] = {13299, 62415, 258879};
static const int dx[] = {0, 1, 1, -1};
static const int dy[] = {1, 0, 1, 1};
static struct State* global_state;
//11001111110011, 1111001111001111, 111111001100111111
/**
 *  whether it is OK to place a black stone at (x, y), considering forbidden moves
 */
bool queryBlackPlaceable(struct State* const state, int x, int y)
{
    if(x < 0 || y < 0 || x >= SIZE || y >= SIZE) return false;
    if(isEmpty(x,y))
    {
        bool forbid = false;
        PLACE(state, x, y, BLACK);
        forbid = checkForbid(state, x, y);
        CLEAR(state, x, y);
        return !forbid;
    }
    else return false;
}/*
bool queryBlackPlaceable(struct State* const state, int x, int y)
{
    if(x < 0 || y < 0 || x >= SIZE || y >= SIZE) return false;
    if(isEmpty(x,y))
    {
        struct Cross cross = takeCross(state, x, y);
        bool forbid = false;
        PLACE_CROSS(cross, BLACK);
        forbid = checkForbid(state, x, y);
        CLEAR_CROSS(cross);
        return !forbid;
    }
    else return false;
}
static bool checkLiveThreeDiagL(struct State *const state, int x, int y)
{
    struct Cross cross = takeCross(state, x, y);
    for(int k = 0; k < DIR_NUM; k++)
    {
        int local_l = *(cross.line[k]);
        int idx = cross.idx[k];
        for(int i = max(-2, -idx); i <= min(0, SIZE - 3 - idx); i++)
            if(checkGrids(local_l, idx + i, 3) == BBB)//find a BBB at [idx + i, idx + i + 2]
            {
                if(queryBlackPlaceable(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k]))//can form a four at [idx + i - 1, idx + i + 2]
                {
                    PLACE(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k], BLACK);
                    if(queryBlackPlaceable(state, x + (i - 2) * dx[k], y + (i - 2) * dy[k])
                        && queryBlackPlaceable(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]))
                    {
                        CLEAR(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k]);
                        return true;
                    }
                    CLEAR(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k]);
                }
                if(queryBlackPlaceable(state,x + (i + 3) * dx[k], y + (i + 3) * dy[k]))//can form a four at [idx + i, idx + i + 3]
                {
                    PLACE(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k], BLACK);
                    if(queryBlackPlaceable(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k])
                        && queryBlackPlaceable(state, x + (i + 4) * dx[k], y + (i + 4) * dy[k]))
                    {
                        CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
                        return true;
                    }
                    CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
                }
            }
        for(int i = max(-4, -idx); i <= min(-1, SIZE - 6 - idx); i++)
        {
            if(checkGrids(local_l, idx + i, 6) == EBBEBE)//find pattern EBBEBE at [i, i + 5]
            {
                if(queryBlackPlaceable(state,x + (i + 3) * dx[k], y + (i + 3) * dy[k]))//can form a four at [idx + i, idx + i + 3]
                {
                    PLACE(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k], BLACK);
                    if(queryBlackPlaceable(state, x + i * dx[k], y + i * dy[k])
                        && queryBlackPlaceable(state, x + (i + 5) * dx[k], y + (i + 5) * dy[k]))
                    {
                        CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
                        return true;
                    }
                    CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
                }
            }
            else if(checkGrids(local_l, idx + i, 6) == EBEBBE)
            {
                if(queryBlackPlaceable(state,x + (i + 2) * dx[k], y + (i + 2) * dy[k]))//can form a four at [idx + i, idx + i + 3]
                {
                    PLACE(state, x + (i + 2) * dx[k], y + (i + 2) * dy[k], BLACK);
                    if(queryBlackPlaceable(state, x + i * dx[k], y + i * dy[k])
                        && queryBlackPlaceable(state, x + (i + 5) * dx[k], y + (i + 5) * dy[k]))
                    {
                        CLEAR(state, x + (i + 2) * dx[k], y + (i + 2) * dy[k]);
                        return true;
                    }
                    CLEAR(state, x + (i + 2) * dx[k], y + (i + 2) * dy[k]);
                }
            }
        }
    }
    return false;
}
*/
static bool checkLiveThreeDiagL(struct State *const state, int x, int y)
{
    struct Cross cross = takeCross(state, x, y);
    int k = DIAG_L;
    int local_l = *(cross.line[k]);
    int idx = cross.idx[k];
    for(int i = max(-2, -idx); i <= min(0, SIZE - 3 - idx); i++)
        if(checkGrids(local_l, idx + i, 3) == BBB)//find a BBB at [idx + i, idx + i + 2]
        {
            if(queryBlackPlaceable(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k]))//can form a four at [idx + i - 1, idx + i + 2]
            {
                PLACE(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k], BLACK);
                if(queryBlackPlaceable(state, x + (i - 2) * dx[k], y + (i - 2) * dy[k])
                   && queryBlackPlaceable(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]))
                {
                    CLEAR(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k]);
                    return true;
                }
                CLEAR(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k]);
            }
            if(queryBlackPlaceable(state,x + (i + 3) * dx[k], y + (i + 3) * dy[k]))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k], BLACK);
                if(queryBlackPlaceable(state, x + (i - 1) * dx[k], y + (i - 1) * dy[k])
                   && queryBlackPlaceable(state, x + (i + 4) * dx[k], y + (i + 4) * dy[k]))
                {
                    CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
                    return true;
                }
                CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
            }
        }
    for(int i = max(-4, -idx); i <= min(-1, SIZE - 6 - idx); i++)
    {
        if(checkGrids(local_l, idx + i, 6) == EBBEBE)//find pattern EBBEBE at [i, i + 5]
        {
            if(queryBlackPlaceable(state,x + (i + 3) * dx[k], y + (i + 3) * dy[k]))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k], BLACK);
                if(queryBlackPlaceable(state, x + i * dx[k], y + i * dy[k])
                   && queryBlackPlaceable(state, x + (i + 5) * dx[k], y + (i + 5) * dy[k]))
                {
                    CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
                    return true;
                }
                CLEAR(state, x + (i + 3) * dx[k], y + (i + 3) * dy[k]);
            }
        }
        else if(checkGrids(local_l, idx + i, 6) == EBEBBE)
        {
            if(queryBlackPlaceable(state,x + (i + 2) * dx[k], y + (i + 2) * dy[k]))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x + (i + 2) * dx[k], y + (i + 2) * dy[k], BLACK);
                if(queryBlackPlaceable(state, x + i * dx[k], y + i * dy[k])
                   && queryBlackPlaceable(state, x + (i + 5) * dx[k], y + (i + 5) * dy[k]))
                {
                    CLEAR(state, x + (i + 2) * dx[k], y + (i + 2) * dy[k]);
                    return true;
                }
                CLEAR(state, x + (i + 2) * dx[k], y + (i + 2) * dy[k]);
            }
        }
    }
    return false;
}
static bool checkLiveThreeDiagR(struct State *const state, int x, int y)
{
    int diag, idx;
    if(x + y <= SIZE - 1)
    {
        diag = state->diagRU[x + y];
        idx = y;
    }
    else
    {
        diag = state->diagRD[x + y - SIZE + 1];
        idx = SIZE - 1 - x;
    }
    for(int i = max(-2, -idx); i <= min(0, SIZE - 3 - idx); i++)
        if(checkGrids(diag, idx + i, 3) == BBB)//find a BBB at [idx + i, idx + i + 2]
        {
            if(queryBlackPlaceable(state, x - i + 1, y + i - 1))//can form a four at [idx + i - 1, idx + i + 2]
            {
                PLACE(state, x - i + 1, y + i - 1, BLACK);
                if(queryBlackPlaceable(state, x - i + 2, y + i - 2) && queryBlackPlaceable(state, x - i - 3, y + i + 3))
                {
                    CLEAR(state, x - i + 1, y + i - 1);
                    return true;
                }
                CLEAR(state, x - i + 1, y + i - 1);
            }
            if(queryBlackPlaceable(state,x - i - 3, y + i + 3))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x - i - 3, y + i + 3, BLACK);
                if(queryBlackPlaceable(state, x - i + 1, y + i - 1) && queryBlackPlaceable(state, x - i - 4, y + i + 4))
                {
                    CLEAR(state, x - i - 3, y + i + 3);
                    return true;
                }
                CLEAR(state, x - i - 3, y + i + 3);
            }
        }
    for(int i = max(-4, -idx); i <= min(-1, SIZE - 6 - idx); i++)
    {
        if(checkGrids(diag, i, 6) == EBBEBE)//find pattern EBBEBE at [i, i + 5]
        {
            if(queryBlackPlaceable(state,x - i - 3, y + i + 3))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x - i - 3, y + i + 3, BLACK);
                if(queryBlackPlaceable(state, x - i, y + i) && queryBlackPlaceable(state, x - i - 5, y + i + 5))
                {
                    CLEAR(state, x - i - 3, y + i + 3);
                    return true;
                }
                CLEAR(state, x - i - 3, y + i + 3);
            }
        }
        else if(checkGrids(diag, idx + i, 6) == EBEBBE)
        {
            if(queryBlackPlaceable(state,x - i - 2, y + i + 2))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x - i - 2, y + i + 2, BLACK);
                if(queryBlackPlaceable(state, x - i, y + i) && queryBlackPlaceable(state, x - i - 5, y + i + 5))
                {
                    CLEAR(state, x - i - 2, y + i + 2);
                    return true;
                }
                CLEAR(state, x - i - 2, y + i + 2);
            }
        }
    }
    return false;
}

static bool checkLiveThreeColumn(struct State *const state, int x, int y)
{
    for(int i = max(x - 2, 0); i <= min(x, SIZE - 3); i++)
        if(checkGrids(state->col[y], i, 3) == BBB)//find a BBB at [i, i + 2]
        {
            if(queryBlackPlaceable(state, i - 1, y))//can form a four at [i - 2, i + 2]
            {
                PLACE(state, i - 1, y, BLACK);
                if(queryBlackPlaceable(state, i - 2, y) && queryBlackPlaceable(state, i + 3, y))
                {
                    CLEAR(state, i - 1, y);
                    return true;
                }
                CLEAR(state, i - 1, y);
            }
            if(queryBlackPlaceable(state,i + 3, y))//can form a four at [i, i + 3]
            {
                PLACE(state, i + 3, y, BLACK);
                if(queryBlackPlaceable(state, i - 1, y) && queryBlackPlaceable(state, i + 4, y))
                {
                    CLEAR(state, i + 3, y);
                    return true;
                }
                CLEAR(state, i + 3, y);
            }
        }
    for(int i = max(x - 4, 0); i <= min(x - 1, SIZE - 6); i++)
    {
        if(checkGrids(state->col[y], i, 6) == EBBEBE)//find pattern EBBEBE at [i, i + 5]
        {
            if(queryBlackPlaceable(state,i + 3, y))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, i + 3, y, BLACK);
                if(queryBlackPlaceable(state, i, y) && queryBlackPlaceable(state, i + 5, y))
                {
                    CLEAR(state, i + 3, y);
                    return true;
                }
                CLEAR(state, i + 3, y);
            }
        }
        else if(checkGrids(state->col[y], i, 6) == EBEBBE)
        {
            if(queryBlackPlaceable(state,i + 2, y))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, i + 2, y, BLACK);
                if(queryBlackPlaceable(state, i, y) && queryBlackPlaceable(state, i + 5, y))
                {
                    CLEAR(state,  i + 2, y);
                    return true;
                }
                CLEAR(state, i + 2, y);
            }
        }
    }
    return false;
}

static bool checkLiveThreeRow(struct State *const state, int x, int y)
{
    for(int i = max(y - 2, 0); i <= min(y, SIZE - 3); i++)
        if(checkGrids(state->row[x], i, 3) == BBB)//find a BBB at [i, i + 2]
        {
            if(queryBlackPlaceable(state, x, i - 1))//can form a four at [i - 1, i + 2]
            {
                PLACE(state, x, i - 1, BLACK);
                if(queryBlackPlaceable(state, x, i - 2) && queryBlackPlaceable(state, x, i + 3))
                {
                    CLEAR(state, x, i - 1);
                    return true;
                }
                CLEAR(state, x, i - 1);
            }
            if(queryBlackPlaceable(state, x, i + 3))//can form a four at [i, i + 3]
            {
                PLACE(state, x, i + 3, BLACK);
                if(queryBlackPlaceable(state, x, i - 1) && queryBlackPlaceable(state, x, i + 4))
                {
                    CLEAR(state, x, i + 3);
                    return true;
                }
                CLEAR(state, x, i + 3);
            }
        }
    for(int i = max(y - 4, 0); i <= min(y - 1, SIZE - 6); i++)
    {
        if(checkGrids(state->row[x], i, 6) == EBBEBE)//find pattern EBBEBE at [i, i + 5]
        {
            if(queryBlackPlaceable(state, x, i + 3))//can form a four at [idx + i, idx + i + 3]
            {
                PLACE(state, x, i + 3, BLACK);
                if(queryBlackPlaceable(state, x, i) && queryBlackPlaceable(state, x, i + 5))
                {
                    CLEAR(state, x, i + 3);
                    return true;
                }
                CLEAR(state, x, i + 3);
            }
        }
        else if(checkGrids(state->row[x], i, 6) == EBEBBE)
        {
            if(queryBlackPlaceable(state, x, i + 2))//can form a four at [i, i + 3]
            {
                PLACE(state, x, i + 2, BLACK);
                if(queryBlackPlaceable(state, x, i) && queryBlackPlaceable(state, x, i + 5))
                {
                    CLEAR(state, x, i + 2);
                    return true;
                }
                CLEAR(state, x, i + 2);
            }
        }
    }
    return false;
}

static bool checkFourDiagL(struct State *const state, int x, int y)
{
    int diag = y > x ? state->diagLU[y - x] : state->diagLD[x - y];
    int idx = min(x, y);
    //in fact the upper bound for i can be more specific since not all diagonals have 15 grids
    //but that will just cause a few more loops and do nothing, because they won't pass the checkGrids
    for(int i = max(-3, -idx); i <= min(0, SIZE - 3 - idx); i++)
        if(checkGrids(diag, idx + i, 4) == BBBB)//find a BBBB at [idx + i, idx + i + 3]
        {
            if(queryBlackPlaceable(state, x + i - 1, y + i - 1) //can form a five at [idx + i - 1, idx + i + 3]
                || queryBlackPlaceable(state,x + i + 4, y + i + 4)) //can form a five at [idx + i, idx + i + 4]
                return true;
        }
    for(int i = max(-4, -idx); i <= min(0, SIZE - 5 - idx); i++)
    {
        if(checkGrids(diag, idx + i, 5) == BEBBB //find pattern BEBBB at [i, i + 4]
            && queryBlackPlaceable(state, x + i + 1, y + i + 1))
            return true;
        if(checkGrids(diag, idx + i, 5) == BBEBB
            && queryBlackPlaceable(state, x + i + 2, y + i + 2))
            return true;
        if(checkGrids(diag, idx + i, 5) == BBBEB
           && queryBlackPlaceable(state, x + i + 3, y + i + 3))
            return true;
    }
    return false;
}

static bool checkFourDiagR(struct State *const state, int x, int y)
{
    int diag, idx;
    if(x + y <= SIZE - 1)
    {
        diag = state->diagRU[x + y];
        idx = y;
    }
    else
    {
        diag = state->diagRD[x + y - SIZE + 1];
        idx = SIZE - 1 - x;
    }
    for(int i = max(-3, -idx); i <= min(0, SIZE - 3 - idx); i++)
        if(checkGrids(diag, idx + i, 4) == BBBB)//find a BBBB at [idx + i, idx + i + 3]
        {
            if(queryBlackPlaceable(state, x - i + 1, y + i - 1) //can form a five at [idx + i - 1, idx + i + 3]
               || queryBlackPlaceable(state,x - i - 4, y + i + 4)) //can form a five at [idx + i, idx + i + 4]
                return true;
        }
    for(int i = max(-4, -idx); i <= min(0, SIZE - 5 - idx); i++)
    {
        if(checkGrids(diag, idx + i, 5) == BEBBB //find pattern BEBBB at [i, i + 4]
           && queryBlackPlaceable(state, x - i - 1, y + i + 1))
            return true;
        if(checkGrids(diag, idx + i, 5) == BBEBB
           && queryBlackPlaceable(state, x - i - 2, y + i + 2))
            return true;
        if(checkGrids(diag, idx + i, 5) == BBBEB
           && queryBlackPlaceable(state, x - i - 3, y + i + 3))
            return true;
    }
    return false;
}

static bool checkFourColumn(struct State *const state, int x, int y)
{
    for(int i = max(x - 3, 0); i <= min(x, SIZE - 3); i++)
        if(checkGrids(state->col[y], i, 4) == BBBB)//find a BBBB at [idx + i, idx + i + 3]
        {
            if(queryBlackPlaceable(state, i - 1, y) //can form a five at [idx + i - 1, idx + i + 3]
               || queryBlackPlaceable(state,i + 4, y)) //can form a five at [idx + i, idx + i + 4]
                return true;
        }
    for(int i = max(x - 4, 0); i <= min(x, SIZE - 5); i++)
    {
        if(checkGrids(state->col[y], i, 5) == BEBBB //find pattern BEBBB at [i, i + 4]
           && queryBlackPlaceable(state, i + 1, y))
            return true;
        if(checkGrids(state->col[y], i, 5) == BBEBB
           && queryBlackPlaceable(state, i + 2, y))
            return true;
        if(checkGrids(state->col[y], i, 5) == BBBEB
           && queryBlackPlaceable(state, i + 3, y))
            return true;
    }
    return false;
}

static bool checkFourRow(struct State *const state, int x, int y)
{
    for(int i = max(y - 3, 0); i <= min(y, SIZE - 3); i++)
        if(checkGrids(state->row[x], i, 4) == BBBB)//find a BBBB at [idx + i, idx + i + 3]
        {
            if(queryBlackPlaceable(state, x, i - 1) //can form a five at [idx + i - 1, idx + i + 3]
               || queryBlackPlaceable(state,x, i + 4)) //can form a five at [idx + i, idx + i + 4]
                return true;
        }
    for(int i = max(y - 4, 0); i <= min(y, SIZE - 5); i++)
    {
        if(checkGrids(state->row[x], i + 1, 5) == BEBBB //find pattern BEBBB at [i, i + 4]
           && queryBlackPlaceable(state, x, i + 1))
            return true;
        if(checkGrids(state->row[x], i + 2, 5) == BBEBB
           && queryBlackPlaceable(state, x, i + 2))
            return true;
        if(checkGrids(state->row[x], i + 3, 5) == BBBEB
           && queryBlackPlaceable(state, x, i + 3))
            return true;
    }
    return false;
}
/**
 * Checks whether the black stone placed at (x, y) forms a forbidden move.
 * To guarantee the accuracy of this check, I sacrifice much efficiency to make it robust.
 * However, this function can't be used in AI. To guarantee the efficiency of AI, the forbid checks should be as fast as
 * possible. It's only used in PVP and I won't optimize it intentionally.
 * It's a great waste to spend much time just for a weird and rare forbid check. So the checking function used for AI
 * will be greatly simplified to ignore the f**king extremely complex states and focus on efficiency.
 */

bool checkForbid(struct State* const state, int x, int y)
{
    //first consider the forbidden move formed in one direction, including the long forbid
    if(searchPatternAroundPointGreedy(state, x, y, 6, BLACK_LONG_MASK)) return true;
    if(searchPatternAroundPoint(state, x, y, RENJU_LENGTH, BLACK_RENJU_MASK)) return false;
    if(searchPatternAroundPoint(state, x, y, 7, BLACK_DOUBLE_FOUR_SAME_LINE[0])) return true;
    if(searchPatternAroundPoint(state, x, y, 8, BLACK_DOUBLE_FOUR_SAME_LINE[1])) return true;
    if(searchPatternAroundPoint(state, x, y, 9, BLACK_DOUBLE_FOUR_SAME_LINE[2])) return true;
    //now we can focus on the forbidden move formed in 2 directions
    //this can only be double-3 and double-4, first check double-3
    int live_three_cnt = 0;
    //The four functions can be compact into one using some small tricks
    //But I'll leave this for future...now I only need to make it run properly
    if(checkLiveThreeRow(state, x, y))live_three_cnt++;
    if(checkLiveThreeColumn(state, x, y)) live_three_cnt++;
    if(live_three_cnt == 2) return true;
    if(checkLiveThreeDiagL(state, x, y))live_three_cnt++;
    if(live_three_cnt == 2) return true;
    if(checkLiveThreeDiagR(state, x, y))live_three_cnt++;
    if(live_three_cnt == 2) return true;
    //finally check double-4
    int four_cnt = 0;
    if(checkFourRow(state, x, y)) four_cnt++;
    if(checkFourColumn(state, x, y)) four_cnt++;
    if(four_cnt == 2) return true;
    if(checkFourDiagL(state, x, y)) four_cnt++;
    if(four_cnt == 2) return true;
    if(checkFourDiagR(state, x, y)) four_cnt++;
    if(four_cnt == 2) return true;
    return false;
}
#undef isEmpty