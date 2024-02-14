#include "myShell.h"
#include "myFunction.h"
int main()
{
    welcome();
    while (1)
    {
        getLocation();
        char* input = getInputFromUser();
         puts(input);
        char **arguments = splitArgument(input);
        for (int i = 0; *(arguments + i) != NULL; i++)
        {
            puts(*(arguments+i));
            // puts(arguments[i]);
        }

        free(arguments);
        free(input);

        break;
    }
    return 0;
}

void welcome()
{
    puts("Welcome to my Shell");
}