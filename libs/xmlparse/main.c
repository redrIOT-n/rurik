#include "xmlparse.h"

int main(void){

  XMLDoc doc;
  XMLDocRead(&doc, "/home/redriot/projects/test/scr/colorscheme/scheme.xml");
//  printf("%s\t %s\t%d\n", doc.root->tag, doc.root->child_node[0]->text, doc.root->no_of_children);


  XMLNode *current_node = malloc(sizeof(XMLNode));
  XMLGetNodeByTag(doc.root, "comments", current_node);
  printf("%s\n", current_node->tag);


  char **child_array = XMLGetChildTextByNode(current_node);
  
  for (int i = 0; i < 2; i++)
    printf("%s\n", child_array[i]);

  XMLDocFree(&doc);

  return 0;
}
