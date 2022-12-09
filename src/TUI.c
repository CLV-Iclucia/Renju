//
// Created by creeper on 22-11-18.
//
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "GameCore.h"
#include "TUI.h"
#define MAX_BUFLINES 128
#define MAX_BUFCOLUMNS 512
struct TUIWidget* constructTUIWidget(struct Vec2i pos, int width, int height, bool show_bounding_box, const char* name, const char* content)
{
    if(pos.x + height >= MAX_BUFLINES || pos.y + width >= MAX_BUFCOLUMNS)
    {
        printf("Error: the size of TUIWidget cannot exceed 512");
        return NULL;
    }
    struct TUIWidget* tuiWidget;
    if(!(tuiWidget = (struct TUIWidget*)malloc(sizeof(struct TUIWidget))))
    {
        printf("Error: cannot construct new TUIWidget instance due to malloc failure.\n");
        return NULL;
    }
    tuiWidget->pos = pos;
    tuiWidget->width = width;
    tuiWidget->height = height;
    tuiWidget->show_bounding_box = show_bounding_box;
    int length = strlen(name);
    tuiWidget->content = (char*)malloc(length * sizeof(char));
    memcpy(tuiWidget->name, name, length * sizeof(char));
    length = strlen(content);
    tuiWidget->nxt = NULL;
    tuiWidget->content = (char*)malloc(length * sizeof(char));
    memcpy(tuiWidget->content, content, length * sizeof(char));
    return tuiWidget;
}

void deconstructTUIWidget(struct TUIWidget* tuiWidget)
{
    tuiWidget->nxt = NULL;
    free(tuiWidget->content);
    tuiWidget->content = NULL;
    free(tuiWidget);
}

struct TUIManager* constructTUIManager()
{
    struct TUIManager* tuiManager;
    if(!(tuiManager = (struct TUIManager*)malloc(sizeof(struct TUIManager))))
    {
        printf("Error: cannot construct new TUIManager instance due to malooc failure.\n");
        return NULL;
    }
    tuiManager->head = tuiManager->tail = NULL;
    tuiManager->width = tuiManager->height = 0;
    return tuiManager;
}

void addTUIWidgetInstance(struct TUIManager* tuiManager, struct TUIWidget* tuiWidget)
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

/**
 * almost the same as addTUIWidget.
 */
void addTUIWidget(struct TUIManager* tuiManager, struct Vec2i pos, int width, int height, bool show_bounding_box, const char* name, const char* content)
{
    struct TUIWidget* tuiWidget = constructTUIWidget(pos, width, height, show_bounding_box, name, content);
    if(tuiWidget == NULL)
    {
        printf("Error: construction of new TUIWidget instance fails. No new instance was added to TUIManager.\n");
        return ;
    }
    addTUIWidgetInstance(tuiManager, tuiWidget);
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
/**
 * get the character at coordinates (x, y)
 * @param tuiWidget
 * @param x
 * @param y
 * @return
 *      if the position (x, y) is outside the bounding box of tuiWidget, returns -1
 *      else returns the character to be displayed
 */
static char get_char_inside_window(struct TUIWidget* tuiWidget, int x, int y)
{
    int right_bound = tuiWidget->pos.x + tuiWidget->width + 1;
    int bottom_bound = tuiWidget->pos.y + tuiWidget->height + 1;
    if(x < tuiWidget->pos.x || x > bottom_bound || y < tuiWidget->pos.y || y > right_bound) return 0;
    bool at_corner_x = (x == tuiWidget->pos.x) || (x == bottom_bound);
    bool at_corner_y = (y == tuiWidget->pos.y) || (y == right_bound);
    if (tuiWidget->show_bounding_box)
    {
        if(at_corner_x && at_corner_y) return '+';
        else if (at_corner_x) return '-';
        else if (at_corner_y) return '|';
    }
    else if(at_corner_x || at_corner_y) return ' ';
    int offset = (x - tuiWidget->pos.x - 1) * tuiWidget->height + y - tuiWidget->pos.y - 1;
    return tuiWidget->content[offset];
}

void render(struct TUIManager* tuiManager)
{
    static char frame_buf[MAX_BUFLINES][MAX_BUFCOLUMNS];
    memset(frame_buf, 0, sizeof(frame_buf));
    clear_output();
    for(int i = 0; i < tuiManager->width; i++)
    {
        for(int j = 0; j < tuiManager->height; j++)
        {
            for(struct TUIWidget* tuiWidget = tuiManager->head; tuiWidget != NULL; tuiWidget = tuiWidget->nxt)
            {
                //frame_buf[i][j] = ;
            }
        }
    }
    for(int i = 0; i < tuiManager->width; i++)
        printf("%s\n", frame_buf[i]);
}
#undef MAX_BUFCOLUMNS
#undef MAX_BUFLINES