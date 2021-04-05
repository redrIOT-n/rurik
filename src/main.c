#include "main.h"

/*###################### error handle ####################*/
void on_i_error_clear(void){

  size_t i;
  char string[COLS - 1];
  for(i = 0; i < sizeof(string); i++){

    string[i] = ' ';
  }
  string[i] = '\0';

  mvwprintw(win.statusbar, 1, 1, "%s", string);
  box(win.statusbar, 0, 0);
  wrefresh(win.statusbar);

  refresh();
  return;
}

void on_i_error(const char *error){

  on_i_error_clear();

  mvwprintw(win.statusbar, 1, 1, "%s", error);
  wrefresh(win.statusbar);
  refresh();
  return;
}

/*####################### utilities ######################*/
//compare two strings
bool cmpr(char *cmp1, char *cmp2){

  size_t i;

  if(strlen(cmp1) != strlen(cmp2)){

    return false;
  }
  for(i = 0; i < strlen(cmp1); i++){

    if(cmp1[i] == cmp2[i]){
      continue;
    }
    else{
      return false;
    }
  }
  return true;
}

/*############### assign sizes of window #################*/

void assign_sizes(WinConfig *win, int height, int width, int starty, int startx, int y, int x){

  win->height = height;
  win->width = width;
  win->starty = starty;
  win->startx = startx;
  win->y = y;
  win->x = x;
}

/*################## create new win ######################*/
WINDOW *create_new(WinConfig *win){

  WINDOW *local = newwin(win->height, win->width, win->starty, win->startx);
  refresh();
  box(local, 0, 0);

  wrefresh(local);

  return local;
}

WINDOW *create_newder(WINDOW *org, WinConfig *win){

  WINDOW *local = derwin(org, win->height, win->width, win->starty, win->startx);
  refresh();

  wrefresh(local);

  return local;
}

/*######################## editor #######################*/
void editor_hold(void){

  WinConfig size;
  assign_sizes(&size, LINES-20, COLS-20, 3, 0, 1, 1);
  win.hold_editor = create_new(&size);

  show_editor();

  mvwprintw(win.hold_editor, 0, size.x, "Editor");
  wrefresh(win.hold_editor);
}

/*##################### file browser ####################*/

//hold scroll window
void file_browser_hold(void){

  WinConfig size;

  assign_sizes(&size, LINES-20, 20, 3, COLS-20, 1, 1);
  win.hold_file_browser = create_new(&size);

  mvwprintw(win.hold_file_browser, 0, size.x, "Files");
  wrefresh(win.hold_file_browser);

  if(show_files() == -1) return;

  wrefresh(win.hold_file_browser);
}

/*#################### Terminal ########################*/
void terminal_hold(void){

  WinConfig size;
  assign_sizes(&size, 20 - 6, COLS/2, LINES-17, 0, 1, 1);
  win.hold_terminal = create_new(&size);

  mvwprintw(win.hold_terminal, 0, size.x, "Terminal");

  show_terminal();

  wrefresh(win.hold_terminal);
}

/*####################### debugger ######################*/
void show_debugger(void){

  WinConfig size;

  assign_sizes(&size, 20-8, (COLS/2) - 2, 1, 1 , 1, 1);
  win.debugger = create_newder(win.hold_debugger, &size);

  wrefresh(win.debugger);

  keypad(win.debugger, true);
}

void debugger_hold(void){

  WinConfig size;
  assign_sizes(&size, 20-6, COLS/2, LINES-17, COLS/2, 1, 1);
  win.hold_debugger = create_new(&size);

  mvwprintw(win.hold_debugger, 0, size.x, "Debugger");

  show_debugger();

  wrefresh(win.hold_debugger);
}

/*####################### statusbar #####################*/
void show_statusbar(void){

  WinConfig size;

  assign_sizes(&size, 3, COLS, LINES-3, 0, 1, 1);
  win.statusbar = create_new(&size);

  wrefresh(win.statusbar);
}

/*####################### destroy #######################*/
void destroy(void){

  destroyCDKScroll(fs.cdk_file_browser);
  wborder(win.file_browser, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.text_editor, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.terminal, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.debugger, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.menubar, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.hold_editor, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.hold_debugger, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.hold_terminal, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(win.hold_file_browser, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

  wrefresh(win.file_browser);
  wrefresh(win.text_editor);
  wrefresh(win.terminal);
  wrefresh(win.debugger);
  wrefresh(win.menubar);
  wrefresh(win.hold_debugger);
  wrefresh(win.hold_editor);
  wrefresh(win.hold_terminal);
  wrefresh(win.hold_file_browser);

  destroyCDKScreen(fs.file_screen);
  delwin(win.file_browser);
  delwin(win.text_editor);
  delwin(win.terminal);
  delwin(win.debugger);
  delwin(win.menubar);
  delwin(win.hold_editor);
  delwin(win.hold_debugger);
  delwin(win.hold_terminal);
  delwin(win.hold_file_browser);

  endwin();
}

/*######################## main #########################*/

int main(void){

  atexit(destroy);

  initscr();
  raw();
  noecho();
  keypad(stdscr, true);

  show_menu();

  editor_hold();
  terminal_hold();
  debugger_hold();

  show_statusbar();

  file_browser_hold();

  refresh();

  listen_menubar();
  endwin();
  return 0;
}
