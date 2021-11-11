#include "exec.h"

int exec_input(Word_list * word_list, Command_list * command_list) {
	int * pipefds;
	pipefds = (int*) malloc(command_list->num * 2 * sizeof(int));
	for(int z = 0; z < (command_list->count * 2); z += 2) {
		pipe(pipefds + z);
	}
	int rank;
	rank = 0;
	int children[] = (int*)malloc(command_list->num * sizeof(char));
	pid_t parentid = getpid();
	for(int i = 1; i < (command_list->num + 1); ++i) {
		if(getpid() == parentid) {
			int pid = fork();
			if(getpid() != parentid) {
				rank = i;
			} else {
				children[i - 1] = pid;
			}
		}
	}

	if(rank != 0) {
		switch(command_list->types[rank - 1]) {
			case program:
				if(rank == 1) {
					for(int i = 1; i < command_list->num; ++i) {
						if(command_list->num != 1) {
							if(command_list->types[i] == program || command_list->types[i] == file_out) {
								for(int j = 1; j < (command_list->num * 2); ++j) {
									if(j != (2 * i - 1)){
										close(pipefds + j);
									}
								}
							}
							if(command_list[i + 1] == file_in) {
								dup2(pipefds[0], fileno(stdin));
							}
							close(pipefds[0]);
							dup2(pipefds[2 * i - 1], fileno(stdout));
							close(pipefds[2 * i - 1]);
							execvp(word_list->pointer[0], word_list->pointer[0]);
						}
					}
				}
				break;
		}
	}

	if(rank == 0) {
		for(int i = 0; i < command_list->num; ++i) {
			waitpid(children[i]);
		}
	}
}