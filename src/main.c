#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#define PATH_MAX 4096

typedef struct {
    const char *command;
    void (*function)(char *[]);
} internalCommand;

void exitShell();
void changeDirectory(char *tokens[]);
void printWorkingDirectory();
void echoInput(char *tokens[]);

void echoInput(char *tokens[]) {
    for (int i = 1; tokens[i] != NULL; i++) {
        printf("%s ", tokens[i]);
    }
    printf("\n");
}

void printWorkingDirectory() {
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return;
    }
    printf("Could not print cwd\n");
}

void changeDirectory(char *tokens[]) {
    if(chdir(tokens[1]) != 0) {
        printf("Could not change directory to %s\n", tokens[1]);
    }
}

void exitShell() {
    exit(EXIT_SUCCESS);
}

char *getUserName() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if(pw == NULL) {
        perror("getpwuid");
        return NULL;
    }
    return pw->pw_name;
}

bool externalCommand(char *args[]) {
    const char *userName = getUserName();
    char filepath[PATH_MAX];

    snprintf(filepath, sizeof(filepath), "/home/%s/.local/bin/%s", userName, args[0]);

    if (access(filepath, F_OK) == 0) {
        return true;
    }

    snprintf(filepath, sizeof(filepath), "/usr/bin/%s", args[0]);

    if (access(filepath, F_OK) == 0) {
        return true;
    }
    return false;
}

void commandHandler(char *tokens[]) {
    internalCommand builtIn[] = {
        {"cd", changeDirectory},
        {"pwd", printWorkingDirectory},
        {"echo", echoInput},
        {"exit", exitShell},
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

    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("Fork");
            break;
        case 0:
            execvp(tokens[0], tokens);
            perror("execvp");
            exit(EXIT_FAILURE);
        default:
            wait(NULL);
            break;
    }
}

void tokenizeInput(char *input, char **tokens[], int *tokenCount) {
    char *token = strtok(input, " ");

    while(token != NULL) {
        (*tokenCount)++;
        *tokens = realloc(*tokens, (*tokenCount) * sizeof(char *));
        (*tokens)[(*tokenCount) - 1] = strndup(token, strlen(token));
        token = strtok(NULL, " ");
    }
    *tokens = realloc(*tokens, ((*tokenCount) + 1) * sizeof(char *));
    (*tokens)[(*tokenCount)] = NULL;
}

char *readInput() {
    char *str = NULL, c; 
    int size = 0;

    str = (char*)malloc(sizeof(char));

    while ((c = getchar()) != '\n') {
        str = realloc(str, (size + 1) * sizeof(char));
        str[size++] = c;
    }
    str = realloc(str, (size + 1) * sizeof(char));
    str[size] = '\0';
    return str;
}

int main(int argc, char *argv[]) {    
    while(1) {
        printf("sheLLiFY >>> ");
        char *str = readInput(), **tokens = NULL;
        int tokenCount = 0;

        while(strcmp(str, "") == 0) {
            free(str);
            printf("sheLLiFY >>> ");
            str = readInput();
        }

        tokenizeInput(str, &tokens, &tokenCount);

        commandHandler(tokens);

        for(int i = 0; i < tokenCount; i++) {
            free(tokens[i]);
        }
        free(tokens);
        free(str);
    }
}
