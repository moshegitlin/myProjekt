#include "myShell.h"
#include "myFunction.h"
int main()
{
    welcome();
    while (1)
    {
        getLocation();
        char* input = getInputFromUser();
    }
    return 0;
}

void welcome()
{
    puts("Welcome to my Shell");
}