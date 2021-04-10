#include "attr.h"

CLangSyntaxScheme *parse_c_syntax(void){

  CLangSyntaxScheme *c_scheme = malloc(sizeof(CLangSyntaxScheme));
  if(c_scheme == NULL) return NULL;

  char *colors[8] = {"black\0", "red\0", "green\0", "yellow\0", "blue\0", "magenta\0", "cyan\0", "white\0"};
  c_scheme->colors = colors;
  char *font_type[9] = {"normal\0", "standout\0", "underline\0", "reverse\0", "blink\0", "dim\0", "bold\0", "invisible\0", "italic\0"};
  c_scheme->font_type = font_type;

  const char *home;
  if((home = getenv("HOME")) == NULL) home = getpwuid(getuid())->pw_dir;

  char *path = malloc(strlen(home) + strlen("/colorscheme/scheme.xml\0"));
  if(path == NULL) return NULL;

  memcpy(path, home, strlen(home));
  strcat(path, "/colorscheme/scheme.xml\0");

  XMLDoc doc;
  if(!XMLDocRead(&doc, path)){
  
    free(path);
    on_i_error("'$HOME/colorscheme/scheme.xml' file not found");
    return NULL;
  }
  free(path);
  XMLNode *current_node = malloc(sizeof(XMLNode));
  if(current_node == NULL) return NULL;
  XMLGetNodeByTag(doc.root, "c", current_node);

  start_color();

  for (int i = 0; i < current_node->no_of_children; i++){

    char **list = XMLGetChildTextByNode(current_node->child_node[i]);
    for(int j = 0; j < 8; j++){
        
      if(!(strcmp(c_scheme->colors[j], list[0]))){
       
        switch (j){

        case 0:
          init_pair(1, COLOR_BLACK, COLOR_BLACK);
          break;

        case 1:
          init_pair(1, COLOR_RED, COLOR_BLACK);
          break;

        case 2:
          init_pair(1, COLOR_GREEN, COLOR_BLACK);
          break;

        case 3:
          init_pair(1, COLOR_YELLOW, COLOR_BLACK);
          break;

        case 4:
          init_pair(1, COLOR_BLUE, COLOR_BLACK);
          break;

        case 5:
          init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
          break;

        case 6:
          init_pair(1, COLOR_CYAN, COLOR_BLACK);
          break;

        case 7:
          init_pair(1, COLOR_WHITE, COLOR_BLACK);
          break;
        }
        for (int k = 0; k < 9; k++){
      
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
          else continue;
        }
      }
      else continue;
    }
  }
  XMLDocFree(&doc);
  return c_scheme;
}

