#include "xmlparse.h"

int main(void){

  XMLDoc doc;
  XMLDocRead(&doc, "test/test.xml");
  printf("%s\t %s\t%d\n", doc.root->tag, doc.root->child_node[0]->text, doc.root->no_of_children);

  char buf[256];
  XMLGetTextByTag(doc.root, "conditionals", buf);
  printf("%s\n", buf);

  XMLNode *current_node = malloc(sizeof(XMLNode));
  XMLGetNodeByTag(doc.root, "scheme", current_node);
  printf("%s\n", current_node->text);


  char **child_array = XMLGetChildTextByNode(current_node);
  
  for (int i = 0; i < 3; i++)
    printf("%s\n", child_array[i]);

  XMLDocFree(&doc);

  return 0;
}
