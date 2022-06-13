#ifndef __command_line_c__
#define __command_line_c__

/**
 * @file command_line.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "command_line.h"



const Command_t CMD[] = {

  {   /* command name */
      "push_back",
      /* command argument(s) */
      {"arg_name=", (char *)0},
      on_push_back,
      /* command description */ 
      "push_back will add element at back to list."
  },
  {   /* command name */
      "pop_back",
      /* command argument(s) */
      {"arg_name=", (char *)0},
      on_pop_back,
      /* command description */ 
      "pop_back will remove element at back from list."
  },
  {   /* command name */
      "attach_ue",
      /* command argument(s) */
      {"cell_id=", "5g-stmsi=","guami=", (char *)0},
      on_ue_attach,
      /* command description */ 
      "Attach UE to AMF(Application and Mobility Function)"
  },

  {   /* command name */
      "dettach_ue",
      /* command argument(s) */
      {"cell_id=", "5g-stmsi=","guami=", (char *)0},
      on_ue_detach,
      /* command description */ 
      "Dettach UE to AMF(Application and Mobility Function)"
  },

  {   /* command name */
      "send_ul_data",
      /* command argument(s) */
      {"cell_id=", "5g-stmsi=","guami=", (char *)0},
      on_ul_data,
      /* command description */ 
      "Pumping UL Data from UE to gNB"
  },

  {   /* command name */
      "send_dl_data",
      /* command argument(s) */
      {"cell_id=", "5g-stmsi=","guami=", (char *)0},
      on_dl_data,
      /* command description */ 
      "Pumping DL Data from UE to gNB"
  },

  {
      "send_rrc_reconfiguration" , 
      {"arg1=", "arg2=", (char *)0},
      on_rrc_reconfiguration,
      "Sending RRC Reconfiguration"
  },

  {
      "set_peer_config" , 
      {"ip=", "port=", (char *)0},
      on_peer_config,
      "Peer Server Configuration"
  },

  {
      "show_peer_config" , 
      {"ip=", "port=", (char *)0},
      show_peer_config,
      "Show Peer Configuration"
  },

  {
      "dump_ctx",
      {(char *)0},
      on_dummy,
      "Displays the contents of internal data structure CTX."
  },

  {
    "quit",
    {(char *)0},
    on_dummy,
    "Exiting from Command Prompt"
  },
  {
    "?",
    {(char *)0},
    on_dummy,
    "This is to get the help of supported command"

  },
  {
    "help",
    {(char *)0},
    on_dummy,
    "This is to get he help of supported command"
  },

  /*This mull be the last row.*/
  {
      (char *)0, 
      {(char *)0}, 
      (void *)0,
      (char *)0
  }

};

Context_t CTX = {0, 0, 
                    /* selected command name */(char *)0, 
                    /* selected command argument */{(char *)0}, 
                    false, 0};


Peer_t PEER = {"127.0.0.1", 7788, false, -1, {0}, {0}};

list_t *head_g = NULL;

/* registered callback for processing of command */

/**
 * @brief 
 * 
 * @param arg 
 * @return int 
 */
int on_ue_attach(const char *arg)
{
  fprintf(stderr, "The received Argument is %s\n", arg);

  return(0);
}

int on_ue_detach(const char *arg)
{
  fprintf(stderr, "The received Argument is %s\n", arg);

  return(0);
}

int on_ul_data(const char *arg)
{
  fprintf(stderr, "The received Argument is %s\n", arg);

  return(0);
}

int on_dl_data(const char *arg)
{
  fprintf(stderr, "The received Argument is %s\n", arg);

  return(0);
}

int on_rrc_reconfiguration(const char *arg)
{
    fprintf(stderr, "The received Argument is %s\n", arg);

    return(0);
}

int on_peer_config(const char *arg)
{
  char IP[16] = "127.0.0.1";
  unsigned int PORT = 7788;

  memset((void *)IP, 0, sizeof(IP));
  if(arg) {
    int cnt = sscanf(arg, "ip=%s port=%u",IP, &PORT);
    if(cnt > 1) {
  
      memcpy(PEER.IP, IP, strlen(IP));
      PEER.PORT = PORT;

    }
  }

  PEER.sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  PEER.peer_addr.sin_family = AF_INET;
  PEER.peer_addr.sin_port = htons(PEER.PORT);
  PEER.peer_addr.sin_addr.s_addr = htonl(inet_addr(PEER.IP));
  memset((void *)PEER.peer_addr.sin_zero, 0, sizeof(PEER.peer_addr.sin_zero));

  return(0);
}

