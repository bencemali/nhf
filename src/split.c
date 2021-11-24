#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../include/split.h"
#include "../include/input.h"
#include "../include/debugmalloc.h"

int count_spaces(char * string) {
    if(string[0] == '\0') {
        return 0;
    }
    int cnt = 0; //How many spaces have been counted
    for(int i = 0; string[i] != '\0'; ++i) {
        if(string[i] == ' ') {
            ++cnt;
        }
    }
    ++cnt;
    return cnt;
}

char ** split(char * string, int num_of_spaces) {
    if(string[0] == '\0') { //If there was no input
        return NULL;
    }

    char ** array = (char**) malloc((num_of_spaces + 1) * sizeof(char*)); //Stores strings of the words

    int from = 0;
    int how_many = 0;

    for(int i = 0; string[from] != '\0'; ++i) {
        if(from != 0) {
            ++from;
        }
        how_many = 0;
        while(string[from + how_many] != ' ' && string[from + how_many] != '\0') {
            ++how_many;
        }
        array[i] = (char*) malloc((how_many + 1) * sizeof(char));
        strncpy(array[i], string + from, how_many);
        array[i][how_many] = '\0';
        from += how_many;
    }

    array[num_of_spaces] = NULL; //End the array with a NULL, so last command can also be passed as argument to execv

    return array;
}