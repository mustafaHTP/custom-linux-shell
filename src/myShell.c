/**
 * @file myShell.c
 * @author mustafaHTP - Mustafa Hatipoğlu
 * @brief custom linux shell for bash terminal
 * @version 0.1
 * @date 2022-10-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "myShell.h"
#include <fcntl.h> /*open, read, write ...*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /*pid_t*/
#include <sys/wait.h>  /*wait()*/
#include <unistd.h>    /*pipe, exec, fork, dup2 ...*/

void smile() {
    printf("                -----------                    \n");
    printf("           ----             ----               \n");
    printf("          |                     |              \n");
    printf("          |     OO        OO    |              \n");
    printf("          |     OO        OO    |              \n");
    printf("          |                     |              \n");
    printf("          | --               -- |              \n");
    printf("          |   ---          ---  |              \n");
    printf("          |      ---------      |              \n");
    printf("          |                     |              \n");
    printf("           ---------------------               \n");
    printf("                                               \n");
    putchar('\n');
}

/**
 * @brief free resources of char ** args
 * since the last element of args is NULL,
 * it does not need number of arguments
 *
 *
 * @param args char **
 */
void myFree(char** args) {

    if (args == NULL) {
        return;
    }

    int i = 0;
    /*since last element is null
     * it goes to Null element
     */
    while (args[i] != NULL) {
        free(args[i]);
        ++i;
    }

    free(args[i]);
    free(args);
}

/**
 * @brief checks path is valid and
 * tries to find '/' char
 * valid path : /usr/bin/ls
 *
 * @param arr
 * @return true
 * @return false
 */
