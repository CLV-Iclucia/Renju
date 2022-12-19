//
// Created by creeper on 22-11-18.
//
#include "GameCore.h"
#include <time.h>
static struct State* global_state;
static const int INF = 0x3f3f3f3f;
static const time_t return_time_second = 14;
static time_t return_time, timer;

int alpha, beta;
void dfs(int currenColor)
{
    if(time(NULL) >= return_time)
    {

    }
    else
    {

    }
}
void AIPlace(struct State* state, int currentColor)
{
    timer = time(NULL);
    return_time = return_time_second + timer;
    global_state = state;
    dfs(currentColor);
}