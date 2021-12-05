#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../include/exec.h"
#include "../include/split.h"
#include "../include/input.h"
#include "../include/proc.h"
#include "../include/debugmalloc.h"
#include "../include/var.h"


#define EXIT_WITH_QUIT 22 //returned if exit or quit was inputted
#define REPROMPT_RETURN_VALUE 0 //returned if process has to display a new prompt
#define NO_PROMPT_RETURN_VALUE 33 //returned if the process doesn't have to display the next prompt
#define NO_COMMAND_WITH_TYPE 0 //returned if there isn't a command looked for
#define SOMETHING_WRONG 999 //returned if something unexpectedly went wrong
#define EXECUTE_ERR 420 //returned if couldn't execute program
#define CD_RETURN 343 //returned if input is 'cd' and child process doesn't need to do anything
#define HISTORY_RETURN 555 //will be used later
#define OK_RETURN 777 //standard return value


//Closes ALL pipe ends
void close_pipes(int size, int * pipefds) {
    for(int i = 0; i < size; ++i) {
        close(pipefds[i]);
    }
    free(pipefds);
}

//Finds read or write end of pipe
//Accepts current process' rank
//Gives back pipefd index for other process by difference
int pipe_end_by_diff(char * read_or_write_end, int own_rank, int diff) {
    if(strcmp("read", read_or_write_end) == 0) {
        return ((own_rank + diff) - 1) * 2;
    } else if(strcmp("write", read_or_write_end) == 0){
        return (((own_rank + diff) * 2) - 1);
    }
    return SOMETHING_WRONG;
}

//Finds difference between next 'type' type command rank and current process rank
int next_type_diff(Type sought_type, Type * types, int size, int own_rank) {
    for(int i = own_rank; i < size; ++i) {
        if(types[i] == sought_type) {
            return (i - own_rank + 1);
        }
    }
    return NO_COMMAND_WITH_TYPE;
}

//function for redirecting stdout to file or process
int stdout_redirect(int * pipefds, Word_list * words, Command_list * commands, int own_rank) {
    if(commands->num == 1) {
        return OK_RETURN;
    }
    for(int i = own_rank; i < commands->num; ++i) {
        if(commands->types[i] == program) {
            int pipe_index = pipe_end_by_diff("write", own_rank, i - own_rank + 1);
            dup2(pipefds[pipe_index], fileno(stdout));
            return OK_RETURN;
        } else if(commands->types[i] == file_out) {
            if(freopen(words->pointer[commands->locations[own_rank]], "w", stdout) == NULL) {
                fprintf(stderr, "Couldn't open %s for writing\n", words->pointer[commands->locations[own_rank]]);
                return SOMETHING_WRONG;
            }
            return OK_RETURN;
        } else if(commands->types[i] == file_append) {
            if(freopen(words->pointer[commands->locations[own_rank]], "a", stdout) == NULL) {
                fprintf(stderr, "Couldn't open %s for writing\n", words->pointer[commands->locations[own_rank]]);
                return SOMETHING_WRONG;
            }
            return OK_RETURN;
        }
    }
    return OK_RETURN;
}

//function for redirecting a process' stdin to a pipe end
int stdin_redirect(int * pipefds, Word_list * words, Command_list * commands, int own_rank) {
    if(commands->num == 1) {
        return OK_RETURN;
    } else if(own_rank == 1) { 
        bool stdin_needed = false;
        int i;
        for(i = own_rank; i < commands->num; ++i) {
            if(commands->types[i] != file_in) {
                break;
            }
            stdin_needed = true;
        }
        if(stdin_needed == true) {
            if(freopen(words->pointer[commands->locations[i - 1]], "rt", stdin) == NULL) { //get stdin from file
                fprintf(stderr, "Couldn't open %s for reading\n", words->pointer[commands->locations[i]]);
                return SOMETHING_WRONG;
            }
        } else {
            return OK_RETURN;
        }
    } else {
        dup2(pipefds[(own_rank - 1) * 2], fileno(stdin));
        return OK_RETURN;
    }
    return OK_RETURN;
}



