#include <stdio.h>
#include <math.h>
#define EMPTY 0
#define BLACK 2
#define WHITE 3
#define PLACE(i, j, COLOR) b[(i)]|=(COLOR<<((j)<<1)) //place a chess of color COLOR at (i, j), make sure that (i, j) is valid
#define CHECK(i, j) ((b[(i)]>>((j)<<1))&3) //check the state of (i, j)
#define SIZE 15
int b[SIZE];//00 stands for no chess, 10 stands for black, 11 stands for white.So I can use "COLOR^=1" to change color.
int main()
{
    int color = BLACK;
    while(1)
    {
        for (int i = 0; i < SIZE; ++i, putchar('\n'))
            for (int j = 0; j < SIZE; ++j) {
                int col = CHECK(i, j);
                if (col == EMPTY)putchar('.');
                else if (col == BLACK)putchar('B');
                else putchar('W');
            }
        int i, j;
        scanf("%d %d", &i, &j);
        PLACE(i, j, color);
        color ^= 1;
    }
    return 0;
}