int show_peer_config(const char *arg)
{
  fprintf(stderr, "IP PORT IS-CONNECTED SOCK-FD\n");
  fprintf(stderr, "%s %x %d %d\n", PEER.IP, PEER.PORT, PEER.is_connected, PEER.sockFd);

  return(0);
}

int on_dummy(const char *arg)
{
  fprintf(stderr, "The received Argument is %s\n", arg);

  return(0);
}

/**
 * @brief 
 * 
 */
void display_ctx(void)
{
  fprintf(stderr, "CommandOffset CommandArgOffset CommandName CommandArguments isContinue Len\n");
  fprintf(stderr, "%u %u %s %s %u %u\n",CTX.cmd_offset,CTX.cmd_arg_offset, CTX.cmd_name, CTX.cmd_args[0], CTX.is_continue, CTX.len);
}

/**
 * @brief 
 * 
 * @param arg_name 
 * @return true 
 * @return false 
 */
bool is_arg_already_selected(const char *arg_name)
{
  bool result = false;
  int offset;

  for(offset = 0; CTX.cmd_args[offset]; ++offset) {
    if(!strcmp(arg_name, CTX.cmd_args[offset])) {
      result = true;
      break;
    }
  }

  return(result);
}

/**
 * @brief 
 * 
 * @param req 
 * @param len 
 * @return int 
 */
int send_to_peer(const char *req, unsigned int len)
{
  if(PEER.sockFd < 0) {
  
    PEER.sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
  }

  if(!PEER.is_connected) {

    PEER.is_connected = connect(PEER.sockFd, (struct sockaddr *)&PEER.peer_addr, sizeof(struct sockaddr));

  }

  if(PEER.is_connected) {
    /* Connection is succeded - */

    int offset = 0;
    
    do {

      int sent_len = -1;
      sent_len = send(PEER.sockFd, (req+offset), (len - offset), 0);

      if(sent_len > 0) {
        offset += sent_len;
      }

    } while(offset != len);

  }
  return(0);
}

/**
 * @brief This function is called by realine until it gets (char *)0. For first time, value of state 
 *        will be zero and for subsequent call will br non-zero positive value.
 * 
 * @param text text to be completed.
 * @param state zero for first time and +ve value for subsequent call.
 * @return char* allocated memory will be freed by readline library.
 */
char *command_arg_list_generator(const char *text, int state)
{
  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {
    CTX.cmd_arg_offset = 0;
  }

  int *idx = &CTX.cmd_offset;
  int *sub_idx = &CTX.cmd_arg_offset;

  if(NULL == CMD[*idx].argv[*sub_idx])
    return (char *)0;

  /*memory will be freed by readline library.*/
  return(strdup(CMD[*idx].argv[(*sub_idx)++]));
}

/**
 * @brief This function is invoked by readline until it returns NULL. First time state will be zero 
 *        and for subsequent argument state will be non-zero.
 * 
 * @param text 
 * @param state 
 * @return char* 
 */
char *command_arg_generator(const char *text, int state)
{
  const char *name = NULL;
  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {
    CTX.cmd_arg_offset = 0;
  }

  int *idx = &CTX.cmd_offset;
  int *sub_idx = &CTX.cmd_arg_offset;

  /* Return the next name which partially matches from the command list. */
  while(NULL != (name = CMD[*idx].argv[*sub_idx])) {
    *sub_idx += 1;
    if(!strncmp (name, text, strlen(text))) {
      fprintf(stderr, "%s:%u arg name is %s", __FILE__, __LINE__, text);
      return(strdup(name));
    }
  }

  /*Reset to default either for next command or command argument(s) are exhausted.*/
  CTX.cmd_arg_offset = 0;

  /* If no names matched, then return NULL. */
  return((char *)0);
}


/* Generator function for command completion.  STATE lets us know whether
 * to start from scratch; without any state (i.e. STATE == 0), then we
 * start at the top of the list.
 */

/**
 * @brief 
 * 
 * @param text 
 * @param state 
 * @return char* 
 */
char *command_generator(const char *text, int state)
{
  const char *name;
  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {
    CTX.cmd_offset = 0;
    CTX.len = strlen(text);
  }

  int *idx = &CTX.cmd_offset;
  /* Return the next name which partially matches from the command list. */
  while((char *)0 != (name = CMD[*idx].cmd)) {
    *idx += 1;
    if(!strncmp (name, text, CTX.len)) {
      return(strdup(name));
    }
  }

  /*Reset to default either for next command or command argument(s).*/
  CTX.cmd_offset = 0;

  /* If no names matched, then return NULL. */
  return((char *)0);
}


