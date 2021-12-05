#ifndef VAR_H_INCLUDED
#define VAR_H_INCLUDED

//struct for storing a list element
typedef struct VarListElement {
	char *name;
	char *value;
	struct VarListElement *kov;
} VarListElement;

//list struct
typedef struct VarList {
	VarListElement *begin, *end;
} VarList;


void add_var(VarList * varlist, char * name, char * value);

void free_varlist(VarList * varlist);

char * value_search(VarList * varlist, char * name);

#endif