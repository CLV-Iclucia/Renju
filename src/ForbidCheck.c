//
// Created by creeper on 22-12-3.
//
#include <stdio.h>
#include "GameCore.h"
#define INSIDE(x,y) ((x) >= 0 && (y) >= 0 && (x) < SIZE && (y) < SIZE)
#define STEP(x, step, dir) (x+(step)*(d##x[dir]))
#define isEmpty(step) (!(local_l & (3 << ((idx + (step)) << 1))))
#define PLACEABLE(step) (INSIDE(STEP(x, i + (step), k), STEP(y, i + (step), k)) && isEmpty(step))
#define BLACK_LONG_MASK 4095
#define BBBE 63//three black stones in a row, one possible situation for a live three
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
bool forbid(int x, int y);
bool checkForbid(struct State* state,int x, int y);
static inline bool check_forbid(int x, int y)
{
    PLACE(global_state, x, y, BLACK);
    if(!forbid(x, y))
    {
        CLEAR(global_state, x, y);
        return true;
    }
    else
    {
        CLEAR(global_state, x, y);
        return false;
    }
}
/**
 *  whether it is OK to place a black stone at (x, y), considering forbidden moves
 */
static bool checkLiveThree(int x, int y)
{
    struct Cross cross = takeCross(global_state, x, y);
    int cnt = 0;
    for(int k = 0; k < DIR_NUM; k++)
    {
        int local_l = *(cross.line[k]);
        int idx = cross.idx[k];
        bool find = false;
        for(int i = max(-2, -idx); i <= min(0, SIZE - 5 - idx); i++)
        {
            if(checkGrids(local_l, idx + i, 4) == BBBE)//find a BBBE at [idx + i, idx + i + 3]
            {
                if(PLACEABLE(-1))
                {
                    struct Cross cross_to_be_placed = takeCross(global_state, STEP(x, i - 1, k), STEP(y, i - 1, k));
                    PLACE_CROSS(cross_to_be_placed, BLACK);
                    if(!forbid(STEP(x, -1, k), STEP(y, -1, k)))//can form a four at [idx + i - 1, idx + i + 2]
                    {
                        if(PLACEABLE(-2))
                        {
                            if(!forbid(STEP(x, i - 2, k), STEP(y, i - 2, k))
                               && !forbid(STEP(x, i + 3, k), STEP(y, i + 3, k)))
                            {
                                find = true;
                                CLEAR_CROSS(cross_to_be_placed);
                                break;
                            }
                        }
                    }
                    CLEAR_CROSS(cross_to_be_placed);
                }
                if(PLACEABLE(3))
                {
                    struct Cross cross_to_be_placed = takeCross(global_state, STEP(x, i + 3, k), STEP(y, i + 3, k));
                    PLACE_CROSS(cross_to_be_placed, BLACK);
                    if(!forbid(STEP(x, i + 3, k), STEP(y, i + 3, k)))
                    {
                        if(PLACEABLE(-1) && PLACEABLE(4))//can form a four at [idx + i, idx + i + 3]
                        {
                            if(check_forbid(STEP(x, i - 1, k), STEP(y, i - 1, k))
                               && check_forbid(STEP(x, i + 4, k), STEP(y, i + 4, k)))
                            {
                                find = true;
                                CLEAR_CROSS(cross_to_be_placed);
                                break;
                            }
                        }
                    }
                    CLEAR_CROSS(cross_to_be_placed);
                }
            }
        }
        if(find)
        {
            cnt++;
            if(cnt == 2) return true;
        }
        for(int i = max(-4, -idx); i <= min(-1, SIZE - 6 - idx); i++)
        {
            if(checkGrids(local_l, idx + i, 6) == EBBEBE)//find pattern EBBEBE at [i, i + 5]
            {
                if(PLACEABLE(3))
                {
                    printf("fuck!\n");
                    struct Cross cross_to_be_placed = takeCross(global_state, STEP(x, i + 3, k), STEP(y, i + 3, k));
                    PLACE_CROSS(cross_to_be_placed, BLACK);
                    printf("%d\n", *(cross_to_be_placed.line[0]));
                    if(!forbid(STEP(x, i + 3, k), STEP(y, i + 3, k)))//can form a four at [idx + i + 1, idx + i + 4]
                    {
                        printf("%d\n", *(cross_to_be_placed.line[0]));
                        if(PLACEABLE(0) && PLACEABLE(5))
                        {
                            if(check_forbid(STEP(x, i, k), STEP(y, i, k))
                               && check_forbid(STEP(x, i + 5, k), STEP(y, i + 5, k)))
                            {
                                find = true;
                                CLEAR_CROSS(cross_to_be_placed);
                                break;
                            }
                        }
                        printf("%d %d\n", *(cross_to_be_placed.line[0]), *(cross_to_be_placed.line[0]) & (~(3 << ((cross_to_be_placed.idx[0]) << 1))));
                        CLEAR_CROSS(cross_to_be_placed);
                        printf("%d %d\n", *(cross_to_be_placed.line[0]), 3 << (cross_to_be_placed.idx[0] << 1));
                    }
                }
            }
            else if(checkGrids(local_l, idx + i, 6) == EBEBBE)
            {
                if(PLACEABLE(2))
                {
                    struct Cross cross_to_be_placed = takeCross(global_state, STEP(x, i + 2, k), STEP(y, i + 2, k));
                    PLACE_CROSS(cross_to_be_placed, BLACK);
                    if(!forbid(STEP(x, i + 2, k), STEP(y, i + 2, k)))//can form a four at [idx + i, idx + i + 3]
                    {
                        if(PLACEABLE(0) && PLACEABLE(5))
                        {
                            if(check_forbid(STEP(x, i, k), STEP(y, i, k))
                               && check_forbid(STEP(x, i + 5, k), STEP(y, i + 5, k)))
                            {
                                find = true;
                                CLEAR_CROSS(cross_to_be_placed);
                                break;
                            }
                        }
                    }
                    CLEAR_CROSS(cross_to_be_placed);
                }
            }
        }
        if(find)
        {
            cnt++;
            if(cnt == 2) return true;
        }
    }
    return false;
}

