//
// Created by creeper on 22-11-18.
//
#include "GameCore.h"
#include "GameCheckAndCount.h"
#include "DataStructure.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BRANCHES 5//at each layer search at most 3 choices.

#define update_valid(x_min, x_max, y_min, y_max) {for(int i = x_min; i <= x_max; i++) \
                                                    for(int j = y_min; j <= y_max; j++) \
                                                        if(GET(global_state, i, j) == EMPTY)\
                                                            estimateBlackPoints(i, j);}

#define UPDATE_MIN_MAX(color, axis, nd) color##_min##axis = min(color##_min##axis, nd.axis);\
                                        color##_max##axis = max(color##_max##axis, nd.axis)

#define UPDATE_ALL_MIN_MAX(nd) UPDATE_MIN_MAX(black, x, nd);\
                                UPDATE_MIN_MAX(black, y, nd); \
                                UPDATE_MIN_MAX(white, x, nd); \
                                UPDATE_MIN_MAX(white, y, nd)

#define RESTORE_MIN_MAX(color, axis) color##_min##axis = color##_save_min##axis;\
                                       color##_max##axis = color##_save_max##axis

#define RESTORE_ALL_MIN_MAX     RESTORE_MIN_MAX(black, x);\
                                RESTORE_MIN_MAX(black, y); \
                                RESTORE_MIN_MAX(white, x); \
                                RESTORE_MIN_MAX(white, y)

#define BEFORE_SEARCH_CHILD        int black_save[ALIGNMENT][ALIGNMENT], white_save[ALIGNMENT][ALIGNMENT]; \
                                    int tmpLastPosX = lastPosX;    \
                                    int tmpLastPosY = lastPosY;    \
                                    memcpy(black_save, black_point, sizeof(black_point));\
                                    memcpy(white_save, white_point, sizeof(white_point))

#define BEFORE_DFS(nd) depth++; \
                    int tmp = global_state->row[8];            \
                    lastPosX = nd.x;   \
                     int black_save_minx = black_minx, black_save_miny = black_miny, black_save_maxx = black_maxx, \
                     black_save_maxy = black_maxy, white_save_minx = white_minx, white_save_miny = white_miny,     \
                     white_save_maxx = white_maxx, white_save_maxy = white_maxy; \
                    lastPosY = nd.y;   \
                    PLACE(global_state, nd.x, nd.y, currentColor); \
                    currentColor ^= 1;           \
                    UPDATE_ALL_MIN_MAX(nd)

#define AFTER_DFS(nd)   CLEAR(global_state, nd.x, nd.y);\
                        assert(tmp == global_state->row[8]);                   \
                        depth--; \
                        currentColor ^= 1; \
                        RESTORE_ALL_MIN_MAX

#define BEFORE_RETURN    lastPosX = tmpLastPosX; \
                         lastPosY = tmpLastPosY;  \
                        memcpy(black_point, black_save, sizeof(black_save));\
                        memcpy(white_point, white_save, sizeof(white_save));

#define MAX_ROUND 4// searching six rounds

#define MAX_DEPTH (MAX_ROUND << 1)//if we reach MAX_DEPTH we'll evaluate the state and return

extern void drawBoard(struct State* state);
extern int checkWinner(struct State, int currentColor, int x, int y);
extern struct State* global_state;
extern const int dx[];
extern const int dy[];
extern const int RUSSIAN_ROULLETE;
const int INF = 0x3f3f3f3f;
static int round = 0;
static const time_t return_time_second = 14;
static time_t return_time, timer;
static int depth, currentColor, alphaColor;
static int black_minx = SIZE - 1, black_miny = SIZE - 1, black_maxx = 0, black_maxy = 0, white_minx = SIZE - 1,
            white_miny = SIZE - 1, white_maxx = 0, white_maxy = 0;//to restrict the region for searching
