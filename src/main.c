#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool builtIn(char *str) {
    const char* shell_commands[] = {
        "cd",
        "pwd",
        "echo",
        "exit",
        "source",
        "kill",
        "man"
    };

    int size = sizeof(shell_commands) / sizeof(shell_commands[0]);

    for(int i = 0; i < size; i++) {
        if(strcmp(str, shell_commands[i]) == 0) {
            return true;
        }
    }
    
    return false;
}

void exitShell() {
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {    
    char *str = NULL, c = '\0';

    int i = 0;

    str = (char*)malloc(sizeof(char));

    printf("sheLLiFY >>> ");

    while ((c = getchar()) != '\n') {
        str = realloc(str, (i + 1) * sizeof(char));
        str[i++] = c;
    }

    if(builtIn(str)) {
        printf("%s is a built in command\n", str);
    }
    else {
        printf("%s is just random words\n", str);
    }

    free(str);
}
