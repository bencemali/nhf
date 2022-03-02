#ifndef EXEC_INCLUDED
#define EXEC_INCLUDED
#include "proc.h"
#include "split.h"
#include "var.h"
#include "macros.h"

void close_pipes(int size, int * pipefds);

int pipe_end_by_diff(char * read_or_write_end, int own_rank, int diff);

int next_type_diff(Type sought_type, Type * types, int size, int own_rank);

int stdout_redirect(int * pipefds, Word_list * words, Command_list * commands, int own_rank);

int stdin_redirect(int * pipefds, Word_list * words, Command_list * commands, int own_rank);

int exec(Word_list * word_list, Command_list * command_list, bool var_assignment, VarList * varlist, VarList * aliaslist);

#endif
