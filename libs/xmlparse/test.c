#include "xmlparse.h"

int main(void){

  //fprintf(stdout, "jelloo\n");
  XMLDoc doc;
  XMLDocRead(&doc, "test/test.xml");
  //fprintf(stdout, "bla\n");
  printf("%s\t %s\n", doc.root->tag, doc.root->text);

  XMLDocFree(&doc);

  return 0;
}
