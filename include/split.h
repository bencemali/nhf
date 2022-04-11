#ifndef SPLIT_INCLUDED
#define SPLIT_INCLUDED
#include <stdbool.h>
#include "var.h"

//struct for storing number of words, and the array of words
typedef struct Word_list {
    int num;
    char **pointer;
} Word_list;

int count_spaces(char * string);

char ** split(char * string, int * num_of_spaces, bool * var_assignment, VarList * varlist);

#endif

