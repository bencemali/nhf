#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/input.h"
#include "../include/debugmalloc.h"

char * input(void) {
    int size = 1; //How many characters the string contains at any point
    int position = 0; //Where we are in the allocated memory
    char * line = (char*) malloc(size * sizeof(char));
    line[0] = '\0';
    char newchar;
    while(scanf("%c", &newchar) == 1 && newchar != '\n' && newchar != EOF) {
        if(position + 1 >= size) {
            size *= 2;
            line = (char*) realloc(line, size * sizeof(char));
        }
        line[position] = newchar;
        line[position+1] = '\0';
        ++position;
    }

    line = space_collapse(line);

    printf("%s\n", line);

    return line;
}

char * space_collapse(char * string) {
    int len = strlen(string);
    int new_len = 0;
    bool prev_space = true;
    for(int i = 0; i < len; ++i) {
        if(!prev_space) {
            ++new_len;
        } else if(!isspace(string[i])) {
            ++new_len;
        }

        if(isspace(string[i])) {
            prev_space = true;
        } else {
            prev_space = false;
        }
    }

    char * new_string = (char*) malloc((new_len + 1) * sizeof(char));

    int chars_in_new = 0;
    prev_space = true;
    for(int i = 0; i < len; ++i) {
        if(!prev_space) {
            new_string[chars_in_new] = string[i];
            ++chars_in_new;
        } else if(!isspace(string[i])) {
            new_string[chars_in_new] = string[i];
            ++chars_in_new;
        }

        if(isspace(string[i])) {
            prev_space = true;
        } else {
            prev_space = false;
        }
    }
    new_string[new_len] = '\0';

    free(string);

    return new_string;
}