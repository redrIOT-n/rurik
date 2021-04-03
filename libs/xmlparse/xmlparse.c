#include "xmlparse.h"

bool XMLDocRead(XMLDoc *doc, char *path){

  FILE *fh = fopen(path, "r");
  if(fh == NULL){
    
      return false;
  }

  fseek(fh, 0, SEEK_END);
  int size = ftell(fh);
  fseek(fh, 0, SEEK_SET);

  //doc = malloc(sizeof(XMLDoc));
  //if(doc == NULL) return false;

  doc->file = malloc(sizeof(char) * size + 1);
  if(doc->file == NULL) return false;

  fread(doc->file, sizeof(char), size, fh);
  doc->file[size] = '\0';
  fclose(fh);

  doc->root = XMLNewNode(NULL);
  if(doc->root == NULL) return false;

  char buffer[256];
  int buffer_i = 0;
  int i = 0;

  XMLNode *current_node = NULL;

  while(doc->file[i] != '\0'){
  
    if(doc->file[i] == '<'){

      buffer[buffer_i] = '\0';
      if(buffer_i > 0){
      
        if(!current_node) return false;
        else{
          current_node->text = strdup(buffer);
          buffer_i = 0;
        }
      } 
    
      if(doc->file[i+1] == '/'){

        i+=2;
        while(doc->file[i] != '>') buffer[buffer_i++] = doc->file[i++];
        buffer[buffer_i] = '\0';
        buffer_i = 0;

        if(!current_node) return false;

        if(strcmp(current_node->tag, buffer) != 0) return false;

        current_node = current_node->parent;
        i++;
        continue;
      }
      if(current_node == NULL) current_node = doc->root;
      else current_node = XMLNewNode(current_node);
      if(current_node == NULL) return NULL;

      i++;
      while(doc->file[i] != '>') buffer[buffer_i++] = doc->file[i++];
      buffer[buffer_i] = '\0';
      current_node->tag = strdup(buffer);
      buffer_i = 0;
      i++;
      continue;
    }
    else{
    
      buffer[buffer_i++] = doc->file[i++];
    }
  }
  return true;
}

XMLNode *XMLNewNode(XMLNode *parent){

  XMLNode *node = malloc(sizeof(XMLNode));
  if(node == NULL) return NULL;

  node->parent = parent;
  node->tag = NULL;
  node->text = NULL;

  return node;
}

void XMLNodeFree(XMLNode *node){

  if(node->tag)
    free(node->tag);
  
  if(node->text)
    free(node->text);

  free(node);;
}

bool XMLDocFree(XMLDoc *doc){

  free(doc->file);
  free(doc->root);
}
