#include "myFunction.h"

//
void getLocation()
{
    char location[SIZE_BUFF];
    if (getcwd(location, SIZE_BUFF) == NULL)
        printf("Error\n");
    else
    {
        printf("\033[0;34m");
        puts(location);
        printf("\033[0m");
    }
}
//  יש ליצור פונקציה הקולטת מחרוזת מהמשתמש  באופן דינמי כמו שעשינו בשיעור קודם
char *getInputFromUser()
{

    return NULL;
}

char **splitArgument(char *str)
{

    return NULL;
}