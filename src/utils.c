#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// start from index p and read a non-negative integer from str
int stringstream(const char* str, int* p)
{
    int ret = 0;
    unsigned long n = strlen(str);
    while(!isdigit(str[*p]))(*p)++;
    while(isdigit(str[*p]) && (*p) < n)
    {
        ret = (ret << 3) + (ret << 1) + ((str[*p]) ^ 48);
        (*p)++;
    }
    return ret;
}

//a safer input function, because not all compilers support scanf_s so DIY is better.
int getLine(char* str, int n)
{
    int tot_input;
    char ch;
    for(int i = 0; i <= n ; i++)
        str[i] = 0;
    for(tot_input = 0; tot_input < n; tot_input++)
    {
        ch = getchar();
        if (ch == '\n' || ch == '\r')return tot_input;
        else str[tot_input] = ch;
    }
    while((ch = getchar()) != '\n' && ch != '\r' && ch != EOF);
    str[n] = 0;
    return tot_input + 1;
}

void clear_output()
{
#ifdef IS_LINUX
    system("clear");
#elif IS_DARWIN
    system("clear");
#elif IS_WINDOWS
    system("cls");
#endif
}