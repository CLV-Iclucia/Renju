//
// Created by creeper on 22-11-18.
//
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "GameCore.h"
#include "TUI.h"
#define MAX_BUFCOLUMNS 96
struct TUIWidget* constructText(const char* name, const char* content)
{
    struct TUIWidget* tuiWidget;
    if(!(tuiWidget = (struct TUIWidget*)malloc(sizeof(struct TUIWidget))))
    {
        printf("Error: cannot construct new TUIWidget instance due to malloc failure.\n");
        return NULL;
    }
    tuiWidget->type = TEXT;
    int length = strlen(name);
    tuiWidget->name = (char*)malloc(length + 1);
    memcpy(tuiWidget->name, name,  length + 1);
    length = strlen(content);
    tuiWidget->nxt = NULL;
    tuiWidget->image = NULL;
    tuiWidget->width = 0;
    tuiWidget->height = length;
    tuiWidget->chosen = false;
    tuiWidget->center = true;
    tuiWidget->text = (char*)malloc(length + 1);
    memcpy(tuiWidget->text, content, length + 1);
    return tuiWidget;
}
struct TUIWidget* constructImage(int width, int height, const char* name, char** image)
{
    struct TUIWidget* tuiWidget;
    if(!(tuiWidget = (struct TUIWidget*)malloc(sizeof(struct TUIWidget))))
    {
        printf("Error: cannot construct new TUIWidget instance due to malloc failure.\n");
        return NULL;
    }
    tuiWidget->type = IMAGE;
    int length = strlen(name);
    tuiWidget->name = (char*)malloc(length + 1);
    memcpy(tuiWidget->name, name, length + 1);
    tuiWidget->image = (char**)malloc(width * sizeof(char*));
    tuiWidget->width = width;
    tuiWidget->height = height;
    tuiWidget->chosen = false;
    tuiWidget->center = true;
    for(int i = 0; i < width; i++)
    {
        tuiWidget->image[i] = (char*)malloc(height * 3 + 1);
        memcpy(tuiWidget->image[i], image[i], height * 3 + 1);
    }
    tuiWidget->nxt = NULL;
    tuiWidget->text = NULL;
    return tuiWidget;
}
void deconstructTUIWidget(struct TUIWidget* tuiWidget)
{
    tuiWidget->nxt = NULL;
    free(tuiWidget->text);
    tuiWidget->text = NULL;
    free(tuiWidget->image);
    tuiWidget->image = NULL;
    free(tuiWidget);
}

struct TUIManager* constructTUIManager()
{
    struct TUIManager* tuiManager;
    if(!(tuiManager = (struct TUIManager*)malloc(sizeof(struct TUIManager))))
    {
        printf("Error: cannot construct new TUIManager instance due to malloc failure.\n");
        return NULL;
    }
    tuiManager->head = tuiManager->tail = NULL;
    return tuiManager;
}

void addTUIWidgetFront(struct TUIManager* tuiManager, struct TUIWidget* tuiWidget)
{
    if(tuiWidget == NULL)
    {
        printf("Warning: a NULL TUIWidget instance is being added to the TUIManager. Skipped.\n");
        return ;
    }
    if(tuiManager->head == NULL) tuiManager->head = tuiManager->tail = tuiWidget;
    else
    {
        tuiWidget->nxt = tuiManager->head;
        tuiManager->head = tuiWidget;
    }
}

void addTUIWidgetBack(struct TUIManager* tuiManager, struct TUIWidget* tuiWidget)
{
    if(tuiWidget == NULL)
    {
        printf("Warning: a NULL TUIWidget instance is being added to the TUIManager. Skipped.\n");
        return ;
    }
    if(tuiManager->head == NULL) tuiManager->head = tuiManager->tail = tuiWidget;
    else
    {
        tuiManager->tail->nxt = tuiWidget;
        tuiManager->tail = tuiWidget;
    }
}

void destructTUIManager(struct TUIManager* tuiManager)
{
    struct TUIWidget* tuiWidget = tuiManager->head;
    while(tuiWidget != NULL)
    {
        struct TUIWidget* nxtWidget = tuiWidget->nxt;
        deconstructTUIWidget(tuiWidget);
        tuiWidget = nxtWidget;
    }
    free(tuiManager);
    tuiManager = NULL;
}

void setChosen(struct TUIManager* tuiManager, const char* name, bool chosen)
{
    for(struct TUIWidget* tuiWidget = tuiManager->head; tuiWidget; tuiWidget = tuiWidget->nxt)
    {
        if(strcmp(tuiWidget->name, name) == 0)
        {
            tuiWidget->chosen = chosen;
            return ;
        }
    }
}

void render(struct TUIManager* tuiManager, struct Cursor* cursor)
{
    clear_output();
    for(struct TUIWidget* tuiWidget = tuiManager->head; tuiWidget != NULL; tuiWidget = tuiWidget->nxt)
    {
        if(tuiWidget->type == TEXT)
        {
            if(tuiWidget->chosen)
            {
                if(tuiWidget->center)
                {
                    int padding_len = ((MAX_BUFCOLUMNS - tuiWidget->height - cursor->len_left
                                        - cursor->len_right) >> 1);
                    for(int i = 0; i < padding_len; i++) putchar(' ');
                }
                for(int i = 0; i < cursor->len_left; i++) putchar(cursor->cursor_l[i]);
                printf("%s", tuiWidget->text);
                for(int i = 0; i < cursor->len_right; i++)
                    putchar(cursor->cursor_r[i]);
                putchar('\n');
            }
            else
            {
                if(tuiWidget->center)
                    for(int i = 0; i < (MAX_BUFCOLUMNS - tuiWidget->height) >> 1; i++) putchar(' ');
                printf("%s\n", tuiWidget->text);
            }
        }
        else
        {
            for(int i = 0; i < tuiWidget->width; i++)
            {
                if(tuiWidget->center)
                    for(int j = 0; j < (MAX_BUFCOLUMNS - tuiWidget->height) >> 1; j++) putchar(' ');
                printf("%s\n", tuiWidget->image[i]);
            }
        }
    }
}