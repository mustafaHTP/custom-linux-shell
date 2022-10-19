/**
 * @file myShell.h
 * @author mustafaHTP - Mustafa Hatipoğlu
 * @brief 
 * @version 0.1
 * @date 2022-10-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdbool.h>

void smile();

/**
 * @brief free resources of char ** args
 * since the last element of args is NULL,
 * it does not need number of arguments
 *
 *
 * @param args char **
 */
void myFree();

/**
 * @brief checks path is valid and
 * tries to find '/' char
 * valid path : /usr/bin/ls
 *
 * @param arr
 * @return true
 * @return false
 */
bool pathIsValid();

/**
 * @brief Get the System Path Of Command  if there is
 *
 * @param command
 * @return char* -> system path
 * if there is not, -> NULL
 */
char* getSystemPathOfCommand(char*);

/**
 * @brief Get the current Path Of Command
 *
 * @param command
 * @return char* current path
 *
 */
char* getCurrentPathOfCommand(char*);

/**
 * @brief parses the input and
 * calculate the number of arguments
 * EXP:
 * "head -n 10 foo.txt"
 *  -> 4
 * @param input
 * @return int number of arguments
 */
int getNumOfArgs(char*);

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
char** getArguments(char*);

/**
 * @brief takes input and create a new process
 * for command and run it
 * 
 * @param input 
 */
void runCommand(char*);

void runTerminal();