#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "GamePlay.h"
#include "utils.h"
const int CLR[15] = {~3, ~(3 << 2), ~(3 << 4), ~(3 << 6), ~(3 << 8), ~(3 << 10), ~(3 << 12), ~(3 << 14), ~(3 << 16), ~(3 << 18), ~(3 << 20),
         ~(3 << 22), ~(3 << 24), ~(3 << 26), ~(3 << 28)};//use these constants to clear the board
int row[SIZE], col[SIZE], diagL[SIZE<<1], diagR[SIZE<<1];//00 stands for no chess, 10 stands for black, 11 stands for white.So I can use "COLOR^=1" to change color.
//const int THREE[6] = {252, 168, 942, 648, 828, 552};
//const int FOUR[6] = {};
//int checkDanger(int color) //At current color, we check if there is any "dangerous point"
//{
//    for(int i = 0; i < SIZE; ++i)
//    {
//        for(int j = 1; j < ; ++j)
//        {
//
//        }
//    }
//    for(int i = 0; i < SIZE; ++i)
//    {
//        for(int j = 1; j < ; ++j)
//        {
//
//        }
//    }
//}
//int check(int i, int j)// is it valid to place a black stone on (i, j)?
//{
//
//}
int board[15];
int main()
{
    int currentColor = BLACK;
    int lastPosX = -1, lastPosY = -1;
    while(1)
    {
        drawBoard(lastPosX, lastPosY);
        printf("Please input the coordinates.\n"
               "The alpha denoting the column(a~o) is in the front(both lowercase and uppercase are fine), \n"
               "and the number denoting the row(1~15) follows with no separation between.\n"
               "Now it is ");
        currentColor == BLACK ? printf("black's turn.\n") : printf("white' turn.\n");
        printf("input: ");
        char coord[4];
        getLine(coord, 3);
        int posX, posY;
        ERROR_CODE errorCode = parseCoord(coord, &posX, &posY);
        while(errorCode != NOTHING_WRONG)
        {
            if(errorCode == INVALID_INPUT_CANNOT_PARSE)
                printf("Oops, your input is invalid and can't be parsed. Please input again in the correct form below.\n");
            else if(errorCode == INVALID_INPUT_BUT_PARSEABLE)
            {
                printf("You seem to have input with row first, and it can still be parsed.\n"
                       "A stone will be placed at %c%d\n"
                       "Press any key to confirm this move", posY + 'a', posX + 1);
                getchar();
                break;
            }
            else if(errorCode == INVALID_INPUT_OUT_OF_RANGE)
                printf("The coordinate you have input is out of range. Please input again in the correct form below.\n");
            else printf("There is already a stone at the coordinate you have input.\n"
                        "Please input again in the correct form and make sure the coordinate you input is empty.\n");
            printf("input: ");
            getLine(coord, 3);
            errorCode = parseCoord(coord, &posX, &posY);
        }
        PLACE(posX, posY, currentColor);
        lastPosX = posX;
        lastPosY = posY;
        currentColor ^= 1;
    }
    return 0;
}
