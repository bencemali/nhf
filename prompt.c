#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "prompt.h"
#include "input.h"
#include "split.h"
#include "proc.h"
#include "exec.h"
//#include "debugmalloc.h"

int prompt(void) {
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    printf("[%s@%s]:%s$ ", getenv("USER"), hostname, getenv("PWD")); //print prompt
    char * line = input();
    Word_list words;
    words.num = count_spaces(line);
    words.pointer = split(line, words.num);
    for(int i = 0; i < words.num; ++i) {
        printf("%d: ", i);
        printf("%s\n", words.pointer[i]);
    }
    Command_list commands;
    proc(&words, &commands);
    printf("Command count: %d\n", commands.num);
    for(int i = 0; i < words.num; ++i) {
        if(words.pointer[i] != NULL) {
            printf("%d: ", i);
            printf("%s\n", words.pointer[i]);
        }
    }
    for(int i = 0; i < commands.num; ++i) {
        if(commands.types[i] == program) {
            printf("%d: \n\tprogram\n\t%d\n", i, commands.locations[i]);
        } else if(commands.types[i] == file_out) {
            printf("%d: \n\tfile_out\n\t%d\n", i, commands.locations[i]);
        } else if(commands.types[i] == file_in) {
            printf("%d: \n\tfile_in\n\t%d\n", i, commands.locations[i]);
        }
    }
}
