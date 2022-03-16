#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/prompt.h"
#include "../include/input.h"
#include "../include/split.h"
#include "../include/proc.h"
#include "../include/exec.h"
#include "../include/var.h"
#include "../include/macros.h"

int prompt(VarList * varlist, VarList * aliaslist) {

    //PRINT PROMPT
    print_prompt();

    //GET INPUT
    char * line;
    int input_ret = input(&line);
    switch(input_ret) {
        case NO_PROMPT_RETURN_VALUE:
            printf("\n\n\tSession ended with 'Ctrl+d'\n");
            return NO_PROMPT_RETURN_VALUE;
            break;
        case REPROMPT_RETURN_VALUE:
            return REPROMPT_RETURN_VALUE;
            break;
        default:
            break;
    }

    //SPLIT INPUT INTO AN ARRAY OF WORDS
    Word_list words;
    words.num = count_spaces(line);
    bool var_assignment = false; //variable signaling the need to create shell variable
    words.pointer = split(line, &words.num, &var_assignment, varlist); //split the string
    if(words.pointer == NULL) { //if input is only enter reprompt
        return REPROMPT_RETURN_VALUE;
    }

    //PROC
    Command_list commands;
    proc(&words, &commands); //process separators (|, >, <, >>)

    //EXEC
    int exec_return = exec(&words, &commands, var_assignment, varlist, aliaslist); //execute the programs


    //FREE EVERYTHING
    for(int i = 0; i < words.num; ++i) {
        if(words.pointer[i] != NULL) {
            free(words.pointer[i]);
        }
    }
    free(line);
    free(words.pointer);
    free(commands.types);
    free(commands.locations);


    //RETURN or ABORT
    if(exec_return == REPROMPT_RETURN_VALUE) {
        return REPROMPT_RETURN_VALUE;
    } else if(exec_return == EXECUTE_ERR
            ||exec_return == CD_RETURN
            ||exec_return == HISTORY_RETURN){
        abort();
    } else if(exec_return == SOMETHING_WRONG) {
        return NO_PROMPT_RETURN_VALUE;
    } else {
        return NO_PROMPT_RETURN_VALUE;
    }
}

//Function for printing out the shell prompt
void print_prompt(void) {
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX); //Get hostname env variable
    char cwd[256];
    getcwd(cwd, sizeof(cwd)); //Get cwd env variable

    //If inside home directory replace home with ~
    if(strstr(cwd, getenv("HOME")) != NULL) {
        char * new_path = (char*) malloc(strlen(cwd) - strlen(getenv("HOME") + 2));
        new_path[0] = '~';
        new_path[1] = '\0';
        strcat(new_path, cwd + strlen(getenv("HOME")));
        printf("[%s@%s]:%s$ ", getenv("USER"), hostname, new_path); //print prompt
        free(new_path);
    } else {
        //print out the prompt
        printf("[%s@%s]:%s$ ", getenv("USER"), hostname, cwd); //print prompt
    }
}
