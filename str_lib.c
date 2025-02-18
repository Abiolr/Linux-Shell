#include "str_lib.h"

#include <unistd.h>

int my_strlen(const char *str)
{
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int my_streq(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *s1 == *s2; // returns 1 if equal, 0 if not
}

char *my_strncpy(char *dest, const char *source, int n)
{
    int  i;
    for (i = 0; i < n && source[i] != '\0'; i++)
    {
        dest[i] = source[i];
    }
    dest[i] = '\0';
    return dest;
}

void tokenizeString(char *str, char tokens[MAX_ARGS][BUFFER_SIZE], unsigned int *tokenCount)
{
    *tokenCount = 0;
    char *start = str;

    while (*str)
    {
        if (*str == ' ')
        {
            if (start != str)
            {
                my_strncpy(tokens[*tokenCount], start, str - start);
                tokens[*tokenCount][str - start] = '\0';
                (*tokenCount)++;
            }
            start = str + 1;
        }
        str++;
    }

    if (start != str)
    {
        my_strncpy(tokens[*tokenCount], start, str - start);
        tokens[*tokenCount][str - start] = '\0';
        (*tokenCount)++;
    }
}

void print_tokens(char tokens[MAX_ARGS][BUFFER_SIZE], unsigned int *numTokens)
{
    int i;
    for (i = 0; i < *numTokens; i++)
    {
        write(2, tokens[i], my_strlen(tokens[i]));
        write(2, "\n", 1);
    }
}
