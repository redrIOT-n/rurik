#ifndef _XML_PARSE_
#define _XML_PARSE_

/*############### includes #############*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* ############# typedefs ############ */
struct xmlnode{

  char *tag;
  char *text;
  struct xmlnode *parent;
}; 

typedef struct xmlnode XMLNode;

typedef struct {
  
  char *file;
  XMLNode *root;
} XMLDoc; 

bool XMLDocRead(XMLDoc *doc, char *path);
bool XMLDocFree(XMLDoc *doc);
XMLNode *XMLNewNode(XMLNode *parent);
void XMLNodeFree(XMLNode *node);

#endif
