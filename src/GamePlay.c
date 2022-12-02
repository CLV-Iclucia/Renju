#include "GamePlay.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "GameAI.h"
#include <ctype.h>
#define NOTHING_WRONG 0
#define INVALID_INPUT_BUT_PARSEABLE 1
#define INVALID_INPUT_CANNOT_PARSE 2
#define INVALID_INPUT_OUT_OF_RANGE 3
#define INVALID_INPUT_OVERLAP 4

static const int WHITE_RENJU_MASK = 682;///the bitmask of five white stones in a row
static int lastPosX, lastPosY;

/**
 * draw the state based on current state and the last move.
 */
void drawBoard(const struct State* state)
{
    for(int i = 0; i < SIZE; i++)
    {
        printf("%2d", i + 1);
        for(int j = 0; j < SIZE; j++)
        {
            int status = GET(state, i, j);
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

/**
 * parsing the input string to get where the stone is to be placed
 * @return
 *      if the input is valid, then returns NOTHING_WRONG,
 *      else returns the corresponding error code.
 */
static ERROR_CODE parseCoord(const struct State* state, char* coord, int* posX, int* posY)
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
    if(GET(state, *posX, *posY) != EMPTY)
        return INVALID_INPUT_OVERLAP;
    return Y_first ? NOTHING_WRONG : INVALID_INPUT_BUT_PARSEABLE;
}

/**
 * process the input of coordinates and parse the input
 * @return a Vec2i instance with first element as the x-coordinate and the second element as the y-coordinate
 */
static struct Vec2i inputCoord(const struct State* state, int currentColor)
{
    char coord[4];
    getLine(coord, 3);
    int posX, posY;
    ERROR_CODE errorCode = parseCoord(state, coord, &posX, &posY);
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
        errorCode = parseCoord(state, coord, &posX, &posY);
    }
    lastPosX = posX;
    lastPosY = posY;
    return (struct Vec2i){posX, posY};
}

static void printPrompt(int currentColor)
{
    printf("Please input the coordinates.\n"
           "The alpha denoting the column(a~o) is in the front(both lowercase and uppercase are fine), \n"
           "and the number denoting the row(1~15) follows with no separation between.\n"
           "Now it is ");
    currentColor == BLACK ? printf("black's turn.\n") : printf("white' turn.\n");
    printf("input: ");
}

void playerPlace(struct State* const state, const int currentColor)
{
    struct Vec2i pos = inputCoord(state, currentColor);
    PLACE(state, pos.x, pos.y, currentColor);
}

/**
 * the main game loop
 * @param gameMode
 * the mode of the game, can either be PVP or PVE
 * @param AIColor
 * the color AI is using, enabled only when gameMode is set to PVE.
 */
void gameLoop(const int gameMode, const int AIColor)
{
    struct State* state = constructState();
    lastPosX = -1, lastPosY = -1;
    int currentColor = BLACK;
    int winner = -1;
    while(1)
    {
        clear_output();
        drawBoard(state);
        if((gameMode == GAME_MODE_PVE && currentColor == AIColor) || gameMode == GAME_MODE_PVP)
        {
            printPrompt(currentColor);
            playerPlace(state, currentColor);
        }
        else AIPlace(state, AIColor);
        if((winner = checkWinner(state, currentColor)) > 0) break;
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



/**
 * Judge the winner of the game based on current state and current color
 * @param currentColor
 * current color. Given its position in gameLoop, the player of this color should have made its move.
 * Passing this argument can reduce some unnecessary calculation, because the other color can't have won in most cases so we don't need to consider it.
 * The only exception occurs when the black player make a forbidden move.
 * @return the ending code of current state
 */
ENDING_CODE checkWinner(struct State* const state, int currentColor)
{
    if(currentColor == WHITE)//In this case, the black player cannot win
    {
        if(matchPattern(state, RENJU_LENGTH, WHITE_RENJU_MASK))
            return ENDING_WHITE_WIN;
    }
    else//The only case that white player win is that the black player made a forbidden move.
    {
        return ENDING_NOT_END;
    }
    return ENDING_NOT_END;
}