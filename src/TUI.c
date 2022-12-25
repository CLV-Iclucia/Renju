//
// Created by creeper on 22-11-18.
//
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "GameCore.h"
#include "TUI.h"
#define MAX_BUFCOLUMNS 96
#define initBase(tuiWidget) tuiWidget->prv = NULL;\
                        tuiWidget->nxt = NULL;    \
                        tuiWidget->opt = NULL;    \
                        tuiWidget->center = true; \
                        tuiWidget->nxtTUI = NULL;
static struct Cursor* cursor;
struct TUIWidget* constructText(const char* content)
{
    struct TUIWidget* tuiWidget;
    if(!(tuiWidget = (struct TUIWidget*)malloc(sizeof(struct TUIWidget))))
    {
        printf("Error: cannot construct new TUIWidget instance due to malloc failure.\n");
        return NULL;
    }
    tuiWidget->type = TEXT;
    int length = strlen(content);
    tuiWidget->image = NULL;
    tuiWidget->width = 0;
    tuiWidget->height = length;
    initBase(tuiWidget);
    tuiWidget->text = (char*)malloc(length + 1);
    memcpy(tuiWidget->text, content, length + 1);
    return tuiWidget;
}
struct TUIWidget* constructImage(int width, int height, char** image)
{
    struct TUIWidget* tuiWidget;
    if(!(tuiWidget = (struct TUIWidget*)malloc(sizeof(struct TUIWidget))))
    {
        printf("Error: cannot construct new TUIWidget instance due to malloc failure.\n");
        return NULL;
    }
    tuiWidget->type = IMAGE;
    tuiWidget->image = (char**)malloc(width * sizeof(char*));
    tuiWidget->width = width;
    tuiWidget->height = height;
    initBase(tuiWidget);
    tuiWidget->text = NULL;
    for(int i = 0; i < width; i++)
    {
        tuiWidget->image[i] = (char*)malloc(height * 3 + 1);
        memcpy(tuiWidget->image[i], image[i], height * 3 + 1);
    }
    return tuiWidget;
}

struct TUIWidget* constructOption(const char* text)
{
    struct TUIWidget* tuiWidget = constructText(text);
    tuiWidget->type = OPTION;
    return tuiWidget;
}

struct OptionEntry *constructOptionEntry(const char *str)
{
    struct OptionEntry* optionEntry = (struct OptionEntry*) malloc(sizeof(struct OptionEntry));
    optionEntry->n = strlen(str);
    memcpy(optionEntry->str, str, optionEntry->n + 1);
    optionEntry->prv = optionEntry->nxt = NULL;
    return optionEntry;
}
/**
 * add an option entry to an Option Widget instance
 */
void addOptionEntry(struct TUIWidget* tuiWidget, struct OptionEntry* opt)
{
    if(tuiWidget->type != OPTION)
    {
        printf("Warning: cannot add an OptionEntry to a non-option widget. Skipped.\n");
        return ;
    }
    if(opt == NULL)
    {
        printf("Warning: a NULL OptionEntry instance is being added to the TUIWidget. Skipped.\n");
        return ;
    }
    if(tuiWidget->opt == NULL) tuiWidget->opt = opt;
    else
    {
        LINK(opt, tuiWidget->opt);
        tuiWidget->opt = opt;
    }
}

void destructTUIWidget(struct TUIWidget* tuiWidget)
{
    if(tuiWidget->type == OPTION)
    {
        struct OptionEntry* optionEntry = tuiWidget->opt->nxt;
        while(optionEntry->prv != NULL) optionEntry = optionEntry->prv;
        while(optionEntry != NULL)
        {
            struct OptionEntry* nxtOpt = optionEntry->nxt;
            free(optionEntry);
            optionEntry = nxtOpt;
            if(optionEntry) optionEntry->prv = NULL;
        }
    }
    DELETE(tuiWidget->text);
    for(int i = 0; i < tuiWidget->width; i++) { DELETE(tuiWidget->image[i]); }
    DELETE(tuiWidget->image);
    tuiWidget->nxtTUI = NULL;
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
    //by default, only the option widgets and text widgets can be chosen by cursor.
    tuiManager->choose_type = CHOOSE_TEXT | CHOOSE_OPTION;
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
        LINK(tuiWidget, tuiManager->head);
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
        LINK(tuiManager->tail, tuiWidget);
        tuiManager->tail = tuiWidget;
    }
}

void destructTUIManager(struct TUIManager* tuiManager)
{
    struct TUIWidget* tuiWidget = tuiManager->head;
    while(tuiWidget != NULL)
    {
        struct TUIWidget* nxtWidget = tuiWidget->nxt;
        destructTUIWidget(tuiWidget);
        tuiWidget = nxtWidget;
    }
    free(tuiManager);
}

