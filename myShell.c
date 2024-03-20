#include "myShell.h"
#include "myFunction.h"
/**
 * This function takes an array of strings and a number representing the size of the array.
 * The function splits the array into sub-arrays, where each sub-array contains strings that are between two NULL values in the original array.
 * The function returns a pointer to an array of sub-arrays.
 */
char ***splitArgumentsArray(char **arguments, int size) {
    // ספירת מספר התת-מערכים
    int numSubArrays = 1;
    for (int i = 0; i < size; i++) {
        if (arguments[i] == NULL) {
            numSubArrays++;
        }
    }

    // הקצאת זיכרון למערך של תת-מערכים
    char ***splitArray = (char ***)malloc((numSubArrays + 1) * sizeof(char **));
    if (splitArray == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // יצירת התת-מערכים
    int subArrayIndex = 0;
    int start = 0;
    int sizeOfSubArray = 0;
    for (int i = 0; i < size; i++, sizeOfSubArray++) {
        if (arguments[i] == NULL || i == size - 1) {
            // הקצאת זיכרון לתת-מערך
            char **subArray = (char **)malloc((sizeOfSubArray + 1) * sizeof(char *));
            // העתקת המחרוזות לתת-מערך
            for(int j = start; j <= i; j++){
                subArray[j - start] = arguments[j];
            }
            // סימון סוף התת-מערך
            subArray[sizeOfSubArray] = NULL;
            // הוספת התת-מערך למערך של תת-מערכים
            splitArray[subArrayIndex++] = subArray;
            // התחלת תת-מערך חדש
            start = i + 1;
            sizeOfSubArray = 0;
        }
    }

    // סימון סוף המערך
    splitArray[subArrayIndex] = NULL;

    return splitArray;
}


/**
 * This function takes an array of strings (arguments) as input.
 * It iterates over the array and checks each string. If the first character of a string is a pipe ('|'), it replaces the string with NULL in the array.
 * The function returns 1 if at least one pipe character was found and replaced with NULL, and 0 otherwise.
 */
int replacePipeWithNull(char **arguments) {
    int res = 0; // Initialize result to 0 (no pipe character found yet)
    int i = 0; // Initialize index to iterate over the array
    while (arguments[i] != NULL) { // Iterate over each string in the array
        if (arguments[i][0] == '|') { // If the first character of the string is a pipe
            arguments[i] = NULL; // Replace the string with NULL in the array
            res = 1; // Set result to 1 (pipe character found and replaced)
        }
        i++; // Move to the next string in the array
    }
    return res; // Return the result
}

int sizeArray(char **arguments) {
    int i = 0;
    while (arguments[i] != NULL) {
        i++;
    }
    return i;
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
        if(strcmp(arguments[0], "exit") == 0){ // if  user enters withspace before exit
            free(arguments);
            logout(input);
}

        int size = sizeArray(arguments);
        int piping = replacePipeWithNull(arguments);
        if (strcmp(input, "echo") == 0){
            if(size ==1) puts("");// if the user enter only echo
            else if(strcmp(arguments[1],">") ==0 || strcmp(arguments[1],">>") ==0) // if the user enter echo > or echo >>
             puts("-bash: syntax error near unexpected token `newline'");
            else if(strcmp(arguments[size-2],">")==0) echorite(arguments);// if the user enter echo > filename
            else if(strcmp(arguments[size-2],">>")==0) echoppend(arguments);// if the user enter echo >> filename
            else// if the user enter echo and some words
            echo(arguments);
        }
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "read") == 0)
            readfile(arguments);
        else if (strcmp(input, "mv") == 0)
            move(arguments);
        else if (strcmp(input, "wc") == 0)
            wordCount(arguments);
        else if (strncmp(input, "cp", 2) == 0){
        puts("cp");
            cp(arguments);
        }
         else if (strcmp(input, "delete") == 0)
            delete(arguments);
        else if (strcmp(input, "dir") == 0)
            get_dir(arguments);
             else if (piping)
        {
char ***splitArray = splitArgumentsArray(arguments,size);// split the array into sub-arrays    
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
    printf(
        "    MMMMMM      OOOOOO     SSSSSS    H      H     EEEEEEE     |  Welcome to Moshe Shell                           \n"      
        "  M m   m  M  O        O  S          H      H     E           |                                                   \n"      
        "  M   m    M  O        O   SSSSS     HHHHHHH      EEEEEEE     |   https://github.com/moshegitlin/myProjekt         \n"      
        "  M        M  O        O        S    H      H     E           |                                                   \n"     
        "  M        M   O      O         S    H      H     E           |   Have Fun!                                       \n"     
        "  M        M    OOOOOO    SSSSSS     H      H     EEEEEEE     |                                                   \n"
    );
}


