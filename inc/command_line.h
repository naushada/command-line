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
	char *cmd;
	char *argv[16];
	char *usage;  
}Command_t;

typedef struct {
	int cmd_offset;
	int cmd_arg_offset;
	char* cmd_name;
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
execute_command(int cnt, ...);






#endif /* __command_line_h__ */