int exec(Word_list * word_list, Command_list * command_list, bool var_assignment, VarList * varlist, VarList * aliaslist) {
    //QUIT/EXIT
    if(command_list->num == 1 && (strcmp(word_list->pointer[0], "quit") == 0 || strcmp(word_list->pointer[0], "exit") == 0)) { //if input is exit or quit, don't execute anything
        return(EXIT_WITH_QUIT);
    }

    //PIPE
	int * pipefds; //array for storing pipe file descriptors
    pipefds = (int*) malloc(command_list->num * 2 * sizeof(int)); for(int z = 0; z < (command_list->num * 2); z += 2) {
		pipe(pipefds + z);
	}

    //FORKING
	int rank; //variable for distinguishing process'
	rank = 0;
	pid_t parentid = getpid(); //variable storing parent's pid
	for(int i = 1; i < (command_list->num + 1); ++i) {
		if(getpid() == parentid) { //only parent forks
            if(command_list->types[i - 1] == program) {
                fork();
                if(getpid() != parentid) {
                    rank = i;
                } else {
                }
            }
		}
	}

    //CHILDREN
    if(rank != 0) {

        //STDOUT
        int stdout_ret = stdout_redirect(pipefds, word_list, command_list, rank);
        switch(stdout_ret) {
            case OK_RETURN:
                break;
            case SOMETHING_WRONG:
                return SOMETHING_WRONG;
                break;
            default:
                break;
        }

        //STDIN
        int stdin_ret = stdin_redirect(pipefds, word_list, command_list, rank);
        switch(stdin_ret) {
            case OK_RETURN:
                break;
            case SOMETHING_WRONG:
                return SOMETHING_WRONG;
                break;
            default:
                break;
        }

        close_pipes(command_list->num * 2, pipefds);

        if(value_search(aliaslist, word_list->pointer[command_list->locations[rank - 1]]) != NULL) {
            char * alias_value = value_search(aliaslist, word_list->pointer[command_list->locations[rank - 1]]);
            free(word_list->pointer[command_list->locations[rank - 1]]);
            word_list->pointer[command_list->locations[rank - 1]] = (char*)malloc(sizeof(alias_value));
            strcpy(word_list->pointer[command_list->locations[rank - 1]], alias_value);
            execvp(word_list->pointer[command_list->locations[rank - 1]], word_list->pointer + command_list->locations[rank - 1]);
        }
        if(var_assignment && rank == 1) {
            return VAR_ASSIGNMENT;
        } else if(strcmp(word_list->pointer[command_list->locations[rank - 1]], "alias") == 0) {
            return VAR_ASSIGNMENT;
        } else if(strcmp(word_list->pointer[command_list->locations[rank - 1]], "cd") == 0) {
            return CD_RETURN;
        } 
            /*else if(strcmp(word_list->pointer[command_list->locations[rank - 1]], "history") == 0) {
            FILE * shell_history;
            shell_history = fopen("~/shell_history", "r");
            if(shell_history == NULL) {
                return HISTORY_RETURN;
            }
            char buffer[256];
            while(fgets(buffer, 256, shell_history) != NULL) {
                fputs(buffer, stdout);
            }
            fclose(shell_history);
            return HISTORY_RETURN;
        }*/ 
            else {
            int exec_return = execvp(word_list->pointer[command_list->locations[rank - 1]], word_list->pointer + command_list->locations[rank - 1]);
            if(exec_return == -1) {
                fprintf(stderr, "\n\tCommand not found\n\n");
                return EXECUTE_ERR;
            }
        }

    //PARENT
    } else {
        if(strcmp(word_list->pointer[0], "alias") == 0 && !var_assignment) { //if there is need to create alias
            add_var(aliaslist, word_list->pointer[1], word_list->pointer[2]);
        } else if(var_assignment) { //if there is need to create shell variable
            add_var(varlist, word_list->pointer[0], word_list->pointer[1]);
        } else {
            for(int i = 0; i < command_list->num; ++i) {
                if(strcmp(word_list->pointer[command_list->locations[i]], "cd") == 0) { //if input is cd, to change directory
                    if(command_list->locations[i] == word_list->num - 1 || word_list->pointer[command_list->locations[i] + 1] == NULL) {
                        chdir(getenv("HOME"));
                    } else if(word_list->pointer[command_list->locations[i] + 1][0] == '~') { //if withing home directory replace home with ~
                        char * home = getenv("HOME");
                        char *new_path = (char*) malloc(strlen(word_list->pointer[command_list->locations[i] + 1]) + strlen(home));
                        strcpy(new_path, home);
                        strcat(new_path, word_list->pointer[command_list->locations[i] + 1] + 1);
                        if(chdir(new_path)) {
                            fprintf(stderr, "cd : '%s': no such file or directory\n", new_path);
                        }
                        free(new_path);
                    } else {
                        if(chdir(word_list->pointer[command_list->locations[i] + 1])) {
                            fprintf(stderr, "cd: '%s': no such file or directory\n", word_list->pointer[command_list->locations[i] + 1]);
                        }
                    }
                }
            }
        }
        close_pipes(command_list->num * 2, pipefds);
        //free(children);
		for(int i = 0; i < command_list->num; ++i) {
			wait(NULL); //wait for children to finish execution
		}
        return REPROMPT_RETURN_VALUE; //return to display next prompt
	}

    return 2; //something wrong
}
