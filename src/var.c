#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../include/var.h"
#include "../include/split.h"
#include "../include/macros.h"

//functio for adding variable to either shell variable list, or alias list
void add_var(VarList * varlist, char * name, char * value) {
	if(value_search(varlist, name) == NULL) {
		VarListElement * new_element;
		new_element = (VarListElement*) malloc(sizeof(VarListElement));
		new_element->name = (char*) malloc(strlen(name) + 1);
		strcpy(new_element->name, name);
		new_element->value = (char*) malloc(strlen(value) + 1);
		strcpy(new_element->value, value);
		new_element->kov = NULL;
		if(varlist->begin == NULL) {
			varlist->begin = new_element;
			varlist->end = new_element;
		} else {
			varlist->end->kov = new_element;
			varlist->end = new_element;
		}
	} else {
		VarListElement * iter = varlist->begin;
		while(iter->name != name && iter->kov != NULL) {
			iter = iter->kov;
		}
		free(iter->value);
		iter->value = (char*) malloc(strlen(value) + 1);
		strcpy(iter->value, value);
	}
}

//function to free up linked list
void free_varlist(VarList * varlist) {
	if(varlist->begin != NULL) {
		VarListElement * iter = varlist->begin;
		while(iter->kov != NULL) {
			VarListElement * temp = iter;
			iter = iter->kov;
			free(temp->name);
			free(temp->value);
			free(temp);
		}
	}
}

//function for finding value of a shell variable or alias
char * value_search(VarList * varlist, char * name) {
	if(varlist->begin == NULL) {
		return NULL;
	}
	VarListElement * iter = varlist->begin;
	while(1) {
		if(strcmp(iter->name, name) == 0) {
			return iter->value;
		}
		if(iter->kov == NULL) {
			break;
		} else {
			iter = iter->kov;
		}
	}
	return NULL;
}
