#include "attr.h"

CLangSyntaxScheme *parse_c_syntax(xmlDocPtr doc, xmlNodeptr cur){

  CLangSyntaxScheme c_scheme;
  
  c_scheme.colors = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
  c_scheme.font_type = {"normal", "standout", "reverse", "blink", "dim", "bold", "invisible", "altcharset", "chartext"};
 
  xmlChar *color, font_type;
  cur = cur->xmlChildrenNode;

  start_color();

    //xml parsing
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
      for (int j = 0; j < 10; j++){
      
        switch (j)
      }
    }
  }
}
    
