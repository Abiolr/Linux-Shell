#include "constants.h"
#ifndef STRLIB_H
#define STRLIB_H

int my_strlen(const char *str);
void tokenizeString(char *str, char tokens[MAX_TOKENS][BUFFER_SIZE], int *tokenCount);
int my_streq(const char *s1, const char *s2);

#endif