#include "attr.h"

CLangSyntaxScheme *parse_c_syntax(xmlDocPtr doc, xmlNodeptr cur){

  CLangSyntaxScheme c_scheme;
  
  c_scheme.colors = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
  c_scheme.font_type = {"normal", "standout", "reverse", "blink", "dim", "bold", "invisible", "altcharset", "chartext"};
 
  xmlChar *color, font_type;
  cur = cur->xmlChildrenNode;

  start_color();
  
  if((!xmlStrcmp(cur->name, (const xmlChar *) "C"))){
    
    if((!xmlStrcmp(cur->xmlChildrenNode->name, (const xmlChar *) "comments"))){
      
      if((!xmlStrcmp(cur->xmlChildrenNode->xmlChildrenNode->name, (const xmlChar *) "color")))
        color = xmlNodeListGetString(doc, cur->xmlChildrenNode->xmlChildrenNode->xmlChildrenNode, 1);
                
      if((!xmlStrcmp(cur->xmlChildrenNode, (const xmlChar *) "font_type")))
        font_type = xmlNodeListGetString(doc, cur->xmlChildrenNode->xmlChildrenNode, 1);

      for(int i = 0; i < 8; i++){
        
        if(strcmp(c_scheme.colors[i], color)){
        
          switch (i){

          case COLOR_BLACK:
            init_color(1, COLOR_BLACK, COLOR_BLACK);
            break;

          case COLOR_RED:
            init_color(1, COLOR_RED, COLOR_BLACK;
            break;

          case COLOR_GREEN:
            init_color(1, COLOR_GREEN, COLOR_BLACK);
            break;

          case COLOR_YELLOW:
            init_color(1, COLOR_YELLOW, COLOR_BLACK);
            break;

          case COLOR_BLUE:
            init_color(1, COLOR_BLUE, COLOR_BLACK);
            break;

          case COLOR_MAGENTA:
            init_color(1, COLOR_MAGENTA, COLOR_BLACK);
            break;

          case COLOR_CYAN:
            init_color(1, COLOR_CYAN, COLOR_BLACK);
            break;

          case COLOR_WHITE:
            init_color(1, COLOR_WHITE, COLOR_BLACK);
            break;

          default:
            break;
          }
          for(int j = 0; j < 10; j++){
           
            if(!(strcmp(c_scheme.attributes[j], font_type)){

              switch (j) {

              case 
              }  
            }  
          }
        }
      }
    } 
  }
}

static int parse_doc(char *docname, char *lang){

  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile(docname);
  if(doc == NULL) return -1;

  cur = xmlDocGetRootElement(doc);
  if(cur == NULL) {

    xmlFreeDoc(doc);
    return -1;
  }
  if(xmlStrcmp(cur->name, (const xmlChar *) "scheme")){
  
    xmlFreeDoc(doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;
  while (cur != NULL){

    if(!xmlStrcmp(cur->name, (const xmlChar *) lang)){

      if(!(strcmp(lang, "c"))
        parse_c_syntax(doc, cur);
    }
    cur = cur->xmlChildrenNode;
  }
}    
