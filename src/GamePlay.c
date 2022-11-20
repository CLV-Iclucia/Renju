#include "GamePlay.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "GameAI.h"
#include <ctype.h>

void drawBoard(const int* board, int lastPosX, int lastPosY)
{
    for(int i = 0; i < SIZE; i++)
    {
        printf("%2d", i + 1);
        for(int j = 0; j < SIZE; j++)
        {
            int status = GET(board, i, j);
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
                        if(!j) printf("┌");
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

ERROR_CODE parseCoord(const int* board, char* coord, int* posX, int* posY)
{
    unsigned long n = strlen(coord);
    int Y_parsed = 0, X_parsed = 0, Y_first = 0;
    for(int i = 0; i < n;)
    {
        if(isdigit(coord[i]) && !X_parsed)
        {
            int pX = stringstream(coord, &i);
            if(!pX || pX > SIZE)
                return INVALID_INPUT_OUT_OF_RANGE;
            *posX = pX - 1;
            X_parsed = 1;
        }
        else if(isalpha(coord[i]) && !Y_parsed)
        {
            if(!X_parsed) Y_first = 1;
            int pY;
            if(islower(coord[i]))
                pY = coord[i] - 'a';
            else if(isupper(coord[i]))
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
    if(GET(board, *posX, *posY) != EMPTY)
        return INVALID_INPUT_OVERLAP;
    return Y_first ? NOTHING_WRONG : INVALID_INPUT_BUT_PARSEABLE;
}

/**
 *
 * @param gameMode
 * the mode of the game can either be PVP or PVE
 * @param AIColor
 * the color AI is using, enabled only when gameMode is set to PVE,
 */
void gameLoop(const int gameMode, const int AIColor)
{
    static int board[ALIGNMENT];
    memset(board, 0, sizeof(0));
    int lastPosX = -1, lastPosY = -1;
    int currentColor = BLACK;
    int winner = -1;
    while(1)
    {
        drawBoard(board, lastPosX, lastPosY);
        printf("Please input the coordinates.\n"
               "The alpha denoting the column(a~o) is in the front(both lowercase and uppercase are fine), \n"
               "and the number denoting the row(1~15) follows with no separation between.\n"
               "Now it is ");
        currentColor == BLACK ? printf("black's turn.\n") : printf("white' turn.\n");
        printf("input: ");
        char coord[4];
        getLine(coord, 3);
        int posX, posY;
        ERROR_CODE errorCode = parseCoord(board, coord, &posX, &posY);
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
            errorCode = parseCoord(board, coord, &posX, &posY);
        }
        if((gameMode == GAME_MODE_PVE && currentColor == AIColor) || gameMode == GAME_MODE_PVP)
        {
        //    playerPlace(board);
            PLACE(board, posX, posY, currentColor);
        }
    //    else AIPlace(board, AIColor);
        if((winner = checkWinner(board)) > 0) break;
        lastPosX = posX;
        lastPosY = posY;
        currentColor ^= 1;
    }
    if(winner == BLACK)
    {
        printf("Black wins!");
    }
    else
    {
        printf("White wins!");
    }
}