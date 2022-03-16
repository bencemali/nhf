#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/split.h"
#include "../include/input.h"
#include "../include/var.h"

//function for counting spaces in a string
//used to determine number of words
int count_spaces(char * string) {
    if(string[0] == '\0') {
        return 0;
    }
    int cnt = 0; //How many spaces have been counted
    bool quotation = false;
    bool first_seperator = false;

    for(int i = 0; string[i] != '\0'; ++i) {
        if(string[i] == '"') {
            quotation = !quotation;
        } else if(string[i] == ' ' && !quotation) {
            first_seperator = true;
            ++cnt;
        } else if(string[i] == '=' && !quotation && first_seperator == false) {
            first_seperator = true;
            ++cnt;
        }
    }

    ++cnt;
    return cnt;
}

//function for splitting up the input into words
char ** split(char * string, int * num_of_spaces, bool * var_assignment, VarList * varlist) {
    if(string[0] == '\0') { //If there was no input
        return NULL;
    }

    char ** array = (char**) malloc((*num_of_spaces + 1) * sizeof(char*)); //Stores strings of the words

    int from = 0;
    int how_many = 0;
    int separator_count = 0;
    bool aliasing = false;

    for(int i = 0; string[from] != '\0'; ++i) {
        how_many = 0;
        if(from != 0) {
            ++from;
        }
        if(string[from] == '"') {
            ++from;
            while(string[from + how_many] != '"' && string[from + how_many] != '\0') {
                ++how_many;
            }
            array[i] = (char*) malloc((how_many + 1) * sizeof(char));
            strncpy(array[i], string + from, how_many);
            array[i][how_many] = '\0';
            from += (how_many + 1);
        } else {
            while(string[from + how_many] != ' ' && string[from + how_many] != '\0') {
                if(string[from + how_many] == '=' && separator_count < 2) {
                    if(separator_count == 1 && aliasing) {
                        break;
                    } else {
                        *var_assignment = true;
                        break;
                    }
                }
                ++how_many;
            }
            ++separator_count;
            if(string[from] == '$') {
                if(value_search(varlist, string + from + 1) != NULL) {
                    char * var_value = value_search(varlist, string + from + 1);
                    array[i] = (char*) malloc(strlen(var_value) + 1);
                    strcpy(array[i], var_value);
            } else {
                    char * envvar = getenv(string + from + 1);
                    if(envvar != NULL) {
                        array[i] = (char*) malloc(strlen(envvar) + 1);
                        strcpy(array[i], envvar);
                    } else {
                        array[i] = (char*) malloc(sizeof(char));
                        array[i][0] = '\0';
                    }
                }
            } else {
                if(from == 0 && strcmp(string + from, "alias") == 0) {
                    aliasing = true;
                }
                array[i] = (char*) malloc((how_many + 1) * sizeof(char));
                strncpy(array[i], string + from, how_many);
                array[i][how_many] = '\0';
            }
            from += how_many;
        }
    }

    array[*num_of_spaces] = NULL; //End the array with a NULL, so last command can also be passed as argument to execv

    return array;
}

