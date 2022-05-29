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
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

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

typedef struct {
	char IP[16];
	unsigned int PORT;
	bool is_connected;
	int sockFd;
	struct sockaddr_in peer_addr;
	struct sockaddr_in self_addr;
}Peer_t;

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

int on_ue_attach(const char *arg);
int on_ue_detach(const char *arg);
int on_rrc_reconfiguration(const char *arg);
int on_dummy(const char *arg);
int on_ul_data(const char *arg);
int on_dl_data(const char *arg);
int on_peer_config(const char *arg);
int show_peer_config(const char *arg);
bool spawn_worker_to_receive(void *arg);
void* event_handler_loop(void *arg);

#endif /* __command_line_h__ */