/**
 * @brief Attempt to complete on the contents of TEXT.  START and END show the
 *        region of TEXT that contains the word to complete.  We can use the
 *        entire line in case we want to do some simple parsing.  Return the
 *        array of matches, or NULL if there aren't any.
 * 
 * @param text 
 * @param start 
 * @param end 
 * @return char** 
 */
char **command_completion(const char *text, int start, int end)
{
  char **matches;
  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if(start == 0) {
    rl_attempted_completion_over = 1;
    matches = rl_completion_matches(text, command_generator);
  }
  else {
    /*user has hit the space bar*/
    if(start == end) {
      int idx = 0;
      /*remember it into its context - this is the command whose argument(s) to be listed.*/
      //ReadlineIF::cmdName(rl_line_buffer);

      /*Return the entire arguments - list must have last element as NULL.*/
      for(idx = 0; CMD[idx].cmd; idx++) {
        if(!strncmp(rl_line_buffer, CMD[idx].cmd, strlen(CMD[idx].cmd))) {
          /*remember this offset and will be used while looping through command arguments.*/
          CTX.cmd_offset = idx;
          rl_attempted_completion_over = 1;
          matches = rl_completion_matches(text, command_arg_list_generator);
          /*break the while loop.*/
          break;
        }
      }
    }
    else {
      /*user has entered the initials of argument*/
      rl_attempted_completion_over = 1;
      matches = rl_completion_matches(text, command_arg_generator);
    }
  }

  return(matches);
}

/**
 * @brief 
 * 
 * @return int 
 */
int init(void)
{

  rl_attempted_completion_over = 1;
  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = command_completion;
  return(0);
}

bool continue_status(void)
{
  return(CTX.is_continue);
}


bool is_valid(char *cmd)
{
  int idx;
  bool result = false;
  do {
    for(idx = 0; CMD[idx].cmd; idx++) {
      if(cmd && !strcmp(cmd, CMD[idx].cmd)) {
        result = true;
        break;
      }
    }
  } while(0);

  return(result);
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
char *stripwhite (char *string)
{
  char *s, *t; 

  for (s = string; whitespace (*s); s++)
    ;   
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && whitespace (*t))
    t--;
  *++t = '\0';

  return s;
}

void help(char *cmd)
{
  int i;
  int printed = 0;

  for (i = 0; CMD[i].cmd; i++) {
    if(!*cmd || (!strcmp(cmd, CMD[i].cmd))) {
      printf("%s\t\t%s.\n", 
             CMD[i].cmd, 
             CMD[i].usage);
      printed++;
    }
  }

  if(!printed) {
    fprintf (stderr, "No commands match %s  Possibilties are:\n", cmd);

    for (i = 0; CMD[i].cmd; i++) {
      /* Print in six columns. */
      if (printed == 10) {
        printed = 0;
        printf ("\n");
      }

      printf ("%s\t", CMD[i].cmd);
      printed++;
    }

    if(printed)
      printf ("\n");
  }

}

void quit(void)
{
  CTX.is_continue = true;
}

int execute_line(char *req)
{
  int i;
  char *cmd = NULL;
  char *cmdArg = NULL;
  bool isFound = false;
  char *line = strdup(req);
  int status = 1;

  /* Isolate the command word. */
  i = 0;
  while(line[i] && whitespace(line[i]))
    i++;
  cmd = line + i;

  while(line[i] && !whitespace(line[i]))
    i++;

  if(line[i])
    cmd[i++] = '\0';

  cmdArg = line + i;
  (void)cmdArg;

  isFound = is_valid(cmd);

  do {
    if(!isFound) {
      fprintf(stderr, "%s:%u Invalid command\n", __FILE__, __LINE__);
      help(cmd);
      /*bypass the following statement.*/
      break;
    }
  
    if(!strncmp(cmd, "help", 4) || (!strncmp(cmd, "?", 1))) {
      help(cmd);
    }
    else if(!strncmp(cmd, "quit", 4)) {
      quit();
    }
    else {
      /* Call the function. */
      //process_command(origLine, strlen(origLine));
      status = 0;
    }

  } while(0);

  free(line);
  return(status);
}

int command_line_main(char *prompt)
{
  char *line = (char *)0;
  char *s = (char *)0;

  init();

  /* Loop reading and executing lines until the user quits. */
  for( ; !continue_status(); ) {

    /* readline allocates the memory for line and must be freed by application. */
    line = readline(prompt);

    if(!line)
      continue;

    /* Remove leading and trailing whitespace from the line.
       Then, if there is anything left, add it to the history list
       and execute it. */
    s = stripwhite(line);

    if(*s) {
      add_history(s);
      if(!execute_line(s)) {
        /*Send this command to hostapd via control interface.*/
        //if(-1 == transmit(s))
        {
          fprintf(stderr,"%s:%u The command is %s\n", __FILE__, __LINE__, s);
          execute_command(s);
        }
      }
    }
    
    /* freeying memory allocated by readline */
    free (line);
  }

  return(0);
}

