//
// Created by creeper on 22-12-1.
//
#include <malloc.h>
#include <string.h>
#include "GameCore.h"

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





