#include "xmlparse.h"

int main(void){

  printf("hutt");
  XMLDoc doc;
  XMLDocRead(&doc, "test/test.xml");
  printf("blaa");
  printf("%d\n", doc.root->no_of_children);

  XMLNode *current_node = malloc(sizeof(XMLNode));
  XMLGetNodeByTag(doc.root, "scheme", current_node);
  printf("%s\n", current_node->tag);


  char **child_array = XMLGetChildTextByNode(current_node);
  
  for (int i = 0; i < 2; i++)
    printf("%s\n", child_array[i]);

  XMLDocFree(&doc);

  return 0;
}
