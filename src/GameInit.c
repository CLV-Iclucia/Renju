//
// Created by creeper on 22-12-15.
//
/**
 * The initializing processes are in this file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GamePlay.h"
#include "utils.h"
#include "TUI.h"
#define OPTION_CNT 5
#define INIT_ENTER_WIDGET(MANAGER,TEXT) struct TUIWidget* TEXT##Widget = constructText(#TEXT);\
                                            bind(TEXT##Widget, TEXT##ScreenManager);\
                                            addTUIWidgetBack(MANAGER, TEXT##Widget)
static int save_cnt = 0;
static char theme[ENTRY_MAX_LENGTH];
static struct TUIManager *loadScreenManager, *SettingsScreenManager, *AboutScreenManager, *newGameManager;
static char* buf[MAX_IMAGE_SIZE];
static struct Cursor* cursor;
struct TUIManager* titleScreenManager;
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

struct TUIWidget* initThemeImage(FILE* fp)
{
    int height = read_num(fp);
    int width = read_num(fp);
    for(int i = 0; i < width; i++)
    {
        if(buf[i] == NULL) buf[i] = (char*) malloc(MAX_IMAGE_SIZE * 3 + 1);
        fgetLine(fp, buf[i], height * 3);
    }
    return constructImage(width, height, buf);
}

struct Cursor* initThemeCursor(FILE* fp)
{
    struct Cursor* new_cursor = (struct Cursor*) malloc(sizeof(struct Cursor));
    new_cursor->len_left = read_num(fp);
    fgetLine(fp, new_cursor->cursor_l, new_cursor->len_left);
    new_cursor->len_right = read_num(fp);
    fgetLine(fp, new_cursor->cursor_r, new_cursor->len_right);
    return new_cursor;
}

void initTheme(const char* path)
{
    FILE* fp = fopen(path, "r");
    if(fp == NULL)
    {
        printf("Error: Fail to read theme assets.");
        exit(-1);
    }
    struct TUIWidget* main_title_widget = initThemeImage(fp);
    struct TUIWidget* setting_widget = initThemeImage(fp);
    struct TUIWidget* load_widget = initThemeImage(fp);
    struct TUIWidget* about_widget = initThemeImage(fp);
    cursor = initThemeCursor(fp);
    setCursor(cursor);
    addTUIWidgetBack(titleScreenManager, main_title_widget);
    addTUIWidgetBack(SettingsScreenManager, setting_widget);
    addTUIWidgetBack(loadScreenManager, load_widget);
    addTUIWidgetBack(AboutScreenManager, about_widget);
    fclose(fp);
}

void initTitleScreen()
{
    struct TUIWidget* newGameWidget = constructText("New Game");
    bind(newGameWidget, newGameManager);
    addTUIWidgetBack(titleScreenManager, newGameWidget);
    struct TUIWidget* loadGameWidget = constructText("Load Game");
    bind(loadGameWidget, loadScreenManager);
    addTUIWidgetBack(titleScreenManager, loadGameWidget);
    INIT_ENTER_WIDGET(titleScreenManager, Settings);
    INIT_ENTER_WIDGET(titleScreenManager, About);
    addTUIWidgetBack(titleScreenManager, constructText("Quit"));
    titleScreenManager->cur = titleScreenManager->head;
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
    while(fscanf(fp, "%s", meta_entry) == 1)
    {
        struct Entry* entry = constructEntry(meta_entry);
        if(strcmp(entry->name, "theme") == 0)
            memcpy(theme, entry->value, entry->n_value);
        else if(strcmp(entry->name, "savings") == 0)
        {
            int i = 0;
            save_cnt = stringstream(entry->value, &i);
        }
        free(entry);
    }
    fclose(fp);
    titleScreenManager = constructTUIManager();
    loadScreenManager = constructTUIManager();
    SettingsScreenManager = constructTUIManager();
    AboutScreenManager = constructTUIManager();
    newGameManager = constructTUIManager();
    static char path[ENTRY_MAX_LENGTH << 1];
    memset(path, 0, sizeof(path));
    strcpy(path, "../assets/themes/");
    strcat(path, theme);
    initTheme(path);
}
void initAboutScreen()
{
    addTUIWidgetBack(AboutScreenManager, constructText("About Renju"));
    addTUIWidgetBack(AboutScreenManager, constructText("CreeperIclucia-Vader"));
    addTUIWidgetBack(AboutScreenManager, constructText("Version: 1.0.0"));
    addTUIWidgetBack(AboutScreenManager, constructText("My github: https://github.com/CreeperLordVader-Iclucia-Ashina"));
    addTUIWidgetBack(AboutScreenManager, constructText("My Blog: https://creeperlordvader-iclucia-ashina.github.io"));
    addTUIWidgetBack(AboutScreenManager, constructText("Thanks for playing Renju! Welcome to my github and blog for more interesting things!"));
    addTUIWidgetBack(AboutScreenManager, constructText("Press any key to return to title screen."));
}
#define HUMAN 1
#define AI 0
static void confirm_game(struct TUIManager* GameManager)
{
    int black_player = -1;
    int white_player = -1;
    for(struct TUIWidget* tuiWidget = GameManager->head; tuiWidget; tuiWidget = tuiWidget->nxt)
    {
        if(strcmp(tuiWidget->text, "Black") == 0)
        {
            if(strcmp(tuiWidget->opt->str, "Human") == 0) black_player = HUMAN;
            else black_player = AI;
        }
        if(strcmp(tuiWidget->text, "White") == 0)
        {
            if(strcmp(tuiWidget->opt->str, "Human") == 0) white_player = HUMAN;
            else white_player = AI;
        }
    }
    if(black_player >= 0 && white_player >= 0)
    {
        if(black_player == HUMAN && white_player == HUMAN)
            gameLoop(GAME_MODE_PVP, -1);
        else if(black_player == HUMAN && white_player == AI)
            gameLoop(GAME_MODE_PVE, WHITE);
        else if(black_player == AI && white_player == HUMAN)
            gameLoop(GAME_MODE_PVE, BLACK);
        else printf("Error: EVE game haven't been supported yet.\n");
    }
}
#undef HUMAN
#undef AI
void initNewGameScreen()
{
    struct TUIWidget* BlackPlayer = constructOption("Black");
    addOptionEntry(BlackPlayer, constructOptionEntry("Human"));
    addOptionEntry(BlackPlayer, constructOptionEntry("AI"));
    struct TUIWidget* WhitePlayer = constructOption("White");
    addOptionEntry(WhitePlayer, constructOptionEntry("Human"));
    addOptionEntry(WhitePlayer, constructOptionEntry("AI"));
    addTUIWidgetBack(newGameManager, BlackPlayer);
    addTUIWidgetBack(newGameManager, WhitePlayer);
    addTUIWidgetBack(newGameManager, constructText("Back"));
    newGameManager->confirm = &confirm_game;
    newGameManager->cur = BlackPlayer;
}
void initLoadScreen()
{

}
void initSettingScreen()
{

}
void clean()
{
    destructTUIManager(titleScreenManager);
    destructTUIManager(loadScreenManager);
    destructTUIManager(SettingsScreenManager);
    destructTUIManager(AboutScreenManager);
    destructTUIManager(newGameManager);
    for(int i = 0; i < MAX_IMAGE_SIZE; i++)
        free(buf[i]);
    DELETE(cursor);
}

void init()
{
    initGame();
    initNewGameScreen();
    initTitleScreen();
    initLoadScreen();
    initSettingScreen();
    initAboutScreen();
}