#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../include/prompt.h"
#include "../include/input.h"
#include "../include/split.h"
#include "../include/proc.h"
#include "../include/exec.h"
#include "../include/debugmalloc.h"

int prompt(void) {
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    printf("[%s@%s]:%s$ ", getenv("USER"), hostname, getenv("PWD")); //print prompt
    char * line = input();
    Word_list words;
    words.num = count_spaces(line);
    words.pointer = split(line, words.num);
    free(line);
    if(words.pointer == NULL) {
        return 0;
    }
    Command_list commands;
    proc(&words, &commands);
    //printf("Command count: %d\n", commands.num);
    /*
    for(int i = 0; i < commands.num; ++i) {
        if(commands.types[i] == program) {
            printf("%d: \n\tprogram\n\tIndex: %d\n", i, commands.locations[i]);
        } else if(commands.types[i] == file_out) {
            printf("%d: \n\tfile_out\n\tIndex: %d\n", i, commands.locations[i]);
        } else if(commands.types[i] == file_in) {
            printf("%d: \n\tfile_in\n\tIndex: %d\n", i, commands.locations[i]);
        }
    }
    */

    //EXEC
    int exec_return = exec(&words, &commands);

    //FREEing
    for(int i = 0; i < words.num; ++i) {
        if(words.pointer[i] != NULL) {
            free(words.pointer[i]);
        }
    }
    free(words.pointer);
    free(commands.types);
    free(commands.locations);

    //RETURN
    if(exec_return == 0) {
        return 0;
    } else {
        return 1;
    }
}
