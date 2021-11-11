#include "prompt.h"

void prompt(void) {
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    printf("[%s@%s]:%s$ ", getenv("USER"), hostname, getenv("PWD")); //print prompt
    char * line = line_in(); // this function gets the line
    Word_list * words = line_to_word(line); // this function breaks the inputted line into words
    if(strcmp(words->pointer[0], "quit") == 0) { // if first word of inputted line is quit, then stop the program
        exit(0);
    }
    Command_list * commands; /* this variable will store the input, where words of the input are stored in an array,
                                all commands and arguments end with a NULL, but they are in the same array */
    proc_list(words, commands); // this function processes the array of words
    exec_input(words, commands); // this function will execute all commands

    free(line);
    free(words->pointer);
    free(commands->locations);
    free(commands->types);
    prompt(); // prompt recursively calls itself, so you can type the next command
}