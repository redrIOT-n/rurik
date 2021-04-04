#include "attr.h"

CLangSyntaxScheme *parse_c_syntax(void){

  CLangSyntaxScheme c_scheme;
  
  char *colors[8] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
  c_scheme.colors = colors;
  char *font_type[10] = {"normal", "standout", "reverse", "blink", "dim", "bold", "invisible", "altcharset", "chartext"};
  c_scheme.font_type = font_type;

  //start_color();

  //xml parse
  
  XMLDoc doc;
  char *path = "colorscheme/scheme.xml";

  if(!XMLDocRead(&doc, path)) {

    //on_i_error("failed");
    return NULL;
  }

  XMLNode *current_node = malloc(sizeof(sizeof(XMLNode)));
  XMLGetNodeByTag(doc.root, "c", current_node);

  for (int i = 0; i < current_node->no_of_children; i++){

    char **list = XMLGetChildTextByNode(current_node->child_node[i]);
    printf("%s\t %s\n", list[0], list[1]);
    /*for(int j = 0; j < 8; j++){
        
      if(strcmp(c_scheme.colors[j], list[0])){
        
        switch (j){

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
        for (int k = 0; k < 10; k++){
      
          switch (k)
        }
      }
    } */
  }
  XMLDocFree(&doc);

}

int main(void){

  parse_c_syntax();
}
