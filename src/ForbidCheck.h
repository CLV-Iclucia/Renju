//
// Created by creeper on 22-12-3.
//

#ifndef RENJU_FORBIDCHECK_H
#define RENJU_FORBIDCHECK_H
#include "GameCore.h"
bool queryBlackPlaceable(struct State* state, int x, int y);
bool checkForbid(struct State* state, int x, int y);

#endif //RENJU_FORBIDCHECK_H
