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
    int ch;
    for(int i = 0; i <= n ; i++)
        str[i] = 0;
    for(tot_input = 0; tot_input < n; tot_input++)
    {
        ch = getchar();
        if (ch == '\n' || ch == '\r')return tot_input;
        else str[tot_input] = (char)ch;
    }
    while((ch = getchar()) != '\n' && ch != '\r' && ch != EOF);
    str[n] = 0;
    return tot_input + 1;
}

int fgetLine(FILE* fp, char* str, int n)
{
    int tot_input;
    int ch;
    for(int i = 0; i <= n ; i++)
        str[i] = 0;
    for(tot_input = 0; tot_input < n; tot_input++)
    {
        ch = fgetc(fp);
        if (ch == '\n' || ch == '\r')
        {
            str[tot_input] = 0;
            return tot_input;
        }
        else str[tot_input] = (char)ch;
    }
    while((ch = fgetc(fp)) != '\n' && ch != '\r' && ch != EOF);
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
            switch(opt)
            {
                case 65: return SIGNAL_UP;
                case 66: return SIGNAL_DOWN;
                case 67: return SIGNAL_RIGHT;
                case 68: return SIGNAL_LEFT;
                default: return SIGNAL_UNKNOWN;
            }
        }
    }
    else if(opt == '\r') return SIGNAL_CONFIRM;
    else return SIGNAL_UNKNOWN;
#endif
}

int read_num(FILE *fp)
{
    int x = 0;
    char c = fgetc(fp);
    while(!isdigit(c)) c = fgetc(fp);
    while(isdigit(c))
    {
        x = (x << 3) + (x << 1) + (c ^ 48);
        c = fgetc(fp);
    }
    return x;
}
