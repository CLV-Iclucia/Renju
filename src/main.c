#include "TUI.h"
#include "GamePlay.h"
#include "stdio.h"
extern void init();
extern void clean();
extern struct TUIManager* titleScreenManager;
int main()
{
    init();
    TUI(titleScreenManager);
    clean();
    clear_output();
//    gameLoop(AI, AI);
}