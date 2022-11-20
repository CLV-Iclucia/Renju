//
// Created by creeper on 22-11-18.
//
#include <malloc.h>
#include <string.h>
#include "GameCore.h"
#include "TUI.h"
struct TUIWidget* constructTUIWidget(struct Vec2i pos, int width, int height, BOOL show_bounding_box, const char* name, const char* content)
{
    struct TUIWidget* tuiWidget = (struct TUIWidget*)malloc(sizeof(struct TUIWidget));
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
    free(tuiWidget);
}

struct TUIManager* constructTUIManager()
{
    struct TUIManager* tuiManager = (struct TUIManager*)malloc(sizeof(struct TUIManager));
    tuiManager->head = tuiManager->tail = NULL;
    return tuiManager;
}

void addTUIWidget(struct TUIManager* tuiManager, struct Vec2i pos, int width, int height, BOOL show_bounding_box, const char* name, const char* content)
{
    struct TUIWidget* tuiWidget = constructTUIWidget(pos, width, height, show_bounding_box, name, content);
    if(tuiManager->head == NULL) tuiManager->head = tuiManager->tail = tuiWidget;
    else
    {
        tuiWidget->nxt = tuiManager->head;
        tuiManager->head = tuiWidget;
    }
}

void deconstructTUIManager(struct TUIManager* tuiManager)
{
    struct TUIWidget* tuiWidget = tuiManager->head;
    while(tuiWidget != NULL)
    {
        struct TUIWidget* nxtWidget = tuiWidget->nxt;
        deconstructTUIWidget(tuiWidget);
        tuiWidget = nxtWidget;
    }
    free(tuiManager);
}

static char get_char_inside_window(struct TUIWidget* tuiWidget, int x, int y)
{
    int right_bound = tuiWidget->pos.x + tuiWidget->width + 1;
    int bottom_bound = tuiWidget->pos.y + tuiWidget->height + 1;
    if(x < tuiWidget->pos.x || x > bottom_bound || y < tuiWidget->pos.y || y > right_bound) return 0;
    BOOL at_corner_x = (x == tuiWidget->pos.x) || (x == bottom_bound);
    BOOL at_corner_y = (y == tuiWidget->pos.y) || (y == right_bound);
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
    for(int i = 0; i < tuiManager->width; i++)
    {
        for(int j = 0; j < tuiManager->height; j++)
        {
            for(struct TUIWidget* tuiWidget = tuiManager->head; tuiWidget != NULL; tuiWidget = tuiWidget->nxt)
            {
                char ch = get_char_inside_window(tuiWidget, i, j);
                if(ch > 0)
                {
                    putchar(ch);
                    break;
                }
            }
        }
        printf("\n");
    }
}