static int black_tot, white_tot;
int white_point[ALIGNMENT][ALIGNMENT], black_point[ALIGNMENT][ALIGNMENT];// store the point for every coord
extern int lastPosX, lastPosY;
static struct BinaryHeap* BH[MAX_DEPTH];
static struct Vec2i best_choice;
void initAI()
{
    srand(time(NULL));
    for(int i = 0; i < MAX_DEPTH; i++)
    {
        BH[i] = (struct BinaryHeap*) malloc(sizeof(struct BinaryHeap));
        memset(BH[i], 0, sizeof(struct BinaryHeap));
    }
    memset(white_point, 0, sizeof(white_point));
    memset(black_point, 0, sizeof(black_point));
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
 *    the status of other empty places! But I assume that simply putting a stone will not make an  which
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
    if(i >= 3 && i <= 12 && j >= 3 && j <= 12)white_point[i][j]++;
    CLEAR_CROSS(cross);
}

void eval_black_form(int i, int j)
{
    if(searchPatternAroundPoint(global_state, i, j, RENJU_LENGTH, BLACK_RENJU_MASK))
        black_point[i][j] = FORM_RENJU;
    else
    {
        struct Cross cross = takeCross(global_state, i, j);
        int live_three_cnt = countBlackLiveThree(cross, i, j);
        int four_cnt = countBlackFour(cross, i, j);
        int two_cnt = countBlackTwo(cross, i, j);
        black_point[i][j] = live_three_cnt * FORM_LIVE_THREE + four_cnt * FORM_FOUR + two_cnt * FORM_TWO;
        if(i >= 3 && i <= 12 && j >= 3 && j <= 12) black_point[i][j]++;
    }
}

void estimate_black_form(int i, int j)
{
    if(black_point[i][j] >= 0)
    {
        struct Cross cross = takeCross(global_state, i, j);
        PLACE_CROSS(cross, BLACK);
        int two_cnt = countBlackTwo(cross, i, j);
        black_point[i][j] += two_cnt * FORM_TWO;
        if(i >= 3 && i <= 12 && j >= 3 && j <= 12)black_point[i][j]++;
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
    for(int i = white_minx; i <= white_maxx; i++)
        for (int j = white_minx; j <= white_maxx; j++)
            if (GET(global_state, i, j) == WHITE)
                white_point[i][j] = eval_white_form(i, j);
    for(int i = black_minx; i <= black_maxx; i++)
        for (int j = black_minx; j <= black_maxx; j++)
            if (GET(global_state, i, j) == BLACK)
                eval_black_form(i, j);
    int max_white_val = -INF, max_black_val = -INF;
    for(int i = white_minx; i <= white_maxx; i++)
        for(int j = white_miny; j <= white_maxy; j++)
            if(GET(global_state, i, j) == WHITE)
                max_white_val = max(max_white_val, white_point[i][j]);
    for(int i = black_minx; i <= black_maxx; i++)
        for(int j = black_miny; j <= black_maxy; j++)
            if(GET(global_state, i, j) == BLACK)
                max_black_val = max(max_black_val, black_point[i][j]);

    if(alphaColor == WHITE) return (max_white_val << 1) - max_black_val;
    else return (max_black_val << 1) - max_white_val;
}

static inline void evaluate_all_moves()
{
    // assume that the last move only affects a few places
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
        for(int i = max(black_minx, 0); i <= min(black_maxx, SIZE - 1); i++)
            for (int j = max(black_minx, 0); j <= min(black_maxx, SIZE - 1); j++)
                if (GET(global_state, i, j) == BLACK)
                    if (destroyBlackLiveThree(i, j)) break;
        for(int i = max(black_minx, 0); i <= min(black_maxx, SIZE - 1); i++)
            for (int j = max(black_minx, 0); j <= min(black_maxx, SIZE - 1); j++)
                if (GET(global_state, i, j) == BLACK)
                    if (destroyBlackFour(i, j)) break;
    }
    else
    {
        for(int i = max(white_minx, 0); i < min(white_maxx, SIZE - 1); i++)
            for (int j = max(white_minx, 0); j < min(white_maxx, SIZE - 1); j++)
                if (GET(global_state, i, j) == WHITE)
                    if (destroyWhiteLiveThree(i, j)) break;
        for(int i = max(white_minx, 0); i < min(white_maxx, SIZE - 1); i++)
            for (int j = max(white_minx, 0); j < min(white_maxx, SIZE - 1); j++)
                if (GET(global_state, i, j) == BLACK)
                    if (destroyWhiteFour(i, j)) break;
    }
}

