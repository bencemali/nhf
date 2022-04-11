#ifndef MACROS_INCLUDED
#define MACROS_INCLUDED

#define EXIT_WITH_QUIT 22 //returned if exit or quit was inputted
#define REPROMPT_RETURN_VALUE 0 //returned if process has to display a new prompt
#define NO_PROMPT_RETURN_VALUE 33 //returned if the process doesn't have to display the next prompt
#define NO_COMMAND_WITH_TYPE 0 //returned if there isn't a command looked for
#define SOMETHING_WRONG 999 //returned if something unexpectedly went wrong
#define EXECUTE_ERR 420 //returned if couldn't execute program
#define CD_RETURN 343 //returned if input is 'cd' and child process doesn't need to do anything
#define HISTORY_RETURN 555 //will be used later
#define OK_RETURN 777 //standard return value

#endif

