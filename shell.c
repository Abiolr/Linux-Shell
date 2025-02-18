#include "constants.h"
#include "str_lib.h"
#include "command.h"

#include <unistd.h>
#include <stdlib.h>

int main()
{
    struct Command command;
    
    get_command(&command);

    while (my_streq(command.argv[0], "exit") == 0)
    {
        run_command(&command);
        get_command(&command);
    }

    return 0;
}