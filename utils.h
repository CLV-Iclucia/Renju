#ifndef UTILS_H
#define UTILS_H

#define is_lowercase(ch) ((ch) <= 'z' && (ch) >= 'a')
#define is_capital(ch) ((ch) <= 'Z' && (ch) >= 'A')
#define is_alpha(ch) (is_lowercase(ch) || is_capital(ch))
#define is_digit(ch) ((ch) <= '9' && (ch) >= '0')

int stringstream(const char*, int*);
int getLine(char*, int);
#endif