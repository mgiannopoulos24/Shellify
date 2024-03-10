#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    const char *command;
    void (*function)();
} internalCommand;

void exitShell();
void changeDirectory(const char **tokens);
/* void printWorkingDirectory(); */
/* void echo(); */
/* void sourceFile(); */
/* void killProcess(); */
/* void showManual(); */

void changeDirectory(const char **tokens) {
}

void exitShell() {
    exit(EXIT_SUCCESS);
}

bool externalCommand(const char *fname) {
    const char *directory = "/usr/bin/";
    char filepath[1033];

    sprintf(filepath, "%s/%s", directory, fname);

    if (access(filepath, F_OK) == 0) {
        return true;
    }
    return false;
}

void commandHandler(const char *str) {
    internalCommand builtIn[] = {
        {"cd", changeDirectory},
        /* {"pwd", printWorkingDirectory}, */
        /* {"echo", echo}, */
        /* {"exit", exitShell}, */
        /* {"source", sourceFile}, */
        /* {"kill", killProcess}, */
        /* {"man", showManual} */
    };
   
    int commandsCount = sizeof(builtIn) / sizeof(builtIn[0]);

    for(int i = 0; i < commandsCount; i++) {
        if(strcmp(str, builtIn[i].command) == 0) {
            builtIn[i].function();
            return;
        }
    }
    
    if(externalCommand(str)) {
        printf("Command Exists\n");
    }
    else {
        printf("Command not found: %s\n", str);
    }
}

int main(int argc, char *argv[]) {    
    char *str = NULL, *token = NULL, c = '\0';
    char **tokens = NULL;

    int i = 0;
    int tokenCount = 0;

    str = (char*)malloc(sizeof(char));

    printf("sheLLiFY >>> ");

    while ((c = getchar()) != '\n') {
        str = realloc(str, (i + 1) * sizeof(char));
        str[i++] = c;
    }
    
    tokens = (char**)(malloc(sizeof(char*)));
    token = strtok(str, " ");

    while(token != NULL) {
        tokens = realloc(tokens, (tokenCount + 1) * sizeof(char*));
        tokens[tokenCount++] = strndup(token, strlen(token));
        token = strtok(NULL, " ");
    }
    free(str);

    commandHandler(tokens[0]);
    free(tokens);
}
