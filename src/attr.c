#include "attr.h"

CLangSyntaxScheme *parse_c_syntax(void){

  CLangSyntaxScheme *c_scheme = malloc(sizeof(CLangSyntaxScheme));
  
  char *colors[8] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
  c_scheme->colors = colors;
  char *font_type[9] = {"normal", "standout", "underline", "reverse", "blink", "dim", "bold", "invisible", "italic"};
  c_scheme->font_type = font_type;

  //start_color();
  
  XMLDoc doc;
  XMLDocRead(&doc, "colorscheme/scheme.xml");
  fprintf(stdout, "%s\t %s\n", doc.root->text, doc.root->tag);

  XMLNode *current_node = malloc(sizeof(XMLNode));
  XMLGetNodeByTag(doc.root, "c", current_node);

  for (int i = 0; i < current_node->no_of_children; i++){

    char **list = XMLGetChildTextByNode(current_node->child_node[i]);
    printf("%s\t %s\n", list[0], list[1]);
    for(int j = 0; j < 8; j++){
        
      if(!(strcmp(c_scheme->colors[j], list[0]))){
        
        switch (j){

        case COLOR_BLACK:
          init_pair(1, COLOR_BLACK, COLOR_BLACK);
          break;

        case COLOR_RED:
          init_pair(1, COLOR_RED, COLOR_BLACK);
          break;

        case COLOR_GREEN:
          init_pair(1, COLOR_GREEN, COLOR_BLACK);
          break;

        case COLOR_YELLOW:
          init_pair(1, COLOR_YELLOW, COLOR_BLACK);
          break;

        case COLOR_BLUE:
          init_pair(1, COLOR_BLUE, COLOR_BLACK);
          break;

        case COLOR_MAGENTA:
          init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
          break;

        case COLOR_CYAN:
          init_pair(1, COLOR_CYAN, COLOR_BLACK);
          break;

        case COLOR_WHITE:
          init_pair(1, COLOR_WHITE, COLOR_BLACK);
          break;

        default:
          break;
        }
        for (int k = 0; k < 10; k++){
      
          if(!(strcmp(c_scheme->font_type[k], list[1]))){
            switch (k){
          
            case 0:
              c_scheme->statements[i] = A_NORMAL | COLOR_PAIR(1);
              break;

            case 1:
              c_scheme->statements[i] = A_STANDOUT | COLOR_PAIR(1);
              break;

            case 2:
              c_scheme->statements[i] = A_UNDERLINE | COLOR_PAIR(1);
              break;

            case 3:
              c_scheme->statements[i] = A_REVERSE | COLOR_PAIR(1);  
              break;

            case 4:
              c_scheme->statements[i] = A_BLINK | COLOR_PAIR(1);
              break;

            case 5:
              c_scheme->statements[i] = A_DIM | COLOR_PAIR(1);
              break;

            case 6:
              c_scheme->statements[i] = A_BOLD | COLOR_PAIR(1);
              break;

            case 7:
              c_scheme->statements[i] = A_INVIS | COLOR_PAIR(1);
              break;

            case 8:
              c_scheme->statements[i] = A_ITALIC | COLOR_PAIR(1);
              break;

            default:
              break;
            }
          }
        }
      }
    }
  }
  XMLDocFree(&doc);
  return c_scheme;
}

void check_syntax(char *buffer){

  int prevc = 0;
  CLangSyntaxScheme *scheme = parse_c_syntax();

  for (int i = 0; i < strlen(buffer); i++){
  
    switch (buffer[i]){
    
    case '/':
      if(buffer[i+1] == '*') attron(scheme->statements[0]);
      break;

    case '*':
      if(buffer[i+1] == '/') attroff(scheme->statements[0]);
      break;

    case 'i':
      if(buffer[i+1] == 'f') {
        
        attron(scheme->statements[1]);
        waddch(win.text_editor, buffer[i]);
        waddch(win.text_editor, buffer[i++]);
        attroff(scheme->statements[1]);
        continue; 
      }
      break;
 
    case 's':

      if((buffer[i+1] == 'w') && (buffer[i+2] == 'i') && (buffer[i+3] == 't') && (buffer[i+4] == 'c') &&  (buffer[i+5] == 'h')){
      
        attron(scheme->statements[1]);
        for (int j = 0; j <= 5; j++){
        
          waddch(win.text_editor, buffer[i+j]);
        }
        attroff(scheme->statements[1]);
        continue;
      }

      else if((buffer[i+1] == 't') && (buffer[i+2] == 'r') && (buffer[i+3] == 'u') && (buffer[i+4] == 'c') && (buffer[i+5] == 't')){
      
        attron(scheme->statements[3]);
        for (int j = 0; j <= 5; j++){
        
          waddch(win.text_editor, buffer[i+j]);
        }
        attroff(scheme->statements[3]);
        continue;
      }
      break;


    case 'f':
      if(buffer[i+1] == 'o' && buffer[i+2] == 'r') {
      
        attron(scheme->statements[2]);
        for (int j = 0; j <= 2; j++){
         
          waddch(win.text-editor, buffer[i]);
        }
        attroff(scheme->statements[2]);
        continue;
      }
      break;

    case 'w':
      if((buffer[i+1] == 'h') && (buffer[i+2] == 'i') && (buffer[i+3] == 'l') && (buffer[i+4] == 'e')){
  
        attron(scheme->statements[2]);
        for(int j = 0; j <= 4; j++){
        
          waddch(win.text_editor, buffer[i+j]);
        }
        attroff(scheme->statements[2]);
        continue;
      }
      break;

    case 't':
      if((buffer[i+1] == 'y') && (buffer[i+2] == 'p') && (buffer[i+3] == 'e') && (buffer[i+4] == 'd') && (buffer[i+5] == 'e') && (buffer[i+6] == 'f')){
      
        attron(scheme->statements[3]);
        for (int j = 0; j <= 6; j++){
        
          waddch(win.text_editor, buffer[i+j]);
        }
        attroff(scheme->statements[3]);
        continue;
      }
      break;
    case 'r':
      if((buffer[i+1] == 'e') && (buffer[i+2] == 't') && (buffer[i+3] == 'u') && (buffer[i+4] == 'r') && (buffer[i+5] == 'n')){
      
        attron(scheme->statements[4]);
        for (int j = 0; j <= 5; j++){
        
          waddch(win.text_editor, buffer[i+j]);
        }
        attroff(scheme->statements[4]);
        continue;
      }
      break;
    }
  }
}
