//
// Created by creeper on 22-11-18.
//
/**
 * @brief It is hard to make a cross-platform programme without external GUI libs.
 * But it is easy to try to implement a small TUI lib myself.
 * This may seem unnecessary, and it is indeed unnecessary but I'll do it anyway.
 * It just help manage certain scenes.
 * Though it might seem unnecessary in this project, I believe that it can be really useful for many TUI programmes.
 */
#ifndef RENJU_TUI_H
#define RENJU_TUI_H
#include "utils.h"
#include "GameCore.h"
#include "DataStructure.h"
#define ENTRY_MAX_LENGTH 128
#define MAX_IMAGE_SIZE 72
#define MAX_CURSOR_LEN 8
#define CHOOSE_TEXT 1
#define CHOOSE_OPTION 2

struct TUIManager;
/**
 * There are 3 kinds of TUIWidgets: Image, Text and Option
 * Image cannot be chosen, the other two can.
 * They are stored in TUIManager as a doubly-linked list.
 */
struct TUIWidget
{
    DOUBLY_LINKED_LIST(struct TUIWidget);
    struct OptionEntry *opt;///< enabled if and only if type=OPTION
    char **image;///< enabled if and only if type=IMAGE
    char *text;///< enabled if and only if type=TEXT or OPTION
    struct TUIManager* nxtTUI;///< enabled if and only if type=TEXT. Points to the TUI to be triggered when triggered.
    int width, height;///< the width and height of the image
    enum TYPE{TEXT = 0, OPTION, IMAGE} type;///< the type of this widget
    bool center;///< whether the widget is at the center of the screen.
};

/**
 * describes an entry of an Option TUIWidget, also stored as doubly-linked list
 */
struct OptionEntry
{
    char str[ENTRY_MAX_LENGTH];
    DOUBLY_LINKED_LIST(struct OptionEntry);
    int n;///< length of str
};

struct OptionEntry* constructOptionEntry(const char* str);
void addOptionEntry(struct TUIWidget* tuiWidget, struct OptionEntry* opt);
/**
 * a constructor for TUIWidget
 * @return a new TUIWidget instance with text
 */
struct TUIWidget* constructText(const char* content);
/**
 * a constructor for TUIWidget
 * @return a new TUIWidget instance with image
 */
struct TUIWidget* constructImage(int width, int height, char **image);
/**
 * a constructor for TUIWidget
 * @return a new TUIWidget instance with option
 */
struct TUIWidget* constructOption(const char* text);
/**
 * a destructor for TUIWidget
 */
void destructTUIWidget(struct TUIWidget *tuiWidget);

/**
 * @brief TUIManager manages the TUI widgets to be rendered in one frame. All the TUIWidgets are stored as linked list in TUIManager.
 *  Of course more advanced data structure (like KD-Tree, BSP and blocks) can be used to accelerate the rendering process.
 *  There is no such needs because the number of widgets needed to be rendered is actually very small in most cases...
 */
struct TUIManager
{
    struct TUIWidget *head;// the widget closer to the head will be drawn on the top in case of overlapping
    struct TUIWidget *tail;
    struct TUIWidget *cur;///< current widget being chosen
    void (*confirm)(struct TUIManager* this);///< enabled when there is option in this manager and will be triggered when confirmed.
    int choose_type;///< what kinds of widgets can be chosen, using bitmap.
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
static inline void bind(struct TUIWidget* tuiWidget, struct TUIManager* tuiManager) { tuiWidget->nxtTUI = tuiManager; }
/**
 * @return true if we need to quit from this TUI
 */
bool processControlSignal(struct TUIManager *tuiManager, int controlSignal);

struct Cursor
{
    char cursor_l[MAX_CURSOR_LEN + 1];
    char cursor_r[MAX_CURSOR_LEN + 1];
    int len_left, len_right;
};
void setCursor(struct Cursor* pCursor);
void resetCursor();
/**
 * render the UI based on TUIManager instance and Cursor instance
 * @param tuiManager
 * @param cursor
 */
void render(struct TUIManager* tuiManager);
void TUI(struct TUIManager* tuiManager);
#endif //RENJU_TUI_H