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


void logout(char *input)
{
    free(input);
    puts("logout");
    exit(EXIT_SUCCESS); // EXIT_SUCCESS = 0
}

char *my_strtok(char *str, const char delim,int check_quotes) {
    static char *next_token = NULL; 
    char *token;
    static int check_quotes_saved;

    if (str != NULL) { // אם זה הפעם הראשונה שאני מקבל את המחרוזת
        token = str; // אני מצביע על המחרוזת
        next_token= NULL; // אני מאפס את המצביע של המחרוזת הבאה
        check_quotes_saved = check_quotes;
    } 
    else {// אם זה לא הפעם הראשונה שאני מקבל את המחרוזת
    
        if (next_token == NULL) { // אני בודק שלא הגעתי לסוף המחרוזת
            return NULL;
        }
        token = next_token;// אני מצביע על המחרוזת הבאה כדי שאני יוכל להכניס על neext_token את האות הסיומת
    }
    if(check_quotes_saved==1) {
        if(*token == '"') {
            token++;
            char *end = strchr(token, '"');
            if(end != NULL) {
                next_token = end;
                *next_token = '\0';
                next_token++;
            if(*next_token=='\0'){// אם הגעתי לסוף המחרוזת ואין רווח אז סימן שאני בסוף המחרוזת 
            next_token=NULL;
            }
            else{
            next_token++;
            }
            return token;
            }
            token--;
        }
    }
    for(int i=0;i<strlen(token);i++){ //אני מחפש את המקום הראשון שבו יש רווח
        if(*(token+i)== delim){
            next_token=token+i;
            break;
        }
        if(i==strlen(token)-1){// אם הגעתי לסוף המחרוזת ואין רווח אז סימן שאני בסוף המחרוזת 
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
    subStr = my_strtok(str, ' ',1);
    int size = 2;
    int index = 0;
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr;
    while ((subStr = my_strtok(NULL, ' ',1)) != NULL)
    {
        index++;
        size++;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
        *(argumnts + index) = subStr;
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}
void echo(char **arguments)
{
    while (*(++arguments))
        printf("%s ", *arguments);

    puts("");
}
char* concatenateStrings(char** arguments,int size){
    char *temp = (char *)malloc((strlen(arguments[1]) + 1) * sizeof(char)); // הקצאת זכרון למחרוזת שאמורה להחזיק את הנתיב
        strcpy(temp, arguments[1]); // העתקת המחרוזת למשתנה הזמני
        memmove(temp,temp+1,strlen(temp));// מחיקת הגרש
        strcat(temp, " "); // הוספת רווח בסוף המחרוזת
        for(int j=2;j<size;j++){
            temp = (char *)realloc(temp,((strlen(arguments[j])+2) * sizeof(char))); //הוספת מקום למחרוזת הבאה
            if(j == size-1){ // אם זה הארגומנט האחרון
                strcat(temp, arguments[j]); // הוספת הארגומנט למחרוזת
                 temp[strlen(temp)-1] = '\0'; // מחיקת הגרש
            }
            else{
               strcat(temp, arguments[j]); // הוספת הארגומנט למחרוזת
                 strcat(temp, " "); // הוספת רווח בסוף המחרוזת
            }  
        }
    return temp;
}
void cd(char **path) {
    if (path[2] != NULL) {
        printf("-myShell: cd: too many arguments\n");
        return;
    } // בדיקה האם יש יותר מדי ארגומנטים
     if (chdir(path[1]) != 0)  
        printf("-myShell: cd: %s: No such file or directory\n", path[1]);
}
void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("error");
        return;
    }

    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("error");
        fclose(src);
        return;
    }
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, des);

    fclose(src);
    fclose(des);
}


// input = cd "OneDrive - Ariel University"\0
// after split
// input = cd\0"OneDrive\0-\0Ariel\0University"\0
//[cd\0, "OneDrive\0,  -\0,  Ariel\0,  University"\0,  NULL]

// input = cd\0"OneDrive - Ariel University"\0
//[cd\0, "OneDrive ,  - ,  Ariel ,  University"\0,  NULL]
