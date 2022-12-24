#ifndef RENJU_GAMECHECK_H
#define RENJU_GAMECHECK_H
#include "GameCore.h"
bool is_placeable(int x, int y);
int checkLiveThree(int x, int y);
int checkFour(int x, int y);
bool checkForbid(struct State* state, int x, int y);
bool destroyBlackLiveThree(int x, int y);
bool destroyBlackFour(int x, int y);
void estimateBlackPoint(int x, int y);
int countWhiteLiveThree(struct Cross cross);
int countWhiteFour(struct Cross cross);
bool checkWhiteRenju(struct Cross cross);
bool destroyWhiteLiveThree(int x, int y);
bool destroyWhiteFour(int x, int y);
int countWhiteTwo(struct Cross cross);
int countBlackTwo(struct Cross cross);
#endif //RENJU_GAMECHECK_H
