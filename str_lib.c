/*
 * str_lib.c
 *
 * This file provides utility functions for string manipulation, including:
 * - Calculating the length of a string.
 * - Comparing two strings for equality.
 * - Tokenizing a string into an array of tokens based on spaces.
 * - Printing an array of tokens to standard error.
 *
 * These functions are designed to support the shell program by handling string-related
 * operations efficiently and safely.
 */

#include "str_lib.h"
#include <unistd.h>

/*
 * my_strlen
 *
 * Purpose:
 *   Calculates the length of a null-terminated string.
 *
 * Input:
 *   str - The input string whose length is to be calculated.
 *
 * Output:
 *   int - The length of the string (number of characters, excluding the null terminator).
 *
 * Assumptions:
 *   - The input string is null-terminated.
 */
int my_strlen(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

/*
 * my_streq
 *
 * Purpose:
 *   Compares two strings for equality.
 *
 * Inputs:
 *   s1 - The first string to compare.
 *   s2 - The second string to compare.
 *
 * Output:
 *   int - Returns 1 if the strings are equal, 0 if they are not.
 *
 * Assumptions:
 *   - Both input strings are null-terminated.
 */
int my_streq(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *s1 == *s2; // returns 1 if equal, 0 if not
}

/*
 * my_strncpy
 *
 * Purpose:
 *   Copies up to `n` characters from the source string to the destination string.
 *   If the source string is shorter than `n` characters, the remaining space in the destination
 *   is filled with the null terminator.
 *
 * Inputs:
 *   dest - The destination buffer where the copied string will be stored.
 *   source - The source string to be copied.
 *   n - The maximum number of characters to copy, including the null terminator.
 *
 * Output:
 *   char* - Returns the destination buffer (`dest`) after copying.
 * 
 * Assumptions:
 *   - The destination buffer (`dest`) has enough space to hold at least `n` characters.
 *   - The source string (`source`) is null-terminated.
 *
 * Limitations:
 *   - If `n` is greater than the length of the source string, the destination buffer will be
 *     null-terminated at the position corresponding to the end of the source string.
 *   - If `n` is less than or equal to the length of the source string, the destination buffer
 *     will not be null-terminated beyond the `n` characters copied.
 */
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

/*
 * tokenizeString
 *
 * Purpose:
 *   Splits a string into tokens based on a specified delimiter and stores them in a 2D array.
 *
 * Inputs:
 *   str - The input string to be tokenized.
 *   tokens - A 2D array where the resulting tokens will be stored.
 *   tokenCount - A pointer to an unsigned integer where the number of tokens will be stored.
 *   delimiter - The character used to split the string into tokens.
 *
 * Output:
 *   void - The tokens are stored in the `tokens` array, and the number of tokens is stored in `tokenCount`.
 *
 * Assumptions:
 *   - The input string is null-terminated.
 *   - The `tokens` array has enough space to store all tokens (MAX_ARGS x BUFFER_SIZE).
 *   - The `tokenCount` pointer is valid and points to a writable memory location.
 *
 * Limitations:
 *   - Only the specified delimiter is used; other whitespace characters are not handled.
 *   - The function exits with an error if the number of tokens exceeds MAX_ARGS.
 */
void tokenizeString(char *str, char * tokens[MAX_ARGS + 1], unsigned int *tokenCount, char delimiter)
{
    *tokenCount = 0;
    char *start = str;

    while (*str)
    {
        if (*str == delimiter)
        {
            if (start != str)
            {
                if (*tokenCount >= MAX_ARGS)
                {
                    write(2, "error: argument count exceeded\n", 31);
                    _exit(1);
                }

                int length = str - start;
                tokens[*tokenCount] = (char *)my_alloc(length + 1);
                if (tokens[*tokenCount] == NULL)
                {
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
        if (*tokenCount >= MAX_ARGS)
        {
            write(2, "error: argument count exceeded\n", 31);
            _exit(1);
        }

        int length = str - start;
        tokens[*tokenCount] = (char *)my_alloc(length + 1);
        if (tokens[*tokenCount] == NULL)
        {
            return;
        }
        my_strncpy(tokens[*tokenCount], start, length);
        tokens[*tokenCount][length] = '\0';
        (*tokenCount)++;
    }

    tokens[*tokenCount] = NULL;
}

/*
 * print_tokens
 *
 * Purpose:
 *   Prints an array of tokens to standard error, with each token on a new line.
 *
 * Input:
 *   tokens - A 2D array containing the tokens to be printed.
 *   numTokens - A pointer to an unsigned integer indicating the number of tokens in the array.
 *
 * Output:
 *   void - The tokens are printed to standard error.
 *
 * Assumptions:
 *   - The `tokens` array contains valid null-terminated strings.
 *   - The `numTokens` pointer is valid and points to a readable memory location.
 */
void print_tokens(char * tokens[MAX_ARGS + 1], unsigned int *numTokens)
{
    int i;
    for (i = 0; i < *numTokens; i++)
    {
        write(2, tokens[i], my_strlen(tokens[i]));
        write(2, "\n", 1);
    }
}