int check_syntax(char *buffer, int current_loc, int max_y, int max_x){

  raw();

  wmove(win.text_editor, 0, 0);
  CLangSyntaxScheme *scheme = parse_c_syntax();
  if(scheme == NULL) return -1;

  int lines = 0;
  int i, j;
  for (i = 0; lines < max_y; i++){
   
    if(strlen(buffer) == current_loc + i) break;

    int y, x;
    getyx(win.text_editor, y, x);
    if(x == max_x) {
    
      lines++;
      if(lines == max_y) continue;
      wmove(win.text_editor, y+1, 0);
    }

    switch (buffer[current_loc + i]){
    
    case '\n':
      lines++;
      if(lines < max_y) {
        waddch(win.text_editor, buffer[current_loc + i +j]);
        wmove(win.text_editor, y+1, 0);  
      }
      break;

    case '/':
      if(buffer[current_loc + i+1] == '*') {
        wattron(win.text_editor, scheme->statements[0]);
        waddch(win.text_editor, buffer[current_loc + i]);
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;

    case '*':
      if(buffer[current_loc + i+1] == '/') {
        wattroff(win.text_editor, scheme->statements[0]);
        waddch(win.text_editor, buffer[current_loc + i]);
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;
    
    case 'i':
      if(buffer[current_loc + i+1] == 'f') {
        
        wattron(win.text_editor, scheme->statements[1]);
        for (j = 0; j <= 1; j++) {
          
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x) {
            lines++;
            if(lines == max_y) continue;
            wmove(win.text_editor, y+1, 0);
          
          }waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[1]);
        continue; 
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;
 
    case 's':

      if((buffer[current_loc + i+1] == 'w') && (buffer[current_loc + i+2] == 'i') && (buffer[current_loc + i+3] == 't') && (buffer[current_loc + i+4] == 'c') &&  (buffer[current_loc + i+5] == 'h')){
      
        wattron(win.text_editor, scheme->statements[1]);
        for (j = 0; j <= 5; j++){
        
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x){          
            lines++;
            if(lines == max_y) continue;
            wmove(win.text_editor, y+1, 0);
          }
          waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[1]);
        continue;
      }

      else if((buffer[current_loc + i+1] == 't') && (buffer[current_loc + i+2] == 'r') && (buffer[current_loc + i+3] == 'u') && (buffer[current_loc + i+4] == 'c') && (buffer[current_loc + i+5] == 't')){
      
        wattron(win.text_editor, scheme->statements[3]);
        for (j = 0; j <= 5; j++){
        
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x) {
            lines++;
            if(lines == max_y) continue;
            wmove(win.text_editor, y+1, 0);
          }
          waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[3]);
        continue;
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;

    case 'f':
      if(buffer[current_loc + i+1] == 'o' && buffer[current_loc + i+2] == 'r') {
      
        wattron(win.text_editor, scheme->statements[2]);
        for (j = 0; j <= 2; j++){
         
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x) {
            lines++;
            if(lines == max_y) continue;
            wmove(win.text_editor, y+1, 0);
          }
          waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[2]);
        continue;
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;

    case 'w':
      if((buffer[current_loc + i+1] == 'h') && (buffer[current_loc + i+2] == 'i') && (buffer[current_loc + i+3] == 'l') && (buffer[current_loc + i+4] == 'e')){
  
        wattron(win.text_editor, scheme->statements[2]);
        for(j = 0; j <= 4; j++){
       
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x) {
            lines++;
            if(lines == max_y) continue;
            wmove(win.text_editor, y+1, 0);
          }
          waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[2]);
        continue;
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;

    case 't':
      if((buffer[current_loc + i+1] == 'y') && (buffer[current_loc +i+2] == 'p') && (buffer[current_loc + i+3] == 'e') && (buffer[current_loc + i+4] == 'd') && (buffer[current_loc + i+5] == 'e') && (buffer[current_loc + i+6] == 'f')){
      
        wattron(win.text_editor, scheme->statements[3]);
        for (j = 0; j <= 6; j++){
        
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x) wmove(win.text_editor, y+1, 0);
          waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[3]);
        continue;
      }
      waddch(win.text_editor, buffer[current_loc + i]);
      break;

    case 'r':
      if((buffer[current_loc +i+1] == 'e') && (buffer[current_loc +i+2] == 't') && (buffer[current_loc+i+3] == 'u') && (buffer[current_loc +i+4] == 'r') && (buffer[current_loc+i+5] == 'n')){
      
        wattron(win.text_editor, scheme->statements[4]);
        for (j = 0; j <= 5; j++){
        
          int y, x;
          getyx(win.text_editor, y, x);
          if(x == max_x) {
            lines++;
            if(lines == max_y) continue;
            wmove(win.text_editor, y, x);
          }
          waddch(win.text_editor, buffer[current_loc + i+j]);
        }
        i+=(j-1);
        wattroff(win.text_editor, scheme->statements[4]);
        continue;
      }
      break;

    default:
      waddch(win.text_editor, buffer[current_loc + i]);
      break;
    }
  }
  //wprintw(win.text_editor, "%d\n %d\n", max_y, i);
  current_loc = current_loc + i;
  return current_loc;
}
