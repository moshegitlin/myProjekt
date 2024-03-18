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
    if(check_quotes_saved) {
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
    int size = strlen(token);
     if(*(token)==delim){
    for(int i=0; i<size;i++){
       if(*(token+i)!= delim){
              token=token+i;
              break;
       }
    }
     }
    
    for(int i=0;i<strlen(token);i++){ //אני מחפש את המקום הראשון שבו יש רווח

        if(*(token+i)== delim && *(token+i+1)!= delim){
            next_token=token+i;
            break;
        }
        if(i==strlen(token)-1){// אם הגעתי לסוף המחרוזת ואין רווח אז סימן שאני בסוף המחרוזת 
            next_token=NULL;
        }
    }
    if (next_token != NULL) {
        *next_token = '\0';
        next_token ++;
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
    if(arguments[1]==NULL || arguments[2]==NULL){
        puts("error");
        return;
    }
    if(arguments[3]!=NULL){
        puts("error");
        return;
    }
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
void delete(char **path)
{
    if (path[2] != NULL)
    {
        printf("-myShell: delete: too many arguments\n");
        return;
    }
    if (unlink(path[1]) != 0)
        printf("-myShell: delete: %s: No such file or directory\n", path[1]);
}
void get_dir()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./")) == NULL)
    {
        perror("");
        return;
    }
    while ((ent = readdir(dir)) != NULL)
        printf("%s ", ent->d_name);
    puts("");
}
void systemCall(char **arguments)
{
    pid_t pid = fork();
    if (pid == -1)
    {
       
        return;
    }
    if (pid == 0)
    {
        if (execvp(arguments[0], arguments) == -1)
         printf("%s: command not found\n", arguments[0]);
            exit(EXIT_FAILURE);
    }
}
void mypipe(char **argv1,char ** argv2){

    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            /* first component of command line */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
            execvp(argv1[0], argv1);
        }
        /* 2nd command component of command line */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* standard input now comes from pipe */
        execvp(argv2[0], argv2);
    }
}
void move(char **arguments)
{
    if(arguments[1]==NULL){
        puts("mv: missing file operand");
        return;
    }
    if(arguments[2]==NULL){
        printf("mv: missing destination file operand after '%s'", arguments[1]);
        return;
    }
    if(arguments[3]!=NULL){
        puts("too many arguments");
        return;
    }
    cp(arguments);
    if(errno==2){
        printf("mv: cannot stat '%s': No such file or directory\n", arguments[1]);
    }
    else if(errno==13){
        printf("mv: cannot create regular file '%s': Permission denied\n", arguments[2]);
    }
    else if(errno==21){
        printf("mv: cannot create regular file '%s': Is a directory\n", arguments[2]);
    }
    else if(errno==17){
        printf("mv: cannot create regular file '%s': File exists\n", arguments[2]);
    }
    else if(errno==0){
       if (unlink(arguments[1]) != 0)
        printf("-myShell: delete: %s: No such file or directory\n", arguments[1]);
    }
    else {
    printf("An error occurred: %s\n", strerror(errno));
}
}
void wordCount(char **arguments)
{
    if(arguments[1]==NULL||arguments[2]==NULL){
        puts("error");
        return;
    }
    if(arguments[3]!=NULL){
        puts("error");
        return;
    }
    FILE *file;
    char ch;
    int wordCount = 0, lineCount = 0;
    char prevChar = '\0';
    int inWord = 0;
    if ((file = fopen(arguments[2], "r")) == NULL)
    {
        puts("error");
        return;
    }
    while ((ch = fgetc(file)) != EOF)
    {
        if ((ch == ' ' || ch == '\n' || ch == '\t') && !inWord)
        {
            inWord = 1;
            wordCount++;
          if (ch == '\n') {
            lineCount++;
        }
        } else if (ch != ' ' && ch != '\n' && ch != '\t') {
            inWord = 0;
        }
    }
     prevChar = ch;

    // Check if the last word was not followed by a space, newline or tab
    if (prevChar != ' ' && prevChar != '\n' && prevChar != '\t') {
        wordCount++;
    }
    

    fclose(file);
    if (strcmp(arguments[1], "-l") == 0){
        printf("%d %s\n", lineCount, arguments[2]);
    }
     else if (strcmp(arguments[1], "-w") == 0){
        printf("%d %s\n", wordCount, arguments[2]);
     }
     else printf("wc: invalid option -- '%c'\n", arguments[1][1]);
}
void echoppend(char **arguments)
{
    int size=0;
     while (arguments[size] != NULL) {
        size++;
    }
    FILE *file;
    
    if ((file = fopen(arguments[size-1], "a")) == NULL)
    {
        puts("error");
        return;
    }
   for (int i = 1; i < size-2; i++){
    fprintf(file, "%s ", arguments[i]);
   }
    fprintf(file, "%s ","\n");
    fclose(file);

}
void echorite(char **arguments)
{
    int size=0;
     while (arguments[size] != NULL) {
        size++;
    }
    FILE *file;
    
    if ((file = fopen(arguments[size-1], "w")) == NULL)
    {
        puts("error");
        return;
    }
   for (int i = 1; i < size-2; i++){
    fprintf(file, "%s ", arguments[i]);
   }
    fprintf(file, "%s ","\n");
    fclose(file);
}
void readfile(char **arguments)
{
    if(arguments[1]==NULL){
        puts("error");
        return;
    }
    if(arguments[2]!=NULL){
        puts("error");
        return;
    }
    FILE *file;
    char ch;
    if ((file = fopen(arguments[1], "r")) == NULL)
    {
        puts("error");
        return;
    }
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
    fclose(file);
}