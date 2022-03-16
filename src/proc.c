#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "include/proc.h"
#include "include/split.h"
#include "include/input.h"
#include "include/macros.h"

void proc(Word_list * word_list, Command_list * result) {
	if(word_list) {
		result->num = 1;
	}
	for(int i = 0; word_list->pointer[i] != NULL; ++i) { // This for loop counts how many commands there are
		if(strcmp(word_list->pointer[i], "|") == 0
		|| strcmp(word_list->pointer[i], ">") == 0
		|| strcmp(word_list->pointer[i], "<") == 0
		|| strcmp(word_list->pointer[i], ">>") == 0) {
			++(result->num);
		}
	}
	result->types = (Type*)malloc((result->num + 1) * sizeof(Type));
	result->locations = (int*)malloc((result->num + 1) * sizeof(int));
	result->types[0] = program;
	result->locations[0] = 0;
	int prog_count = 0;
	for(int i = 1; i < word_list->num; ++i) {
		if(strcmp(word_list->pointer[i], "|") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			++prog_count;
			result->types[prog_count] = program;
			result->locations[prog_count] = i + 1;
		} else if(strcmp(word_list->pointer[i], ">") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			++prog_count;
			result->types[prog_count] = file_out;
			result->locations[prog_count] = i + 1;
		} else if(strcmp(word_list->pointer[i], "<") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			++prog_count;
			result->types[prog_count] = file_in;
			result->locations[prog_count] = i + 1;
		} else if(strcmp(word_list->pointer[i], ">>") == 0) {
			free(word_list->pointer[i]);
			word_list->pointer[i] = NULL;
			++prog_count;
			result->types[prog_count] = file_append;
			result->locations[prog_count] = i + 1;
		}
	}
}
