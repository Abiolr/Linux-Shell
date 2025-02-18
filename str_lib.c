#include "str_lib.h"

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

void tokenizeString(char *str, char * tokens[MAX_ARGS + 1], unsigned int *tokenCount)
{
    *tokenCount = 0;
    char *start = str;

    while (*str)
    {
        if (*str == ' ')
        {
            if (start != str)
            {
                int length = str - start;
                tokens[*tokenCount] = (char *)my_alloc(length + 1);  // Use my_alloc for memory allocation
                if (tokens[*tokenCount] == NULL) {
                    // Handle memory allocation failure
                    return;
                }
                my_strncpy(tokens[*tokenCount], start, length);
                tokens[*tokenCount][length] = '\0';
                (*tokenCount)++;
            }
            start = str + 1;
        }
        str++;
    }

    if (start != str)
    {
        int length = str - start;
        tokens[*tokenCount] = (char *)my_alloc(length + 1);  // Use my_alloc for memory allocation
        if (tokens[*tokenCount] == NULL) {
            // Handle memory allocation failure
            return;
        }
        my_strncpy(tokens[*tokenCount], start, length);
        tokens[*tokenCount][length] = '\0';
        (*tokenCount)++;
    }

    tokens[*tokenCount] = NULL;  // Null-terminate the argv array
}

void print_tokens(char * tokens[MAX_ARGS + 1], unsigned int *numTokens)
{
    int i;
    for (i = 0; i < *numTokens; i++)
    {
        write(2, tokens[i], my_strlen(tokens[i]));
        write(2, "\n", 1);
    }
}
