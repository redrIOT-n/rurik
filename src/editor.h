#ifndef EDITOR_
#define EDITOR_

#include "file_handle.h"
#include "main.h"
#include "attr.h" 
#include <math.h>

void clear_editor(int max_y, int max_X);
void print_screen(char *buffer, CurrentFile *f);
void listen_editor(char *buffer, int max_scolls, int max_y, int max_x, int current_loc);
//void show_editor(void);

#endif
