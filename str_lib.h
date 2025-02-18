#include "constants.h"

#ifndef STRLIB_H
#define STRLIB_H

int my_strlen(const char *str);
char *my_strncpy(char *dest, const char *source, int n);
int my_streq(const char *s1, const char *s2);
void tokenizeString(char *str, char tokens[MAX_ARGS][BUFFER_SIZE], unsigned int *tokenCount);
void print_tokens(char tokens[MAX_ARGS][BUFFER_SIZE], unsigned int  *numTokens);

#endif