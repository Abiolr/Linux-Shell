#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "str_lib.h"

int main() {
    char buffer[BUFFER_SIZE];
    int bytes_read;
    write(2, "mysh$ ", 6);
    bytes_read = read(0, buffer, BUFFER_SIZE);
    buffer[bytes_read - 1] = '\0';

    while (my_streq(buffer, "exit") == 0) {
        char tokens[MAX_TOKENS][BUFFER_SIZE];
        int numTokens;
        int i;

        tokenizeString(buffer, tokens, &numTokens);

        for (i = 0; i < numTokens; i++) {
            if (strcmp(tokens[i], "exit") == 0) {
                exit(2);
            } else {
                write(2, tokens[i], my_strlen(tokens[i]));
                write(2, "\n", 1);
            }
        }

        write(2, "mysh$ ", 6);
        bytes_read = read(0, buffer, BUFFER_SIZE);
        buffer[bytes_read - 1] = '\0';
    }

    return 0;
}