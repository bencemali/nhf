#ifndef PROMPT_INCLUDED
#define PROMPT_INCLUDED
#include "var.h"


void print_prompt(void);

int prompt(VarList * varlist, VarList * aliaslist);

#endif
