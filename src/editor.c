#include "editor.h"

/*################## editor ################*/
void print_screen(char *buffer, CurrentFile *f){

  int max_y, max_x;
  float max_scrolls;

  getmaxyx(win.text_editor, max_y, max_x);

  max_scrolls = (float)f->no_of_lines / (float)max_y;
  max_scrolls = ceilf(max_scrolls);

  check_syntax(buffer, 0, max_y, max_x);
}

//this way of implementing scroll is kinda silly. to be reimplemented.
void listen_editor(void){

  echo();
  noraw();

  int c, y, x;
  int max_y, max_x;

  c = wgetch(win.text_editor);
  while(1){

    getmaxyx(win.text_editor, max_y, max_x);
    getyx(win.text_editor, y, x);
    switch(c){

      case CNTRL_KEY('q'):
        return;
      
      case KEY_UP:
        y--;
        if(y == -1) y = max_y - 1;
        break;
      
      case KEY_DOWN:
        y++;
        if(y == max_y) y = 0;
        break;
      
      case KEY_LEFT:
        x--;
        if(x == -1){
          
          y--;
          if(y == -1) y = max_y - 1;
          x = max_x- 1;
        }
    }
    wmove(win.text_editor, y, x);
    wrefresh(win.text_editor);
  }

  return;
}

