#include "editor.h"
// find a bettter way for scrolling.
// a way to devide file into screens using no of lines in the file. each part of the file to be aligh// ned with max_y and max_x.  current_loc of the buffer might be useful.
//
/*################## editor ################*/
void print_screen(char *buffer, CurrentFile *f){

  wborder(win.hold_editor, '|', '|', '~', '~', '+', '+', '+', '+');
  wrefresh(win.hold_editor);
  int max_y, max_x;
  float max_scrolls;

  getmaxyx(win.text_editor, max_y, max_x);

  f->no_of_lines = f->fileSize / max_x;
  max_scrolls = (float)f->no_of_lines / (float)max_y;
  max_scrolls = ceilf(max_scrolls);

  int current_loc = check_syntax(buffer, 0, max_y, max_x);
  if(current_loc == -1) return;

  wrefresh(win.text_editor);
  raw();
  
  listen_editor(buffer, max_scrolls, max_y, max_x);

  box(win.hold_editor, 0, 0);
  wrefresh(win.hold_editor);
  return;
}

void clear_editor(int max_y, int max_x){

  char string[max_x];
  for (size_t i = 0; i < sizeof(string); i++) string[i] = ' ';
  string[max_x] = '\0';
  wmove(win.text_editor, 0, 0);
  for (int j = 0; j < max_y; j++) wprintw(win.text_editor, "%s", string);
  wrefresh(win.text_editor);
  refresh();
}

//this way of implementing scroll is kinda silly. to be reimplemented.
void listen_editor(char *buffer, int max_scrolls, int max_y, int max_x){

  echo();

  int c, y, x;
  int scroll = 0;

  while(c = wgetch(win.text_editor)){

    getyx(win.text_editor, y, x);
    switch(c){

      case CNTRL_KEY('q'):
        return;
      
      case KEY_UP:
        scroll--;
        if(scroll == -1) scroll = max_scrolls;
        clear_editor(max_y, max_x);
        if((check_syntax(buffer, max_y * scroll, max_y, max_x)) == -1) return; //change max_y * scroll
        break;

      case KEY_DOWN:
        scroll++;
        if(scroll == max_scrolls+1) scroll = 0;
        clear_editor(max_y, max_x);
        if((check_syntax(buffer, max_y * scroll, max_y, max_x)) == -1) return; // second arg represent the file offset to start printing to the screen
        break;

      case KEY_LEFT:
        x--;
        if(x == -1) x = max_x;
        wmove(win.text_editor, y, x);
        break;

      case KEY_RIGHT:
        x++;
        if(x == max_x+1) x = 0;
        wmove(win.text_editor, y, x);
        break;

      default:
        waddch(win.text_editor, c);
    }
  wrefresh(win.text_editor);
  }
  return;  
}
