#include <errno.h>
#include <stdio.h>
#include "include/prompt.h"
#include "include/var.h"
#include "include/macros.h"

int main(void) {
    int prompt_return_value = 0; //variable storing the return value that determines if process prints next prompt

    VarList varlist; //Linked list storing shell variables
    varlist.begin = NULL;
    varlist.end = NULL;
    VarList aliaslist; //Linked list storing aliases
    aliaslist.begin = NULL;
    aliaslist.end = NULL;

    while(prompt_return_value == REPROMPT_RETURN_VALUE) { //while not exit or quit input
    prompt_return_value = prompt(&varlist, &aliaslist); //main just calls the prompt function
    }

    free_varlist(&varlist); //free linked lists
    free_varlist(&aliaslist);

    return 0;
}
