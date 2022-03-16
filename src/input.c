#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/input.h"
#include "../include/macros.h"


//function for reading a line of input dynamically
int input(char ** line) {
    int size = 0; //How many characters the string contains at any point
    int position = 0; //Where we are in the allocated memory
    *line = (char*) malloc(size * sizeof(char));
    char newchar;
    while(scanf("%c", &newchar) != EOF && newchar != '\n') {
        if(!size) {
            ++size;
            *line = (char*)realloc(*line, size * sizeof(char));
        }
        if(position + 1 >= size) {
            size *= 2;
            *line = (char*) realloc(*line, size * sizeof(char));
        }
        (*line)[position] = newchar;
        (*line)[position+1] = '\0';
        ++position;
    }

    if(size == 0) {
        if(newchar == '\n') {
            free(*line);
            return REPROMPT_RETURN_VALUE;
        } else {
            free(*line);
            return NO_PROMPT_RETURN_VALUE;
        }
    }

    *line = space_collapse(*line);

    /*
    FILE * fp = fopen("~/.shell_history", "a");
    fprintf(fp, "%s\n", *line);
    fclose(fp);
    */


    return OK_RETURN;

}


//function for removing extra spaces
char * space_collapse(char * string) {
    int len = 0;
    for(int i = 0; string[i] != '\0'; ++i) {
        ++len;
    }
    int new_len = 0;
    bool prev_space = true;
    for(int i = 0; i < len; ++i) {
        if(string[i] == '"') {
            ++i;
            ++new_len;
            while(string[i] != '"') {
                ++i;
                ++new_len;
            }
            ++i;
            ++new_len;
        } else {
            if(!prev_space) {
                ++new_len;
            } else if(string[i] != ' ') {
                ++new_len;
            }

            if(string[i] == ' ') {
                prev_space = true;
            } else {
                prev_space = false;
            }
        }
    }

    char * new_string = (char*) malloc((new_len + 1) * sizeof(char));

    int chars_in_new = 0;
    prev_space = true;
    for(int i = 0; i < len; ++i) {
            if(string[i] == '"') {
                new_string[chars_in_new] = string[i];
                ++chars_in_new;
                ++i;
                while(string[i] != '"') {
                    new_string[chars_in_new] = string[i];
                    ++chars_in_new;
                    ++i;
                }
                new_string[chars_in_new] = string[i];
                ++chars_in_new;
                ++i;
            }
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
