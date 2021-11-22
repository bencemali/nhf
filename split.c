#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "split.h"
#include "input.h"
//#include "debugmalloc.h"

int count_spaces(char * string) {
    int cnt = 0;
    for(int i = 0; string[i] != '\0'; ++i) {
        if(string[i] == ' ') {
            ++cnt;
        }
    }
    ++cnt;
    return cnt;
}

char ** split(char * string, int cnt) {
    char ** array = (char**) malloc(cnt * sizeof(char*));

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
    return array;
}