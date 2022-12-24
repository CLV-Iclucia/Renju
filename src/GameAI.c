//
// Created by creeper on 22-11-18.
//
#include "GameCore.h"
#include "GameCheck.h"
#include "DataStructure.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define INSIDE(x,y) ((x) >= 0 && (y) >= 0 && (x) < SIZE && (y) < SIZE)
#define STEP(x, step, dir) (x+(step)*(d##x[dir]))
#define isEmpty(step) (!(local_l & (3 << ((idx + i + (step)) << 1))))
#define PLACEABLE(step) (INSIDE(STEP(x, i + (step), k), STEP(y, i + (step), k)) && isEmpty(step))
#define IS_EMPTY(x, y) (!(global_state->row[(x)] & (3 << ((y) << 1))))
#define update_valid(x_min, x_max, y_min, y_max) {for(int i = x_min; i <= x_max; i++) \
                                                    for(int j = y_min; j <= y_max; j++) \
                                                        if(GET(global_state, i, j) == EMPTY)\
                                                            estimateBlackPoint(i, j);}
#define MAX_BRANCHES 5
#define PRE_DFS(x, y) { int tmpLastPosX = lastPosX;    \
                        int tmpLastPosY = lastPosY;    \
                        lastPosX = x;                  \
                        lastPosY = y;                   \
                        depth++;                \
                        currentColor ^= 1;

#define POST_DFS(x, y) { lastPosX = tmpLastPosX;  \
                         lastPosY = tmpLastPosY;  \
                         depth--;                 \
                         currentColor ^= 1;    \
                         CLEAR(global_state, x, y);}

extern int checkWinner(struct State, int currentColor, int x, int y);
extern struct State* global_state;
extern const int dx[];
extern const int dy[];
extern const int RUSSIAN_ROULLETE;
static const int INF = 0x3f3f3f3f;
static const int ROUND = 3;
static const int MAX_DEPTH = ROUND << 1;
static const time_t return_time_second = 14;
static time_t return_time, timer;
static int depth, currentColor, alphaColor;
static int black_minx, black_miny, black_maxx, black_maxy, white_minx, white_miny, white_maxx, white_maxy;//to restrict the region for searching
static int black_tot, white_tot;
static int white_point[ALIGNMENT][ALIGNMENT], black_point[ALIGNMENT][ALIGNMENT];// store the point for every coord
static int lastPosX, lastPosY;
struct BinaryHeap* BH[MAX_DEPTH];

void initAI()
{
    srand(time(NULL));
    for(int i = 0; i < MAX_DEPTH; i++)
    {
        BH[i] = (struct BinaryHeap*) malloc(sizeof(struct BinaryHeap));
        BH[i]->sz = 0;
    }
}

void endAI()
{
    for(int i = 0; i < MAX_DEPTH; i++)
    {
        free(BH[i]);
        BH[i] = NULL;
    }
}

/**
 * 1. search the pattern around it and test whether FORM_* can be achieved
 *    this will not be performed on every empty place!
 *    We suppose that every move only affects the points in a small region, so we can recompute the points in this region.
 *    However, given the complexity of forbidden move, it is possible that the points will be affected on a large scale.
 *    So we will update all the points at a small probability. This will not affect the speed greatly, and the points
 *    are more believable.
 *    What's more, while testing live-threes, the first thing is to put a black stone to make it four, this may change
 *    the status of other empty places! However, I assume that simply putting a stone will not make an  which
 *     is originally invalid empty place suddenly valid for black player. However, this assumption is definitely wrong.
 *     Again, I will recompute the forbidden moves only at a small probability, most of the time I will simply query the
 *     saved result of the original state.
 * 2. for every place that is occupied with the other stone, test whether there is a dangerous pattern around
 *    and search for valid place to destroy the dangerous pattern and test whether DESTROY_* can be achieved
 *    This will only be performed on the place of the last move.
 * 3. use a heap to store places with the highest points
 * @note once a dangerous pattern is found I will terminate step 2.
 *       because if there is more than 2 dangerous patterns, then probably this state will lose...
 */


