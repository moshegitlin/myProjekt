#include "myShell.h"
#include "myFunction.h"
char ***splitArgumentsArray(char **arguments, int size) {
    int numSubArrays = 1;
    for (int i = 0; i < size; i++) {
        if (arguments[i] == NULL) {
            numSubArrays++;
        }
    }

    char ***splitArray = (char ***)malloc((numSubArrays + 1) * sizeof(char **));
    if (splitArray == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int subArrayIndex = 0;
    int start = 0;
    int sizeOfSubArray = 0;

    for (int i = 0; i < size; i++, sizeOfSubArray++) {
        if (arguments[i] == NULL || i == size - 1) {
            char **subArray = (char **)malloc((sizeOfSubArray + 1) * sizeof(char *));
            for(int j = start; j <= i; j++){
                subArray[j - start] = arguments[j];
            }
            subArray[sizeOfSubArray] = NULL; // סימון סוף התת-מערך
            splitArray[subArrayIndex++] = subArray;
            start = i + 1;
            sizeOfSubArray = 0;
        }
    }

    splitArray[subArrayIndex] = NULL; // סימון סוף המערך

    return splitArray;
}


int replacePipeWithNull(char **arguments) {
    int res = 0;
    int i = 0;
    while (arguments[i] != NULL) {
        if (arguments[i][0] == '|') {
            arguments[i] = NULL;
            res = 1;
        }
        i++;
    }
    if(res==1){
        return i;
    }
    return res;
}

int main()
{

    welcome();
    while (1)
    {
        
        getLocation();
        char *input = getInputFromUser();

       if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
            logout(input);


        char **arguments = splitArgument(input);
        // int i=0;
        // while (arguments[i] != NULL)
        // {
        //     printf("arguments[%s],", arguments[i]);
        //     i++;
        // }
        // puts("");
        if(strcmp(arguments[0], "exit") == 0){
            free(arguments);
            logout(input);
}

        int piping = replacePipeWithNull(arguments);
        if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "mv") == 0)
            move(arguments);
        else if (strncmp(input, "cp", 2) == 0){
        puts("cp");
            cp(arguments);
        }
         else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (strcmp(input, "dir") == 0)
            get_dir(arguments);
             else if (piping)
        {
char ***splitArray = splitArgumentsArray(arguments, piping);
          mypipe(splitArray[0], splitArray[1]);
          wait(NULL);

    for (int i = 0; splitArray[i] != NULL; i++) {
        // שחרור הזיכרון שהוקצה לכל תת-מערך
        free(splitArray[i]);
    }
// שחרור הזיכרון שהוקצה למערך הראשי
    free(splitArray);
        }
        else
        {
            systemCall(arguments);
            wait(NULL);
        }
        free(arguments);
        free(input);
        
    }
    return 0;
}

void welcome()
{
    puts("Welcome to my Shell");
}