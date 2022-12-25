//
// Created by creeper on 22-12-13.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameSys.h"
/**
 *  read from metadata to start the game.
 *  if it fails, then return.
 */
void read_meta()
{
    FILE* fp = fopen("../assets/meta", "r");
    if(fp == NULL)
    {
        printf("Error: Fail to read metadata.\n");
        exit(-1);
    }

}

/**
 * save the current game to file.
 * @param state current state of the game.
 * @param log_head the head node of the log of this game. The log is recorded for traceback.
 * @param save_name the name of this save.
 * @return returns true if saving succeeds.
 */
//bool save(struct State* state, struct Log* log_head, char *save_name)
//{
//#define DIR_LENGTH 16
//    static char save_path[DIR_LENGTH + MAX_SAVE_NAME_LENGTH] = "../assets/saves/";//that should be enough for a path
//    strcat(save_path, save_name);
//    FILE* fp = fopen(save_path, "w+");
//    if(fp == NULL)
//    {
//        printf("Error: Fail to write to save file.\n");
//        return 0;
//    }
//
//#undef DIR_LENGTH
//}