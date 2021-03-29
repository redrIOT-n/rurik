#ifndef ATTR
#define ATTR

#include <libxml2/parser.h>
#include <libxml2/xmlmemory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

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
  char *colors[8];
  char *font_type[10];
} CLangSyntaxScheme;

/* ################ color schemes for other langs ############### */


#endif
