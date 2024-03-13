#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PATH_MAX 4096

typedef struct {
    const char *command;
    void (*function)(char *[]);
} internalCommand;

void exitShell();
void changeDirectory(char *tokens[]);
void printWorkingDirectory();
/* void echo(); */
/* void sourceFile(); */
/* void killProcess(); */
/* void showManual(); */

void printWorkingDirectory() {
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return;
    }
    printf("Could not print cwd\n");
}

void changeDirectory(char *tokens[]) {
    printf("%s\n", tokens[1]);
    if(chdir(tokens[1]) != 0) {
        printf("Could not change directory to %s\n", tokens[1]);
    }
    printWorkingDirectory();
}

void exitShell() {
    exit(EXIT_SUCCESS);
}

bool externalCommand(char *args[]) {
    const char *directory = "/usr/bin/";
    unsigned int pathSize = strlen(directory) + strlen(args[0]) + 1; 
    char filepath[pathSize];

    sprintf(filepath, "%s/%s", directory, args[0]);

    if (access(filepath, F_OK) == 0) {
        return true;
    }
    return false;
}

void commandHandler(char *tokens[]) {
    internalCommand builtIn[] = {
        {"cd", changeDirectory},
        {"pwd", printWorkingDirectory},
        /* {"echo", echo}, */
        {"exit", exitShell},
        /* {"source", sourceFile}, */
        /* {"kill", killProcess}, */
        /* {"man", showManual} */
    };
   
    int commandsCount = sizeof(builtIn) / sizeof(builtIn[0]);

    for(int i = 0; i < commandsCount; i++) {
        if(strcmp(tokens[0], builtIn[i].command) == 0) {
            builtIn[i].function(tokens);
            return;
        }
    }
    
    if(!externalCommand(tokens)) {
        printf("Command not found: %s\n", tokens[0]);
        return;
    }
    execvp(tokens[0], tokens);
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
    tokens = realloc(tokens, (tokenCount + 1) * sizeof(char*));
    tokens[tokenCount] = NULL;

    commandHandler(tokens);
    free(tokens);
}
