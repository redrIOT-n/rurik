#ifndef _ATTR_
#define _ATTR_

#include <ncurses.h>
//#include "main.h"
#include "xmlparse.h"

#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7

/* ############## default highlighting ############### */

//c language

typedef struct{

  attr_t comments;
  attr_t condtional_statements;
  attr_t loops;
  attr_t structures;
  attr_t returns;
  char **colors;
  char **font_type;
} CLangSyntaxScheme;

/* ################ color schemes for other langs ############### */

CLangSyntaxScheme *parse_c_syntax(void);


#endif
