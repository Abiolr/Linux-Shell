#include <string.h>
#include "str_lib.h"

int my_strlen(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int my_streq(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 == *s2; // returns 1 if equal, 0 if not
}

void tokenizeString(char *str, char tokens[MAX_TOKENS][BUFFER_SIZE], int *tokenCount) {
    *tokenCount = 0;
    char *start = str;

    while (*str) {
        if (*str == ' ') {
            if (start != str) {
                strncpy(tokens[*tokenCount], start, str - start);
                tokens[*tokenCount][str - start] = '\0';
                (*tokenCount)++;
            }
            start = str + 1;
        }
        str++;
    }

    if (start != str) {
        strncpy(tokens[*tokenCount], start, str - start);
        tokens[*tokenCount][str - start] = '\0';
        (*tokenCount)++;
    }
}