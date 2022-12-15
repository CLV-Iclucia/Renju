#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GamePlay.h"
#include "utils.h"
#include "TUI.h"
#ifdef DEMO_VERSION
const char main_title[] = " ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄ \n"
                          "▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌\n"
                          "▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌░▌     ▐░▌ ▀▀▀▀▀█░█▀▀▀ ▐░▌       ▐░▌\n"
                          "▐░▌       ▐░▌▐░▌          ▐░▌▐░▌    ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌ ▐░▌   ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░█▀▀▀▀█░█▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌   ▐░▌ ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░▌     ▐░▌  ▐░▌          ▐░▌    ▐░▌▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░▌      ▐░▌ ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌     ▐░▐░▌ ▄▄▄▄▄█░▌    ▐░█▄▄▄▄▄▄▄█░▌\n"
                          "▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌▐░░░░░░░▌    ▐░░░░░░░░░░░▌\n"
                          " ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀  ▀▀▀▀▀▀▀      ▀▀▀▀▀▀▀▀▀▀▀ \n";
const char demo_title[] = "                        DEMO VERSION                             \n";
const char     prompt[] = "               Press Enter to start a PVP game...                \n"
                          "                      Press Ctrl+C to quit                       \n";
#else
#define ENTRY_MAX_LENGTH 128
#define OPTION_CNT 5
static int save_cnt = 0;
static char theme[ENTRY_MAX_LENGTH];
static char *option[OPTION_CNT] = {"New Game", "Load Game", "Settings", "About", "Quit"};
static int opt_idx = 0;
static struct TUIManager* titleScreenManager;
struct Entry
{
    char name[ENTRY_MAX_LENGTH];
    char value[ENTRY_MAX_LENGTH];
    int n_value;
};
struct Entry* constructEntry(const char *meta_entry)
{
    struct Entry* entry = (struct Entry*)malloc(sizeof(struct Entry));
    if(entry == NULL)
    {
        printf("Error: cannot construct new Entry instance due to malloc failure.\n");
        return NULL;
    }
    int n = strlen(meta_entry), m;
    for(m = 0; m < n; m++)
        if(meta_entry[m] == ':')break;
    memcpy(entry->name, meta_entry, sizeof(char) * m);
    entry->name[m] = '\0';
    memcpy(entry->value, meta_entry + m + 1, sizeof(char) * (n - m));
    entry->n_value = n - m;
    return entry;
}
struct TUIManager* initTitleScreen()
{
    static char path[ENTRY_MAX_LENGTH << 1];
    memset(path, 0, sizeof(path));
    strcpy(path, "../assets/themes/");
    strcat(path, theme);
    FILE* fp = fopen(path, "r");
    if(fp == NULL)
    {
        printf("Error: Fail to read theme assets");
        exit(-1);
    }
    fscanf(fp, "%d %d", &)
    titleScreenManager = constructTUIManager();

    return titleScreenManager;
}
void newGame()
{

}
void load()
{

}
void settings()
{

}
void initGame()
{
    FILE* fp = fopen("../assets/meta", "r");
    if(fp == NULL)
    {
        printf("Error: Fail to read metadata. Unable to start game.\n");
        exit(-1);
    }
    char meta_entry[ENTRY_MAX_LENGTH];
    while(fscanf(fp, "%s", meta_entry))
    {
        struct Entry* entry = constructEntry(meta_entry);
        if(strcmp(entry->name, "theme") == 0)
            memcpy(theme, entry->value, sizeof(char) * entry->n_value);
        else if(strcmp(entry->name, "savings") == 0)
        {
            int i = 0;
            save_cnt = stringstream(entry->value, &i);
        }
    }
    fclose(fp);
}
#endif

void about()
{

}

bool processControl()
{
    int opt = getControlInput();
    if(opt == UNKNOWN) return true;
    else if(opt != CONFIRM)
    {
        setBounding(titleScreenManager, option[opt_idx], false);
        opt_idx += opt;
        if(opt_idx < 0) opt_idx = OPTION_CNT - 1;
        if(opt_idx == OPTION_CNT) opt_idx = 0;
        setBounding(titleScreenManager, option[opt_idx], true);
        return true;
    }
    if(strcmp(option[opt_idx], "New Game") == 0) newGame();
    else if(strcmp(option[opt_idx], "Load Game") == 0) load();
    else if(strcmp(option[opt_idx], "Settings") == 0) settings();
    else if(strcmp(option[opt_idx], "About") == 0) about();
    else if(strcmp(option[opt_idx], "Quit") == 0) return false;
    return true;
}

int main()
{
#ifndef DEMO_VERSION
    initGame();
    initTitleScreen();
#endif
    while(1)
    {
#ifdef DEMO_VERSION
        printf("%s%s%s", main_title, demo_title, prompt);
        char opt = getchar();
        if(opt == '\n') gameLoop(GAME_MODE_PVP, -1);
#else
        render(titleScreenManager);
        if(!processControl()) break;
#endif
    }
    clear_output();
    return 0;
}
