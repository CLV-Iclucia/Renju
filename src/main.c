#include <stdio.h>
#include "GamePlay.h"
#include "utils.h"
#include "TUI.h"
const int CLR[15] = {~3, ~(3 << 2), ~(3 << 4), ~(3 << 6), ~(3 << 8), ~(3 << 10), ~(3 << 12), ~(3 << 14), ~(3 << 16), ~(3 << 18), ~(3 << 20),
         ~(3 << 22), ~(3 << 24), ~(3 << 26), ~(3 << 28)};//use these constants to clear the state
//const int THREE[6] = {252, 168, 942, 648, 828, 552};
//const int FOUR[6] = {};
const char main_title[] = " ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄ "
                          "▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌"
                          "▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌░▌     ▐░▌ ▀▀▀▀▀█░█▀▀▀ ▐░▌       ▐░▌"
                          "▐░▌       ▐░▌▐░▌          ▐░▌▐░▌    ▐░▌      ▐░▌    ▐░▌       ▐░▌"
                          "▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌ ▐░▌   ▐░▌      ▐░▌    ▐░▌       ▐░▌"
                          "▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌      ▐░▌    ▐░▌       ▐░▌"
                          "▐░█▀▀▀▀█░█▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌   ▐░▌ ▐░▌      ▐░▌    ▐░▌       ▐░▌"
                          "▐░▌     ▐░▌  ▐░▌          ▐░▌    ▐░▌▐░▌      ▐░▌    ▐░▌       ▐░▌"
                          "▐░▌      ▐░▌ ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌     ▐░▐░▌ ▄▄▄▄▄█░▌    ▐░█▄▄▄▄▄▄▄█░▌"
                          "▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌▐░░░░░░░▌    ▐░░░░░░░░░░░▌"
                          " ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀  ▀▀▀▀▀▀▀      ▀▀▀▀▀▀▀▀▀▀▀ ";
//struct TUIManager* initTitleScreen()
//{
//    struct TUIManager* titleScreenManager = constructTUIManager();
//    addTUIWidgetInstance(titleScreenManager, main_title);
//    addTUIWidgetInstance(titleScreenManager, "New Game");
//    addTUIWidgetInstance(titleScreenManager, "Load Game");
//    addTUIWidgetInstance(titleScreenManager, "Settings");
//    addTUIWidgetInstance(titleScreenManager, "Quit");
//    return titleScreenManager;
//}
int main()
{
    //FILE* fp;
    //struct TUIManager* titleScreenManager = initTitleScreen();
    while(1)
    {
        //render(titleScreenManager);
        gameLoop(GAME_MODE_PVP, -1);
    }
    return 0;
}
