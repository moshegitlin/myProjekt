#include "myFunction.h"
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


char *my_strtok(char *str, const char *delim) {
    static char *next_token = NULL;
    static size_t original_length = 0;  
    char *token;

    if (str != NULL) { // אם זה הפעם הראשונה שאני מקבל את המחרוזת
        token = str; // אני מצביע על המחרוזת
        original_length = strlen(str); // אני שומר את אורך המחרוזת
        next_token= NULL; // אני מאפס את המצביע של המחרוזת הבאה
    } 
    else {// אם זה לא הפעם הראשונה שאני מקבל את המחרוזת
    
        if (next_token == NULL) { // אני בודק שלא הגעתי לסוף המחרוזת
            return NULL;
        }
        token = next_token;// אני מצביע על המחרוזת הבאה כדי שאני יוכל להכניס על neext_token את האות הסיומת
    }

    for(int i=0;i<original_length;i++){ //אני מחפש את המקום הראשון שבו יש רווח
        if(*(token+i)==' '){
            next_token=token+i;
            break;
        }
        if(i==original_length-1){// אם הגעתי לסוף המחרוזת ואין רווח אז סימן שאני בסוף המחרוזת 
            next_token=NULL;
        }
    }
    if (next_token != NULL) {
        *next_token = '\0';
        next_token += 1;
    }

    return token;
}

char **splitArgument(char *str)
{
    char *subStr;
    subStr = my_strtok(str, " ");
    int size = 2;
    int index = 0;
    char **argumnts = (char **)malloc(size * sizeof(char *));
    // argumnts[index] = subStr;
    *(argumnts + index) = subStr; // [subStr,subStr,subStr,subStr,NULL]
    while ((subStr = my_strtok(NULL, " ")) != NULL)
    {
        index++;
        size++;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
        *(argumnts + index) = subStr;
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}