bool pathIsValid(char arr[]) {
    int sizeOfArr = strlen(arr);
    bool isValid = false;
    for (int i = 0; i < sizeOfArr; i++) {
        if ('/' == arr[i] ) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Get the System Path Of Command  if there is
 *
 * @param command
 * @return char* -> system path
 * if there is not, -> NULL
 */
char* getSystemPathOfCommand(char* command) {
    /*In parent process, path to be read*/
    /*In child process, path to be written to pipe (file_descriptors[1])*/
    /*init pipe*/
    int file_descriptors[2];
    if (pipe(file_descriptors) == -1) {
        perror("pipe: ");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid > 0) {
        /*PARENT PROCESS*/
        close(file_descriptors[1]);
        wait(NULL);

        /*read from pipe*/
        char buffer[BUFSIZ];
        int ret = read(file_descriptors[0], buffer, BUFSIZ);
        if (ret == -1) {
            return NULL;
        }
        buffer[ret] = '\0';
        close(file_descriptors[0]);

        /*get path*/
        bool isValid = pathIsValid(buffer);
        if (isValid == true) {
            char* getPath = strtok(buffer, " ");
            getPath = strtok(NULL, " ");
            int sizeOfPath = strlen(getPath);
            char* command_path = calloc(sizeOfPath + 1, sizeof(char));
            if (command_path == NULL) {
                perror("calloc :");
                return NULL;
            }
            strncpy(command_path, getPath, sizeOfPath);
            return command_path;
        } else {
            return NULL;
        }

    } else if (pid == 0) {
        /*CHILD PROCESS*/
        close(file_descriptors[0]);
        /* save std_out */
        int save_out;
        dup2(1, save_out);
        close(1);

        /* direct std_out to pipe*/
        dup2(file_descriptors[1], 1);

        pid_t pid_exec = fork();
        if (pid_exec > 0) {
            wait(NULL);
        } else if (pid_exec == 0) {
            int ret = execl("/usr/bin/whereis", "whereis", command, NULL);
            if (ret == -1) {
                exit(EXIT_FAILURE);
            }
        } else {
            exit(EXIT_FAILURE);
        }

        /* restore std_out */
        dup2(save_out, 1);

        close(save_out);
        close(file_descriptors[1]);

        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Get the current Path Of Command
 *
 * @param command
 * @return char* current path
 *
 */
char* getCurrentPathOfCommand(char* command) {
    /*In parent process, path to be read*/
    /*In child process, path to be written to pipe (file_descriptors[1])*/
    /*init pipe*/
    int file_descriptors[2];
    if (pipe(file_descriptors) == -1) {
        perror("pipe: ");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid > 0) {
        /*PARENT PROCESS*/
        close(file_descriptors[1]);
        wait(NULL);

        /*read from pipe*/
        char buffer[BUFSIZ];
        int ret = read(file_descriptors[0], buffer, BUFSIZ);
        if (ret == -1) {
            return NULL;
        }
        buffer[ret] = '\0';
        
        close(file_descriptors[0]);

        bool isValid = pathIsValid(buffer);
        if(isValid == true){
            /*get current path*/
            char* getPath = strtok(buffer, " ");
            int sizeOfPath = strlen(getPath);

            char* command_path = calloc(sizeOfPath + 1, sizeof(char));
            if (command_path == NULL) {
                perror("calloc: ");
                return NULL;
            }
            strncpy(command_path, getPath, sizeOfPath);

            return command_path;

        }else{
            return NULL;
        }

    } else if (pid == 0) {
        /*CHILD PROCESS*/
        close(file_descriptors[0]);

        /* save std_out */
        int save_out;
        dup2(1, save_out);
        close(1);

        /* direct std_out to pipe */
        dup2(file_descriptors[1], 1);

        pid_t pid_exec = fork();
        if (pid_exec > 0) {
            wait(NULL);
        } else if (pid_exec == 0) {
            int ret = execl("/usr/bin/pwd", "pwd", NULL);
            if (ret == -1) {
                exit(EXIT_FAILURE);
            }
        } else {
            exit(EXIT_FAILURE);
        }

        /* restore std_out */
        dup2(save_out, 1);

        close(save_out);
        close(file_descriptors[1]);

        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief parses the input and
 * calculate the number of arguments
 * EXP:
 * "head -n 10 foo.txt"
 *  -> 4
 * @param input
 * @return int number of arguments
 */
int getNumOfArgs(char* input) {
    int numOfArgs = 0;
    char* arg = strtok(input, " ");
    ++numOfArgs;

    while (arg != NULL) {
        arg = strtok(NULL, " ");
        ++numOfArgs;
    }

    return numOfArgs - 1;
}

/**
 * @brief Parses the input into arguments
 *
 * EXP:
 * "head -n 5 foo.txt"
 * arguments[0] = "head"
 * arguments[1] = "-n"
 * arguments[2] = "5"
 * arguments[3] = "foo.txt"
 *
 * @param input
 * @return char**
 */
char** getArguments(char* input) {
    /* We have to copy input into 2 strings because
     *  strtok() change string
     */
    char copy_input[BUFSIZ];     /* To decide how many arguments */
    char copy_arguments[BUFSIZ]; /* To parse input */
    strcpy(copy_input, input);
    strcpy(copy_arguments, input);

    int numOfArgs = getNumOfArgs(copy_input);

    char** arguments = calloc(numOfArgs + 1, sizeof(char*));
    if (arguments == NULL) {
        return NULL;
    }

    /*allocate memory for arguments depending their length*/
    char* argument = NULL;
    for (int i = 0; i < numOfArgs; i++) {
        if (i == 0) {
            argument = strtok(copy_arguments, " ");
        } else {
            argument = strtok(NULL, " ");
        }

        int size_of_arg = strlen(argument);
        arguments[i] = calloc((size_of_arg + 1), sizeof(char));
        strncpy(arguments[i], argument, size_of_arg);
    }
    arguments[numOfArgs] = NULL;
    return arguments;
}


/**
 * @brief takes input and create a new process
 * for command and run it
 * 
 * @param input 
 */
void runCommand(char* input) {
    /*get arguments from input*/
    char** arguments = getArguments(input);
    /*get command path if there is*/
    char* command_path = getSystemPathOfCommand(arguments[0]);
    /**
     * @brief first check system path, if there is, then run
     * if there is not system path, then try current path
     *
     */
    if (command_path != NULL) {
        pid_t pid = fork();
        if (pid > 0) {
            wait(NULL);
        } else if (pid == 0) {
            int ret = execv(command_path, arguments);
            if (ret == -1) {
                exit(EXIT_FAILURE);
            }
        }
    } else {
        command_path = getCurrentPathOfCommand(input);

        pid_t pid = fork();
        if (pid > 0) {
            wait(NULL);
        } else if (pid == 0) {
            int ret = execv(command_path, arguments);
            if (ret == -1) {
                printf("%s : command not found\n", arguments[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    /*prevent memory leak*/
    myFree(arguments);
    free(command_path);
}

void runTerminal() {
    char input[BUFSIZ];
    while (1) {
        printf(">> ");
        fgets(input, BUFSIZ, stdin);
        input[strcspn(input, "\n")] = '\0'; /*get rid of the newline character*/

        /*check input*/
        if (strncmp(input, "smile", 5) == 0) {
            smile();
        } else if (strncmp(input, "exit", 4) == 0) {
            break;
        } else {
            runCommand(input);
        }
    }
    system("clear");
    printf("Have a nice day :) \n");
}