#ifndef RENJU_GAMESYS_H
#define RENJU_GAMESYS_H
#include "GameCore.h"
#define MAX_SAVE_NAME_LENGTH 128
void read_meta();
bool save(struct State* state, struct Log* log_head, char *save_name);
#endif