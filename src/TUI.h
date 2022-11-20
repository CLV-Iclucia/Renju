//
// Created by creeper on 22-11-18.
//
/**
 * \brief
 *      It is hard to make a cross-platform programme without external GUI libs.
 *      But it is easy to try to implement a small TUI lib myself.
 */
#ifndef RENJU_TUI_H
#define RENJU_TUI_H
#include "utils.h"
#include "GameCore.h"
struct TUIWidget
{
    struct TUIWidget *nxt;
    struct Vec2i pos;///< the position of the center of the bounding box of the widget
    int width, height;///< the width and length of the bounding box
    BOOL show_bounding_box;///< whether the bounding box is shown
    char *name;///< the name of the TUIWidget, used for identifying widgets
    char *content;/// the text to be shown in the bounding box
};

struct TUIWidget* constructTUIWidget(struct Vec2i, int, int, BOOL, const char*, const char*);
void deconstructTUIWidget(struct TUIWidget*);

/**
 * @brief
 *  TUIManager manages the TUI widgets to be rendered in one frame.
 *  All the TUIWidgets are stored as linked list in TUIManager.
 *  Of course more advanced data structure (like KD-Tree, BSP and blocks) can be used to accelerate the rendering process.
 *  There is no such needs because the number of widgets needed to be rendered is actually very small in most cases...
 */
struct TUIManager
{
    int width, height;
    struct TUIWidget *head;/// the widget closer to the head will be drawn on the top in case of overlapping
    struct TUIWidget *tail;
};
struct TUIManager* constructTUIManager();
void deconstructTUIManager(struct TUIManager*);


void addTUIWidget(struct TUIManager *tuiManager, struct Vec2i pos, int width, int height, BOOL show_bounding_box, const char *name, const char *content);
void addTUIWidgetBack(struct TUIManager*, struct Vec2i, int, int, BOOL, const char*, const char*);
void render(struct TUIManager*);

#endif //RENJU_TUI_H
