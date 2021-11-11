#include "../headers/proc.h"

typedef enum Type {
	program,
	file_in,
	file_out,
} Type;

typedef struct Command_list {
	int num;
	int *locations;
	Type *types;
}

/* void proc(Word_list * word_list, Command_list * result) {
	int command_count = 1;
	result->types = (Type*) malloc(command_count * sizeof(Type));
	result->locations = (int*) malloc(command_count * sizeof(int));
	result->locations[0] = 1;
	result->types[0] = program;
	for(int i = 0; i < word_list->size; ++i) {
	    if(word_list->pointer[i] == "|" && i != 0) {
	        word_list->pointer[i] = NULL;
	        command_count += 1;
	        result->types = (Type*) realloc(result->types, command_count * sizeof(Type));
	        result->types[command_count - 1] = program;
	        result->locations = (int*) realloc(result->locations, command_count * sizeof(int));
	        result->locations[command_count - 1] = i;
	    } else if(word_list->pointer[i] == ">" && i != 0) {
	        word_list->pointer[i] = NULL;
	        command_count += 1;
	        result->types = (Type*) realloc(result->types, command_count * sizeof(Type));
	        result->types[command_count - 1] = file_out;
	        result->locations = (int*) realloc(result->locations, command_count * sizeof(int));
	        result->locations[command_count - 1] = i;
	    } else if(word_list->pointer[i] == "<" && i != 0) {
	        word_list->pointer[i] = NULL;
	        command_count += 1;
	        result->types = (Type*) realloc(result->types, command_count * sizeof(Type));
	        result->types[command_count - 1] = file_in;
	        result->locations = (int*) realloc(result->locations, command_count * sizeof(int));
	        result->locations[command_count - 1] = i;
	    }
	}
	result->num = command_count;	
} */

void proc(Word_list *word_list, Command_list *result) {
	result->num = 1;
	for(int i = 0; i < word_list->num; ++i) {
		switch(word_list->pointer + i) {
			case "|":
				++(result->num);
				break;
			case ">":
				++(result->num);
				break;
			case "<":
				++(result->num);
				break;
		}
	}
	result->types = (Type*)malloc(result->num * sizeof(Type));
	result->locations = (int*)malloc(result->num * sizeof(int));
	result->types[0] = program;
	result->locations[0] = 0;
	int current_command = 1; //indx
	for(int i = 1; i < word_list->num; ++i) {
		switch(word_list->pointer + i) {
			case "|":
				free(word_list->pointer + i);
				(word_list->pointer + i) = NULL;
				result->types[i] = program;
				result->locations[i] = i + 1;
				break;
			case ">":
				free(word_list->pointer + i);
				(word_list->pointer + i) = NULL;
				result->types[i] = file_out;
				result->locations[i] = i + 1;
				break;
			case "<":
				free(word_list->pointer + i);
				(word_list->pointer + i) = NULL;
				result->types[i] = file_in;
				result->locations[i] = i + 1;
			default:
				break;
		}
	}
}