/**
 * @brief 
 * 
 * @param req 
 * @return int 
 */
int execute_command(const char *req)
{
  if(!req) {
    /* the command is empty */
    return(0);
  }
  
  /* get the command */
  char cmd_name[256];
  int cnt = sscanf(req, "%s", cmd_name);

  if(EOF == cnt) {
    /* Extracting of command name failed */
    return(0);
  }

  int offset;
  for(offset = 0; CMD[offset].cmd; ++offset) {
    if(!strcmp(cmd_name, CMD[offset].cmd)) {
      /* invoke the registered callback +1 for to get rid of preceeding space*/
      CMD[offset].callback(req+1+strlen(cmd_name));
      break;
    }
  }

  return(0);
}

/**
 * @brief spawning worker forreceiving of response from peer.
 * 
 * @param arg 
 * @return true 
 * @return false 
 */
bool spawn_worker_to_receive(void *arg)
{
    pthread_t threadId = 0;
    int rc = 0;	 
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, (1024 * 1024));
    
    rc = pthread_create(&threadId, &attr, event_handler_loop, &arg);
    
    if(rc)
    {
	    return false;
    }

    pthread_detach(threadId);

    return true;
}

/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void* event_handler_loop(void *arg) 
{
  for(;;) {
    /* Receive from socket */
  }

  return(NULL);
}

int on_push_back(const char *arg) 
{
  char value[256];

  memset((void *)value, 0, sizeof(value));
  int cnt = sscanf(arg, "arg_name=%s", value);
  if(cnt > 0) {
    /*value is extracted successfully.*/
    list_t *node = (list_t *)malloc(sizeof(list_t));
    if(node != NULL) {
      strcpy(node->arg, value);
      node->offset = 0;
      push_back(&head_g, node);
    }
  }

  display(head_g);

  fprintf(stderr, "%s:%u Value of count %d value is %s\n", __FILE__, __LINE__, count(head_g), value);
  return(0);
}

int on_pop_back(const char *arg)
{
  char value[256];

  memset((void *)value, 0, sizeof(value));
  int cnt = sscanf(arg, "arg_name=%s", value);
  if(cnt > 0) {
    /*value is extracted successfully.*/
    pop_back(&head_g, value);
  }
  
  fprintf(stderr, "%s:%u Value of count %d value %s\n", __FILE__, __LINE__, count(head_g), value);
  return(0);
}

/**
 * @brief 
 * 
 * @param head 
 * @param node 
 */
void push_back(list_t **head, list_t *node)
{
  list_t *hh = *head;
  do {
    if(!hh) {
      hh = node;
      hh->next = NULL;
      *head = node;
      break;
    }

    for(; hh->next != NULL; hh = hh->next) ;

    hh->next = node;
    break;

  } while(0);
}

/**
 * @brief 
 * 
 * @param head 
 * @param arg_name 
 */
void pop_back(list_t **head, char *arg_name)
{
  list_t *hh = *head;
  do {

    if(hh && NULL == hh->next) {
      free(*head);
      *head = NULL;
      break;
    }

    list_t *previous = NULL;
    list_t *current = hh;
    for(; current && current->next != NULL;) {
      
      previous = current;
      current = current->next;
      
    }
    
    list_t *node = current;
    previous->next = current->next;
    free(node);

  } while(0);
}

/**
 * @brief 
 * 
 * @param arg_name 
 * @return true 
 * @return false 
 */
bool is_found(list_t *head, const char *arg_name)
{
  for(; head != NULL; head = head->next) {
    if(arg_name && !strncmp(head->arg, arg_name, strlen(arg_name))) 
      return true;
  }
  return false;
}

/**
 * @brief 
 * 
 * @param head 
 */
void erase(list_t **head)
{
  list_t *hh = *head;
  do {

    if(hh && NULL == hh->next) {
      free(*head);
      *head = NULL;
      break;
    }

    for(; hh != NULL;) {
      list_t *tmp = hh;
      hh = hh->next;
      free(tmp);
    }

  } while(0);
}

/**
 * @brief Get the arg count object
 * 
 * @param head 
 * @return int 
 */
int count(list_t *head)
{
  int cnt = 0;
  for(;head != NULL; head = head->next) {
    cnt++;
  }

  return(cnt);
}

void display(list_t *head)
{
  for(;head; head = head->next) {
    fprintf(stderr, "%s\n", head->arg);
  }
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) 
{
  command_line_main("Peace/>");

  return(0);
}

#endif /*__command_line_c__*/
