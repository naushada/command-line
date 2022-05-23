#ifndef __command_line_h__
#define __command_line_h__

#include<stdarg.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum {false, true} bool;

typedef struct {
	const char *cmd;
	const char *argv[255];
	int (*callback)(const char *cmd_argument);
	const char *usage;
}Command_t;

typedef struct {
	int cmd_offset;
	int cmd_arg_offset;
	/* selected command */
	char *cmd_name;
	/* selected arguments */
	char *cmd_args[255];
	bool is_continue;
	int len;
}Context_t;

char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);
char *command_arg_generator(const char *text, int state);
char *command_arg_list_generator(const char *text, int state);

int command_line_main(char *prompt);
int execute_line(char *req);
void help(char *cmd);
char *stripwhite(char *string);
bool is_valid(char *cmd);
int init(void);
int execute_command(const char *req);
bool is_arg_already_selected(const char *arg_name);
void display_ctx(void);

/* callback function prototype */

int handle_attach_ue(const char *arg);



#endif /* __command_line_h__ */