int eval_white_form(int i, int j)
{
    struct Cross cross = takeCross(global_state, i, j);
    if(checkWhiteRenju(cross))return FORM_RENJU;
    int live_three_cnt = countWhiteLiveThree(cross);
    int four_cnt = countWhiteFour(cross);
    int two_cnt = countWhiteTwo(cross);
    return live_three_cnt * FORM_LIVE_THREE + four_cnt * FORM_FOUR + two_cnt * FORM_TWO;
}

void estimate_white_form(int i, int j)
{
    struct Cross cross = takeCross(global_state, i, j);
    PLACE_CROSS(cross, WHITE);
    white_point[i][j] = eval_white_form(i, j);
    CLEAR_CROSS(cross);
}

void estimate_black_form(int i, int j)
{
    if(black_point[i][j] >= 0)
    {
        struct Cross cross = takeCross(global_state, i, j);
        PLACE_CROSS(cross, BLACK);
        int two_cnt = countBlackTwo(cross);
        black_point[i][j] += two_cnt * FORM_TWO;
        CLEAR_CROSS(cross);
    }
}

/**
 * evaluate the state for alpha player
 * alpha player will take the move with the highest point
 * @return the value of current global_state
 */
int eval_state()
{
    int max_white_val = -INF, max_black_val = -INF;
    for(int i = white_minx; i <= white_maxx; i++)
        for(int j = white_miny; j <= white_maxy; j++)
            if(GET(global_state, i, j) == WHITE)
                max_white_val = max(max_white_val, white_point[i][j]);
    for(int i = black_minx; i <= black_maxx; i++)
        for(int j = black_miny; j < black_maxy; j++)
            if(GET(global_state, i, j) == BLACK)
                max_black_val = max(max_black_val, black_point[i][j]);
    if(alphaColor == WHITE) return (max_white_val << 1) - max_black_val;
    else return (max_black_val << 1) - max_white_val;
}

