#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#ifdef IS_WINDOWS
#include <conio.h>
#else
#include <termio.h>
#endif
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

char getchar_crossplatform()
{
    char c = getchar();
#ifdef IS_WINDOWS
    if(c == '\r')
    {
        c = getchar();
        return c;
    }
    else return c;
#else
    return c == '\n';
#endif
}

char getchar_no_buf()
{
#ifdef IS_WINDOWS
    return getch();
#else
    struct termios tm, tm_orig;
    int fd = 0;
    if(tcgetattr(fd, &tm) < 0)
        return -1;
    tm_orig = tm;
    cfmakeraw(&tm);
    if(tcsetattr(fd, TCSANOW, &tm) < 0)
        return -1;
    char c = getchar();
    if(tcsetattr(fd, TCSANOW, &tm_orig) < 0)
        return -1;
    return c;
#endif
}

int getControlInput()
{
    char opt = getchar_no_buf();
#ifdef IS_WINDOWS

#else
    if(opt == 27)
    {
        opt = getchar_no_buf();
        if(opt == 91)
        {
            opt = getchar_no_buf();
            if(opt == 65 || opt == 68) return UP;
            else if(opt == 66 || opt == 67) return DOWN;
            else return UNKNOWN;
        }
    }
#endif
}