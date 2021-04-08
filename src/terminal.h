#ifndef _TERMINAL_
#define _TERMINAL_

#include "main.h"

void changedir(char *dirname);
int start_process(char **string);
char *read_buffer(void);
char **parse(char *str);
void run_shell(void);
//void show_terminal(void);

#endif