int dfs(int alpha, int beta)
{
    //assume that the last move only affects a few places
    // 1. update the forbidden moves, note that while calculating update_valid
    //     some of the black points are already calculated
    if(rand() <= RUSSIAN_ROULLETE)
    {
        update_valid(max(0, lastPosX - 4), min(SIZE - 1, lastPosX + 4),
                     max(0, lastPosY - 4), min(SIZE - 1, lastPosY + 4));
    }
    else update_valid(0, SIZE - 1, 0, SIZE - 1);
    // 2. calculate the points for every empty place
    if(rand() <= RUSSIAN_ROULLETE)
    {
        for(int i = max(0, lastPosX - 4); i < min(SIZE - 1, lastPosX + 4); i++)
            for(int j = max(0, lastPosY - 4); j < min(SIZE - 1, lastPosY + 4); j++)
            {
                if(GET(global_state, i, j) == EMPTY)
                {
                    if(currentColor == WHITE) estimate_white_form(i, j);
                    else estimate_black_form(i, j);
                }
            }
    }
    else
    {
        for(int i = 0; i < SIZE; i++)
            for(int j = 0; j < SIZE; j++)
            {
                if(GET(global_state, i, j) == EMPTY)
                {
                    if(currentColor == WHITE) estimate_white_form(i, j);
                    else estimate_black_form(i, j);
                }
            }
    }
    if(currentColor == WHITE)
    {
        for(int i = max(black_minx - 1, 0); i < min(black_maxx + 1, SIZE - 1); i++)
            for (int j = max(black_minx - 1, 0); j < min(black_maxx + 1, SIZE - 1); j++)
                if (GET(global_state, i, j) == BLACK)
                    if (destroyBlackLiveThree(i, j)) break;
        for(int i = max(black_minx - 1, 0); i < min(black_maxx + 1, SIZE - 1); i++)
            for (int j = max(black_minx - 1, 0); j < min(black_maxx + 1, SIZE - 1); j++)
                if (GET(global_state, i, j) == BLACK)
                    if (destroyBlackFour(i, j)) break;
    }
    else
    {
        for(int i = max(white_minx, 0); i < min(white_maxx, SIZE - 1); i++)
            for (int j = max(white_minx - 1, 0); j < min(white_maxx + 1, SIZE - 1); j++)
                if (GET(global_state, i, j) == WHITE)
                    if (destroyWhiteLiveThree(i, j)) break;
        for(int i = max(white_minx - 1, 0); i < min(white_maxx + 1, SIZE - 1); i++)
            for (int j = max(white_minx - 1, 0); j < min(white_maxx + 1, SIZE - 1); j++)
                if (GET(global_state, i, j) == BLACK)
                    if (destroyWhiteFour(i, j)) break;
    }
    if(depth == MAX_DEPTH)// 3. evaluate the state and return if reaching MAX_DEPTH
        return eval_state();
    else// 4. else we search the best moves based on the points of every place
    {
        int black_save[ALIGNMENT][ALIGNMENT], white_save[ALIGNMENT][ALIGNMENT];
        memcpy(black_save, black_point, sizeof(black_point));
        memcpy(white_save, white_point, sizeof(white_point));
        clear(BH[depth]);
        for(int x = 0; x < SIZE; x++)
        {
            for(int y = 0; y < SIZE; y++)
            {
                if(IS_EMPTY(x, y))//first insert the valid place
                {
                    if(currentColor == BLACK)
                    {
                        if(black_point[x][y] >= 0)insert(BH[depth], x, y, black_point[x][y]);
                    }
                    else insert(BH[depth], x, y, white_point[x][y]);
                }
            }
        }
        depth++;
        for(int i = 0; i < MAX_BRANCHES; i++)
        {
            struct HeapNode nd = top(BH[depth]);
            pop(BH[depth]);
            if(empty(BH[depth])) break;
            lastPosX = nd.x;
            lastPosY = nd.y;
            PLACE(global_state, nd.x, nd.y, currentColor);
            if(currentColor == alphaColor)
            {
                alpha = max(alpha, dfs(alpha, beta));
                currentColor ^= 1;
                if(alpha > beta)//alpha-beta pruning
                {
                    CLEAR(global_state, nd.x, nd.y);
                    depth--;
                    memcpy(black_point, black_save, sizeof(black_save));
                    memcpy(white_point, white_save, sizeof(white_save));
                    currentColor ^= 1;
                    return INF;
                }
            }
            else
            {
                currentColor ^= 1;
                beta = min(beta, dfs(alpha, beta));
                if(alpha > beta)
                {
                    CLEAR(global_state, nd.x, nd.y);
                    depth--;
                    memcpy(black_point, black_save, sizeof(black_save));
                    memcpy(white_point, white_save, sizeof(white_save));
                    currentColor ^= 1;
                    return -INF;
                }
            }
            CLEAR(global_state, nd.x, nd.y);
            memcpy(black_point, black_save, sizeof(black_save));
            memcpy(white_point, white_save, sizeof(white_save));
        }
        depth--;
    }
}

void AIPlace(struct State* state, int color)
{
    global_state = state;
    alphaColor = currentColor = color;
    black_minx = black_miny = white_minx = white_miny = SIZE;
    black_maxx = black_maxy = white_maxx = white_maxy = 0;
    for(int i = 0 ; i < SIZE ; i++)
        for(int j = 0; j < SIZE; j++)
            if(GET(global_state, i, j) == BLACK) black_tot++;
            else white_tot++;
    for(int i = SIZE - 1; i >= 0; i--)
        for(int j = SIZE - 1; j >= 0; j--)
            if(GET(global_state, i, j) == BLACK)
            {
                black_maxx = i;
                black_maxy = j;
            }
            else if(GET(global_state, i, j) == WHITE)
            {
                white_maxx = i;
                white_maxy = j;
            }
    for(int i = 0; i <= SIZE - 1; i++)
        for(int j = 0; j <= SIZE - 1; j++)
            if(GET(global_state, i, j) == BLACK)
            {
                black_minx = i;
                black_miny = j;
            }
            else if(GET(global_state, i, j) == WHITE)
            {
                white_minx = i;
                white_miny = j;
            }
    dfs(-INF, INF);
}

#undef isEmpty
#undef PLACEABLE
#undef INSIDE
#undef STEP