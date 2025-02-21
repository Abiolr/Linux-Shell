#include "constants.h"
#include "mylib.h"

#include <unistd.h>

#ifndef STRLIB_H
#define STRLIB_H

int my_strlen(const char *str);
char *my_strncpy(char *dest, const char *source, int n);
int my_streq(const char *s1, const char *s2);
void tokenize_string(char *str, char * tokens[MAX_ARGS + 1], unsigned int *token_count, char delimiter);
void print_tokens(char * tokens[MAX_ARGS + 1], unsigned int *numTokens);

#endif
