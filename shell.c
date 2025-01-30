#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define MAX_TOKENS 256

int main()
{
  char input[BUFFER_SIZE];
  int bytes_read;
  char *tokens[MAX_TOKENS+1];
  int numTokens;
  
  write(2, "$ ", 2);
  bytes_read = read(0, input, BUFFER_SIZE);
  input[bytes_read - 1] = 0;
 
  while (strcmp(input, "exit") > 0)
    {
      input[bytes_read - 1] = '\n';
      write(2, input, bytes_read);
      write(2, "$ ", 2);
      bytes_read = read(0, input, BUFFER_SIZE);
      input[bytes_read - 1] = 0;
    }
  
  return 0;
}