int dfs(int alpha, int beta)
{
    //printf("%d\n", depth);
    //drawBoard(global_state);
    evaluate_all_moves();
    BEFORE_SEARCH_CHILD;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE ; j++)
        {
            if(GET(global_state, i, j) == BLACK)
            {
                if(searchPatternAroundPoint(global_state, i, j, RENJU_LENGTH, BLACK_RENJU_MASK))
                {
                    BEFORE_RETURN;
                    if(currentColor == alphaColor && currentColor == WHITE) return -INF;
                    if(currentColor == alphaColor && currentColor == BLACK) return INF;
                    if(currentColor != alphaColor && currentColor == BLACK) return INF;
                    if(currentColor != alphaColor && currentColor == WHITE) return -INF;
                }
            }
            if(GET(global_state, i, j) == WHITE)
            {
                if(searchPatternAroundPoint(global_state, i, j, RENJU_LENGTH, WHITE_RENJU_MASK))
                {
                    BEFORE_RETURN;
                    if(currentColor == alphaColor && currentColor == BLACK) return -INF;
                    if(currentColor == alphaColor && currentColor == WHITE) return INF;
                    if(currentColor != alphaColor && currentColor == WHITE) return INF;
                    if(currentColor != alphaColor && currentColor == BLACK) return -INF;
                }
            }
        }
    if(depth == MAX_DEPTH)
    {
        BEFORE_RETURN;
        //drawBoard(global_state);
        int cnt = eval_state();
        return cnt;// 3. evaluate the state and return if reaching MAX_DEPTH
    }
    else// 4. else we search the best moves based on the points of every place
    {
        clear(BH[depth]);
        for(int x = 0; x < SIZE; x++)
        {
            for(int y = 0; y < SIZE; y++)
            {
                if(GET(global_state, x, y) == EMPTY)//first insert the valid place
                {
                    if(currentColor == BLACK)
                    {
                        if(black_point[x][y] >= 0)insert(BH[depth], x, y, black_point[x][y]);
                    }
                    else insert(BH[depth], x, y, white_point[x][y]);
                }
            }
        }
        int best_alpha = -INF;
        for(int i = 0; i < MAX_BRANCHES; i++)
        {
            struct HeapNode nd = top(BH[depth]);
            pop(BH[depth]);
            if(currentColor == alphaColor)
            {
                if(depth == 0)
                {
                    BEFORE_DFS(nd);
                    int new_alpha = dfs(alpha, beta);
                    if(new_alpha > best_alpha || best_choice.x < 0)
                    {
                        best_choice.x = nd.x;
                        best_choice.y = nd.y;
                        best_alpha = new_alpha;
                    }
                    alpha = max(alpha, new_alpha);
                    AFTER_DFS(nd);
                }
                else
                {
                    BEFORE_DFS(nd);
                    alpha = max(alpha, dfs(alpha, beta));
                    AFTER_DFS(nd);
                    if(alpha > beta)//alpha-beta pruning
                    {
                        BEFORE_RETURN;
                        return alpha;
                    }
                }
            }
            else
            {
                BEFORE_DFS(nd);
                beta = min(beta, dfs(alpha, beta));
                AFTER_DFS(nd);
                if(alpha > beta)
                {
                    BEFORE_RETURN;
                    return beta;
                }
            }
        }
        BEFORE_RETURN;
        if(currentColor == alphaColor) return alpha;
        else return beta;
    }
}

void AIPlace(struct State* state, int color)
{
    round++;
    best_choice.x = -1;
    global_state = state;
    alphaColor = currentColor = color;
    for(int i = 0 ; i < SIZE ; i++)
        for(int j = 0; j < SIZE; j++)
            if(GET(global_state, i, j) == BLACK) black_tot++;
            else if(GET(global_state, i, j) == WHITE) white_tot++;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
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
    for(int i = SIZE - 1; i >= 0; i--)
        for(int j = SIZE - 1; j >= 0; j--)
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
    depth = 0;
    dfs(-INF, INF);
    PLACE(state, best_choice.x, best_choice.y, currentColor);
    lastPosX = best_choice.x;
    lastPosY = best_choice.y;
}

#undef isEmpty
#undef PLACEABLE
#undef INSIDE
#undef STEP
#undef MAX_DEPTH
#undef MAX_BRANCHES
#undef MAX_ROUND