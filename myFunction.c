#include "myFunction.h"

//
void getLocation()
{
    struct passwd *pw; // זה מצביע למבנה  שמכילאת פרטי המשתמש במערכת ההפעלה
    pw = getpwuid(getuid());
    if(!pw)
    {
        perror("Error getting user information");
        return;
    }

    char hostname[SIZE_HOSTNAME];
   if (gethostname(hostname, SIZE_HOSTNAME) == -1) {
        perror("Error getting hostname");
        return;
    }

    long size = pathconf(".", _PC_PATH_MAX); // פונקציה שמחזירה את האורך המקסימלי של הנתיב
    if(size == -1)
    {
        perror("Error getting path");
        return;
    }

    if (size > SIZE_BUFF)
        size = SIZE_BUFF;

    char* location = malloc(size + 1); //הוספתי 1 כי צריך להשאיק מקום ל \0
    if(!location)
    {
        perror("Error allocating memory for location");
        return;
    }

    if (getcwd(location, size) == NULL){
        printf("Error getting path\n");
        free(location);
        return;
}
    printf("\033[1;31m");
    printf("%s@%s", pw->pw_name,hostname);
    printf("\033[0m");
    printf(":");
    printf("\033[1;32m");
    printf("%s", location);
    printf("\033[0m");
    printf("$ ");
    free(location);
}
//  יש ליצור פונקציה הקולטת מחרוזת מהמשתמש  באופן דינמי כמו שעשינו בשיעור קודם
char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size);
    }
    *(str + index) = '\0';
    return str;
}


char **splitArgument(char *str)
{

    return NULL;
}