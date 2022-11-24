#include <stdio.h>
#include "GamePlay.h"
#include "utils.h"
#include "TUI.h"
const int CLR[15] = {~3, ~(3 << 2), ~(3 << 4), ~(3 << 6), ~(3 << 8), ~(3 << 10), ~(3 << 12), ~(3 << 14), ~(3 << 16), ~(3 << 18), ~(3 << 20),
         ~(3 << 22), ~(3 << 24), ~(3 << 26), ~(3 << 28)};//use these constants to clear the board
int row[SIZE], col[SIZE], diagL[SIZE<<1], diagR[SIZE<<1];//00 stands for no chess, 10 stands for black, 11 stands for white.So I can use "COLOR^=1" to change color.
//const int THREE[6] = {252, 168, 942, 648, 828, 552};
//const int FOUR[6] = {};
const char main_title[] = " ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄ \n"
                          "▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌\n"
                          "▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌░▌     ▐░▌ ▀▀▀▀▀█░█▀▀▀ ▐░▌       ▐░▌\n"
                          "▐░▌       ▐░▌▐░▌          ▐░▌▐░▌    ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌ ▐░▌   ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░█▀▀▀▀█░█▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌   ▐░▌ ▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░▌     ▐░▌  ▐░▌          ▐░▌    ▐░▌▐░▌      ▐░▌    ▐░▌       ▐░▌\n"
                          "▐░▌      ▐░▌ ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌     ▐░▐░▌ ▄▄▄▄▄█░▌    ▐░█▄▄▄▄▄▄▄█░▌\n"
                          "▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌▐░░░░░░░▌    ▐░░░░░░░░░░░▌\n"
                          " ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀  ▀▀▀▀▀▀▀      ▀▀▀▀▀▀▀▀▀▀▀ \n";
//struct TUIManager* initTitleScreen()
//{
    //struct TUIManager* titleScreenManager = constructTUIManager();
    //addTUIWidgetBack(titleScreenManager, main_title);
    //addTUIWidgetBack(titleScreenManager, "New Game");
    //addTUIWidgetBack(titleScreenManager, "Load Game");
    //addTUIWidgetBack(titleScreenManager, "Settings");
    //addTUIWidgetBack(titleScreenManager, "Quit");
    //return titleScreenManager;
//}
int main()
{
    //FILE* fp;
    //struct TUIManager* titleScreenManager = initTitleScreen();
    //while(1)
    //{
        //render(titleScreenManager);
        //gameLoop();
        //getchar();
    //destructTUIManager(titleScreenManager);
    //}
    return 0;
}
