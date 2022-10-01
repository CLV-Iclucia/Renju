#include "GamePlay.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
extern int board[15];
void drawBoard(int lastPosX, int lastPosY)
{
    for(int i = 0; i < SIZE; i++)
    {
        printf("%2d", i + 1);
        for(int j = 0; j < SIZE; j++)
        {
            int status = GET(i, j);
            if(i == lastPosX && j == lastPosY)
            {
                if(status == BLACK)printf("■");
                if(status == WHITE)printf("□");
            }
            else
            {
                if(status == EMPTY)
                {
                    if(!i)
                    {
                        if(!j)printf("┌");
                        else if(j < SIZE - 1)printf("┬");
                        else printf("┐");
                    }
                    else if(i < SIZE - 1)
                    {
                        if(!j)printf("├");
                        else if(j < SIZE - 1)printf("┼");
                        else printf("┤");
                    }
                    else
                    {
                        if(!j)printf("└");
                        else if(j < SIZE - 1)printf("┴");
                        else printf("┘");
                    }
                }
                else if(status == BLACK)printf("●");
                else printf("◯");
            }
            if(j == SIZE - 1)putchar('\n');
        }
    }
    printf("  ");
    for(int i = 0; i < SIZE; i++)
        printf("%c", 'a' + i);
    putchar('\n');
}

ERROR_CODE parseCoord(char* coord, int* posX, int* posY)
{
    unsigned long n = strlen(coord);
    int Y_parsed = 0, X_parsed = 0, Y_first = 0;
    for(int i = 0; i < n;)
    {
        if(is_digit(coord[i]) && !X_parsed)
        {
            int pX = stringstream(coord, &i);
            if(!pX || pX > SIZE)
                return INVALID_INPUT_OUT_OF_RANGE;
            *posX = pX - 1;
            X_parsed = 1;
        }
        else if(is_alpha(coord[i]) && !Y_parsed)
        {
            if(!X_parsed) Y_first = 1;
            int pY;
            if(is_lowercase(coord[i]))
                pY = coord[i] - 'a';
            else if(is_capital(coord[i]))
                pY = coord[i] - 'A';
            if(pY >= SIZE)
                return INVALID_INPUT_OUT_OF_RANGE;
            *posY = pY;
            Y_parsed = 1;
            i++;
        }
        else return INVALID_INPUT_CANNOT_PARSE;
    }
    if(!X_parsed || !Y_parsed)return INVALID_INPUT_CANNOT_PARSE;
    if(GET(*posX, *posY) != EMPTY)
        return INVALID_INPUT_OVERLAP;
    return Y_first ? NOTHING_WRONG : INVALID_INPUT_BUT_PARSEABLE;
}