bool processControlSignal(struct TUIManager *tuiManager, int controlSignal)
{
    if(tuiManager->cur == NULL) return true;
    switch (controlSignal)
    {
        case SIGNAL_UP:
            do
            {
                if(tuiManager->cur == tuiManager->head) tuiManager->cur = tuiManager->tail;
                else tuiManager->cur = tuiManager->cur->prv;
            }while(!(tuiManager->choose_type & (1 << (tuiManager->cur->type))));
            break;
        case SIGNAL_DOWN:
            do
            {
                if(tuiManager->cur == tuiManager->tail) tuiManager->cur = tuiManager->head;
                else tuiManager->cur = tuiManager->cur->nxt;
            }while(!(tuiManager->choose_type & (1 << (tuiManager->cur->type))));
            break;
        case SIGNAL_LEFT:
            if(tuiManager->cur->type == OPTION && tuiManager->cur->opt->prv != NULL)
                tuiManager->cur->opt = tuiManager->cur->opt->prv;
            break;
        case SIGNAL_RIGHT:
            if(tuiManager->cur->type == OPTION && tuiManager->cur->opt->nxt != NULL)
                tuiManager->cur->opt = tuiManager->cur->opt->nxt;
            break;
        case SIGNAL_CONFIRM:
            if(tuiManager->cur->type == OPTION)
            {
                if(tuiManager->confirm != NULL)tuiManager->confirm(tuiManager);
                else return true;
            }
            else
            {
                if(tuiManager->cur->nxtTUI != NULL)
                    TUI(tuiManager->cur->nxtTUI);
                else return true;
            }
        default:
            break;
    }
    return false;
}
/**
 * render a Text TUIWidget based on an instance.
 * Because Text TUIWidget can be chosen, cursor also need to be considered when rendering it.
 */
static void renderText(struct TUIWidget* tuiWidget, bool chosen)
{
    if(chosen)
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
/**
 * render an Image TUIWidget based on an instance.
 * Because Image TUIWidget can be chosen, cursor also need to be considered when rendering it.
 */
static void renderImage(struct TUIWidget *tuiWidget)
{
    for(int i = 0; i < tuiWidget->width; i++)
    {
        if(tuiWidget->center)
            for(int j = 0; j < (MAX_BUFCOLUMNS - tuiWidget->height) >> 1; j++) putchar(' ');
        printf("%s\n", tuiWidget->image[i]);
    }
}
/**
 * render an Option TUIWidget based on an instance.
 * Because Option TUIWidget can be chosen, cursor also need to be considered when rendering it.
 */
static void renderOption(struct TUIWidget *tuiWidget, bool chosen)
{
    if(chosen)
    {
        if(tuiWidget->center)
        {
            int padding_len = ((MAX_BUFCOLUMNS - tuiWidget->height - tuiWidget->opt->n -
                                cursor->len_left - cursor->len_right - 1) >> 1);
            for(int i = 0; i < padding_len; i++) putchar(' ');
        }
        for(int i = 0; i < cursor->len_left; i++) putchar(cursor->cursor_l[i]);
        printf("%s:%s", tuiWidget->text, tuiWidget->opt->str);
        for(int i = 0; i < cursor->len_right; i++)
            putchar(cursor->cursor_r[i]);
        putchar('\n');
    }
    else
    {
        if(tuiWidget->center)
            for(int i = 0; i < (MAX_BUFCOLUMNS - tuiWidget->height - tuiWidget->opt->n) >> 1; i++) putchar(' ');
        printf("%s:%s\n", tuiWidget->text, tuiWidget->opt->str);
    }
}

void setCursor(struct Cursor* pCursor){ cursor = pCursor; }
void resetCursor(){ cursor = NULL; }

/**
 * render the screen based on a TUIManager instance. All the widgets in this instance will be rendered
 */
void render(struct TUIManager* tuiManager)
{
    clear_output();
    for(struct TUIWidget* tuiWidget = tuiManager->head; tuiWidget != NULL; tuiWidget = tuiWidget->nxt)
    {
        if(tuiWidget->type == TEXT)
            renderText(tuiWidget, tuiManager->cur == tuiWidget);
        else if(tuiWidget->type == IMAGE)
            renderImage(tuiWidget);
        else renderOption(tuiWidget, tuiManager->cur == tuiWidget);
    }
}

/**
 * render the screen based on a TUIManager instance and interact with the instance
 */
void TUI(struct TUIManager* tuiManager)
{
    while(1)
    {
        render(tuiManager);
        int controlSignal = getControlInput();
        if(processControlSignal(tuiManager, controlSignal)) return;
    }
}