#ifndef ATTR_H
#define ATTR_H

#include "main.h"
#include "ncurses.h"
#include "xmlparse.h"

//BLACK   0
//RED     1
//GREEN   2
//YELLOW  3
//BLUE    4
//MAGENTA 5
//CYAN    6
//WHITE   7

//NORMAL    0
//STANDOUT  1
//UNDERLINE 2
//REVERSE   3
//BLINK     4
//DIM       5
//BOLD      6
//INVISIBLE 7
//ITALIC    8

/* ############## default highlighting ############### */

//c language

typedef struct{
  
  attr_t statements[5];
    //comments              = 0
    //condtional_statements = 1
    //loops;                = 2
    //structures;           = 3
    //returns               = 4
  char **colors;
  char **font_type;
} CLangSyntaxScheme;

/* ################ color schemes for other langs ############### */

CLangSyntaxScheme *parse_c_syntax(void);
int check_syntax(char *buffer, int current_loc, int max_y, int max_x);

#endif