static bool checkFour(int x, int y)
{
    struct Cross cross = takeCross(global_state, x, y);
    int cnt = 0;
    for(int k = 0; k < DIR_NUM; k++)
    {
        int local_l = *(cross.line[k]);
        int idx = cross.idx[k];
        bool find = false;
        for(int i = max(-3, -idx); i <= min(0, SIZE - 3 - idx); i++)
        {
            if(checkGrids(local_l, idx + i, 4) == BBBB)//find a BBBB at [idx + i, idx + i + 3]
            {
                if(PLACEABLE(-1) && check_forbid(STEP(x, i - 1, k), STEP(y, i - 1, k)))
                {
                    find = true;
                    break;
                }
                else if(PLACEABLE(4) && check_forbid(STEP(x, i + 4, k), STEP(y, i - 1, k)))
                {
                    find = true; //can form a five at [idx + i, idx + i + 4]
                    break;
                }
            }
        }
        if(find)
        {
            cnt++;
            if(cnt == 2) return true;
        }
        for(int i = max(-4, -idx); i <= min(0, SIZE - 5 - idx); i++)
        {
            if(checkGrids(local_l, idx + i, 5) == BEBBB && PLACEABLE(1))//find pattern BEBBB at [i, i + 4]
            {
                if(check_forbid(STEP(x, i + 1, k), STEP(y, i + 1, k)))
                {
                    find = true;
                    break;
                }
            }
            if(checkGrids(local_l, idx + i, 5) == BBEBB && PLACEABLE(2))
            {
                if(check_forbid(STEP(x, i + 2, k), STEP(y, i + 2, k)))
                {
                    find = true;
                    break;
                }
            }
            if(checkGrids(local_l, idx + i, 5) == BBBEB && PLACEABLE(3))
            {
                if(check_forbid(STEP(x, i + 3, k), STEP(y, i + 3, k)))
                {
                    find = true;
                    break;
                }
            }
        }
        if(find)
        {
            cnt++;
            if(cnt == 2)return true;
        }
    }
    return false;
}

/**
 * Checks whether the black stone placed at (x, y) forms a forbidden move.
 * To guarantee the accuracy of this check, I sacrifice much efficiency to make it robust.
 * However, this function can't be used in AI. To guarantee the efficiency of AI, the forbid checks should be as fast as
 * possible.
 */
bool forbid(int x, int y)
{
    //first consider the forbidden move formed in one direction, including the long forbid
    if(searchPatternAroundPointGreedy(global_state, x, y, 6, BLACK_LONG_MASK)) return true;
    if(searchPatternAroundPoint(global_state, x, y, RENJU_LENGTH, BLACK_RENJU_MASK)) return false;
    if(searchPatternAroundPoint(global_state, x, y, 7, BLACK_DOUBLE_FOUR_SAME_LINE[0])) return true;
    if(searchPatternAroundPoint(global_state, x, y, 8, BLACK_DOUBLE_FOUR_SAME_LINE[1])) return true;
    if(searchPatternAroundPoint(global_state, x, y, 9, BLACK_DOUBLE_FOUR_SAME_LINE[2])) return true;
    //now we can focus on the forbidden move formed in 2 directions
    //this can only be double-3 and double-4, first check double-3
    if(checkLiveThree(x, y)) return true;
    //finally check double-4
    if(checkFour(x, y)) return true;
    return false;
}

bool checkForbid(struct State* const state, int x, int y)
{
    global_state = state;
    return forbid(x, y);
}
#undef isEmpty
#undef STEP
#undef PLACEABLE