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
  struct xmlnode **child_node;
  int no_of_children;
}; 

typedef struct xmlnode XMLNode;

struct xmldoc{
  
  char *file;
  XMLNode *root;
}; 

typedef struct xmldoc XMLDoc;

bool XMLDocRead(XMLDoc *doc, char *path);
void XMLDocFree(XMLDoc *doc);
XMLNode *XMLNewNode(XMLNode *parent);
void XMLNodeFree(XMLNode *node);
void XMLGetTextByTag(XMLNode *root, char *tag, char *textbuf);
void XMLGetNodeByTag(XMLNode *root, char *tag, XMLNode *c_node);
char **XMLGetChildTextByNode(XMLNode *root);

#endif
