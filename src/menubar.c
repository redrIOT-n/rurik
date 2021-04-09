#include "menubar.h"

/*###################### menu bar #######################*/
void display_content(int highlight, int y, int x){

  char *choices[] = {"File", "Editor", "Terminal", "Debug", "Exit"};
  static int no_of_choices = sizeof(choices) / sizeof(char *);

  for (int i = 0; i < no_of_choices; i++){

    if(i == highlight){
      wattron(win.menubar, A_REVERSE);
      if(i == 0) 
        mvwprintw(win.menubar, y, x, "%s", choices[i]);
      else
        mvwprintw(win.menubar, y, x+=strlen(choices[i-1]) + 3, "%s", choices[i]);
      wattroff(win.menubar, A_REVERSE);
    }else{
      if(i == 0)
        mvwprintw(win.menubar, y, x, "%s", choices[i]);
      else
        mvwprintw(win.menubar, y, x+=strlen(choices[i-1]) + 3, "%s", choices[i]);
    }
  }
  wrefresh(win.menubar);
  return;
}

void listen_menubar(void){

  int c;
  int highlight = 0;
  on_i_error("menu bar : navigation = [Arrow left/right], [CTRL + f], [CTRL + e], [CTRL + t], [CTRL + d], [CTRL + q]");
  while(1){

    c = wgetch(win.menubar);
    switch(c){

      case CNTRL_KEY('q'):
        highlight = 4;
        on_i_error_clear();
        display_content(highlight, 1, 1);
        return;

      case CNTRL_KEY('f'):
        highlight = 0;
        display_content(highlight, 1, 1);

        wborder(win.hold_file_browser, '|', '|', '~', '~', '+', '+', '+', '+');
        wrefresh(win.hold_file_browser);

        on_i_error("file browser : navigation = [Arrow up/down] : exit = [CTRL + q]");
        listen_file();

        on_i_error("menu bar : navigation = [Arrow left/right], [CTRL + f], [CTRL + e], [CTRL + t], [CTRL + d], [CTRL + q]");

        box(win.hold_file_browser, 0, 0);
        mvwprintw(win.hold_file_browser, 0, 1, "Files");
        wrefresh(win.hold_file_browser);

        break;

      case CNTRL_KEY('e'):
        highlight = 1;
        display_content(highlight, 1, 1);
        break;

      case CNTRL_KEY('t'):
        highlight = 2;
        display_content(highlight, 1, 1);

        wborder(win.hold_terminal, '|', '|', '~', '~', '+', '+', '+', '+');
        wrefresh(win.hold_terminal);

        on_i_error("terminal : navigation = [keyboard]");
        run_shell();

        on_i_error("menu bar : navigation = [Arrow left/right], [CTRL + f], [CTRL + e], [CTRL + t], [CTRL + d], [CTRL + q]");

        box(win.hold_terminal, 0, 0);
        mvwprintw(win.hold_terminal, 0, 1, "Terminal");
        wrefresh(win.hold_terminal);

        noecho();

        break;

      case CNTRL_KEY('d'):
        highlight = 3;
        display_content(highlight, 1, 1);
        break;

      case KEY_LEFT:
        highlight--;
        if(highlight == -1) highlight = 4;
        display_content(highlight, 1, 1);
        break;

      case KEY_RIGHT:
        highlight++;
        if(highlight == 5) highlight = 0; 
        display_content(highlight, 1, 1);
        break;

      case '\n':
        switch(highlight){

          case 0:

            highlight = 0;
            display_content(highlight, 1, 1);
        
            wborder(win.hold_file_browser, '|',  '|', '~', '~', '+', '+', '+', '+');
            wrefresh(win.hold_file_browser);

            on_i_error("file browser : navigation = [Arrow up/down] : exit = [CTRL + q]");
            listen_file();

            on_i_error("menu bar : navigation = [Arrow left/right], [CTRL + f], [CTRL + e], [CTRL + t], [CTRL + d], [CTRL + q]");

            box(win.hold_file_browser, 0, 0);
            mvwprintw(win.hold_file_browser, 0, 1, "Files");
            wrefresh(win.hold_file_browser);
            break;

          case 2:

            wborder(win.hold_terminal, '|', '|', '~', '~', '+', '+', '+', '+');
            wrefresh(win.hold_terminal);
            on_i_error("terminal : navigation = [keyboard]");
            run_shell();

            on_i_error("menu bar : navigation = [Arrow left/right], [CTRL + f], [CTRL + e], [CTRL + t], [CTRL + d], [CTRL + q]");

            box(win.hold_terminal, 0, 0);
            mvwprintw(win.hold_terminal, 0, 1, "Terminal");
            wrefresh(win.hold_terminal);

            break;

          case 4:
            on_i_error_clear();
            return;
        }
        break;
    }
  }
}
