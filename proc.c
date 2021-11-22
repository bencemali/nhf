#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "proc.h"
#include "split.h"
#include "input.h"

void proc(Word_list * word_list, Command_list * result) {
	result->num = 1;
	for(int i = 0; i < word_list->num; ++i) { // This for loop counts how many commands there are
		if(strcmp(word_list->pointer[i], "|") == 0
		|| strcmp(word_list->pointer[i], ">") == 0
		|| strcmp(word_list->pointer[i], "<") == 0) {
			++(result->num);
		}
	}
	result->types = (Type*)malloc((result->num + 1) * sizeof(Type));
	result->locations = (int*)malloc((result->num + 1) * sizeof(int));
	result->types[0] = program;
	result->locations[0] = 0;
	for(int i = 1; i < word_list->num; ++i) {
		if(strcmp(word_list->pointer[i], "|") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			result->types[i] = program;
			result->locations[i] = i + 1;
		} else if(strcmp(word_list->pointer[i], ">") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			result->types[i] = file_out;
			result->locations[i] = i + 1;
		} else if(strcmp(word_list->pointer[i], "<") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			result->types[i] = file_in;
			result->locations[i] = i + 1;
		}
	}
}
