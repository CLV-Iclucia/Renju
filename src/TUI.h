//
// Created by creeper on 22-11-18.
//
/**
 * @brief It is hard to make a cross-platform programme without external GUI libs.
 * But it is easy to try to implement a small TUI lib myself.
 * This may seem unnecessary, and it is indeed unnecessary but I'll do it anyway.
 * It just help manage certain scenes.
 */
#ifndef RENJU_TUI_H
#define RENJU_TUI_H
#include "utils.h"
#include "GameCore.h"
#define MAX_IMAGE_SIZE 72
#define MAX_CURSOR_LEN 8
struct TUIWidget
{
    struct TUIWidget *nxt;
    char *name;///< the name of the TUIWidget, used for identifying widgets
    char **image;
    char *text;
    int width, height;///< the width and height of the image
    enum TYPE{TEXT, IMAGE} type;
    bool chosen;
    bool center;
};
/**
 * a constructor for TUIWidget
 * @return a new TUIWidget instance with text
 */
struct TUIWidget* constructText(const char*, const char*);
/**
 * a constructor for TUIWidget
 * @return a new TUIWidget instance with image
 */
struct TUIWidget* constructImage(int, int, const char*, char**);

/**
 * a destructor for TUIWidget
 */
void deconstructTUIWidget(struct TUIWidget*);

/**
 * @brief TUIManager manages the TUI widgets to be rendered in one frame. All the TUIWidgets are stored as linked list in TUIManager.
 *  Of course more advanced data structure (like KD-Tree, BSP and blocks) can be used to accelerate the rendering process.
 *  There is no such needs because the number of widgets needed to be rendered is actually very small in most cases...
 */
struct TUIManager
{
    struct TUIWidget *head;// the widget closer to the head will be drawn on the top in case of overlapping
    struct TUIWidget *tail;
};
/**
 * a constructor for TUIManager
 * @return a new TUIManager instance
 */
struct TUIManager* constructTUIManager();
/**
 * a destructor for TUIManager
 */
void destructTUIManager(struct TUIManager*);

void addTUIWidgetBack(struct TUIManager *tuiManager, struct TUIWidget* tuiWidget);
void addTUIWidgetFront(struct TUIManager *tuiManager, struct TUIWidget* tuiWidget);
void setChosen(struct TUIManager *tuiManager, const char *name, bool chosen);

struct Cursor
{
    char cursor_l[MAX_CURSOR_LEN + 1];
    char cursor_r[MAX_CURSOR_LEN + 1];
    int len_left, len_right;
};
/**
 * render the UI based on TUIManager instance and Cursor instance
 * @param tuiManager
 * @param cursor
 */
void render(struct TUIManager* tuiManager, struct Cursor* cursor);
#endif //RENJU_TUI_H
