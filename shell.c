#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define MAX_TOKENS 256

int main()
{
  char input[BUFFER_SIZE];
  int bytes_read;
  int bytes_written;

  write(2, "$ ", 2);
  bytes_read = read(0, input, BUFFER_SIZE);
  input[bytes_read - 1] = 0;

  while (strcmp(input, "exit") != 0)
  {
    char *tokens[MAX_TOKENS + 1];
    int numTokens = 0;
    int i;

    tokens[numTokens] = strtok(input, " ");
    while (tokens[numTokens])
    {
      numTokens++;
      tokens[numTokens] = strtok(NULL, " ");
    }

    for (i = 0; i < numTokens; i++)
    {
      write(2, tokens[i], strlen(tokens[i]));
      write(2, "\n", 1);
    }

    write(2, "$ ", 2);
    bytes_read = read(0, input, BUFFER_SIZE);
    input[bytes_read - 1] = 0;
  }

  return 0;
}