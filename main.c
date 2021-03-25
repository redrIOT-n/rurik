#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cdk.h>

#define CNTRL_KEY(x) ((x) & 0x1f)

/*################### configure windows ###################*/
struct window_config{

  int starty, startx;
  int height, width;
  int y, x;
};
typedef struct window_config WinConfig;

/*##################### all windows #######################*/
struct windows{

  WINDOW *menubar;

  WINDOW *hold_editor;
  WINDOW *text_editor;

  WINDOW *hold_terminal;
  WINDOW *terminal;

  WINDOW *hold_debugger;
  WINDOW *debugger;

  WINDOW *hold_file_browser;
  WINDOW *file_browser;

  WINDOW *statusbar;
};
typedef struct windows Window;

Window win;

/*####################### utilities ######################*/
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
void show_editor(void){

  WinConfig size;

  assign_sizes(&size, LINES-22, COLS-22, 1, 1, 1, 1);
  win.text_editor = create_newder(win.hold_editor, &size);

  wrefresh(win.text_editor);

  keypad(win.text_editor, true);
}

void editor_hold(void){

  WinConfig size;
  assign_sizes(&size, LINES-20, COLS-20, 3, 0, 1, 1);
  win.hold_editor = create_new(&size);

  mvwprintw(win.hold_editor, 0, size.x, "Editor");

  show_editor();

  wrefresh(win.hold_editor);
}

/*##################### file browser ####################*/
struct cdk_scrns{

  CDKSCREEN *file_screen;
  CDKSCROLL *cdk_file_browser;
  CDK_CONST char **file_list;
};

typedef struct cdk_scrns FileScreen;
FileScreen fs;
int no_of_files;

void create_scroll(int x, int y){

  chtype c = 0 | A_REVERSE;
  fs.cdk_file_browser = newCDKScroll(
            fs.file_screen,    x, 
            y,              RIGHT, 
            0,              0, 
            NULL,           fs.file_list, 
            no_of_files,    false, 
            c,              false,
            false
            );

  drawCDKScroll(fs.cdk_file_browser, false);
}


void open_file(char *filename){

  return;
}

void listen_file(void){

  int ret = activateCDKScroll(fs.cdk_file_browser, 0);
  open_file(fs.file_list[ret]);
}


CDK_CONST char **find_files(void){

  int bufferSize = 64;
  CDK_CONST char **buffer;
  int i = 0;

  buffer = malloc(bufferSize * sizeof(CDK_CONST char *));

  DIR *dir = opendir(".");
  if(dir == NULL){

    on_i_error("Could not open current directory [1]");
    return NULL;
  }

  struct dirent *dentry;

  dentry = readdir(dir);
  if(dentry == NULL){

    on_i_error("No files found");
    return NULL;
  }

  while (dentry != NULL){

    if((strcmp(dentry->d_name, ".") == 0) || (strcmp(dentry->d_name, "..") == 0) || (dentry->d_type == 4)){

      dentry = readdir(dir);
      continue;
    }
    if(i == bufferSize){

      buffer = realloc(buffer, sizeof(CDK_CONST char *) * (bufferSize += 3));
    }

    buffer[i] = (CDK_CONST char *)dentry->d_name;

    dentry = readdir(dir);
    i++;
  }
  no_of_files = i;

  return buffer;
}

int show_files(void){

  fs.file_list = find_files();
  if(fs.file_list == NULL) return -1;

  win.file_browser = derwin(win.hold_file_browser, LINES-22, 18, 1, 1);
  fs.file_screen = initCDKScreen(win.file_browser);

  raw();

  if(fs.file_screen == NULL) {

    on_i_error("Could not init file browser window");
    return -1;
  }

  create_scroll(COLS-22, 4);
  return 0;
}

void file_browser_hold(void){

  WinConfig size;

  assign_sizes(&size, LINES-20, 20, 3, COLS-20, 1, 1);
  win.hold_file_browser = create_new(&size);

  mvwprintw(win.hold_file_browser, 0, size.x, "Files");
  wrefresh(win.hold_file_browser);

  if(show_files() == -1) return;

  wrefresh(win.hold_file_browser);
}

/*####################### terminal ######################*/
void changeDir(char *dirname){

  char *path;
  if(cmpr(dirname, "~")){

    path = getenv("HOME");
    chdir(path);
    errno = 0;
    if(errno) on_i_error("Could not change directory");
    return;
  }
  chdir(dirname);
  errno = 0;
  if(errno) on_i_error("Could not change directory");
  return;
}

int start_process(char **string){

  int wstatus;
  pid_t pid;

  char *filename = "result.txt";

  pid_t p = fork();

  if(p == -1){

    on_i_error("Could not start new process [fork failed]");
    return -1;
  }
  else if(p == 0){

    int fd = open(filename, O_WRONLY | O_SYNC | O_CREAT | O_TRUNC, 0777);

    if(fd == -1){

      on_i_error("open failed");
      abort();
    }

    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);

    if(execvp(string[0], string) == -1){

      abort();
    }
  }
  else {

    do{
      pid = waitpid(p, &wstatus, WUNTRACED);
    }while(!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

    int fd1 = open(filename, O_RDONLY);
    if(fd1 == -1){

      on_i_error("Could not open the file");
      return -1;
    }
    int selret;

    struct timeval time;
    time.tv_sec = 20;
    time.tv_usec = 1000;

    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(fd1, &readfds);

    selret = select(fd1 + 1, 
        &readfds, 
        NULL, 
        NULL, 
        &time);
    if(selret == -1){

      on_i_error("File read/write error [1]");
      return -1;
    }
    else if(!selret){

      on_i_error("File read/write error [2]");
      return -1;
    }
    if(FD_ISSET(fd1, &readfds)){

      int fd2 = open(filename, O_RDONLY);
      if(fd2 == -1 && fd2 == fd1){

        on_i_error("open file error");
        return -1;
      }
      FILE *fh = fdopen(fd2, "r");
      if(fh == NULL){

        on_i_error("error");
        return -1;
      }

      int max_y, max_x;
      int y, x;

      getmaxyx(win.terminal, max_y, max_x);
            char *s;
      char buffer[max_x - 2];

      while((s = fgets(buffer, sizeof(buffer), fh))){

        if(s == NULL){

          if(errno == EINTR) continue;
          else if (errno != EINTR){

            on_i_error("File read/write error [3]");
            return -1;
          }else if (feof(fh)){

            break;
          }
        }
        getyx(win.terminal, y, x);

        mvwprintw(win.terminal, y, 1, buffer);
      }
    }
  }
  return pid;
}

char *read_buffer(void){

  int c;
  int y = 1, x = 0;;
  int i = 0;
  int bufferSize = 64;


  char *buffer = malloc(sizeof(char) * bufferSize);
  if(buffer == NULL){
    on_i_error("Terminal Error [Memory not enough]");
    return NULL;
  }

  while(1){

    c = wgetch(win.terminal);

    if((c == '\n') || (c == EOF)){

      if(i == bufferSize){
        buffer = realloc(buffer, sizeof(char) * (bufferSize += 1));
        if(buffer == NULL){

          on_i_error("Terminal Error [Memory not enough]");
          return NULL;
        }
      }
      buffer[i] = '\0';
      break;

    }else{

      if(i == bufferSize){

        buffer = realloc(buffer, sizeof(char) * (bufferSize *= 2));
        if(buffer == NULL){

          on_i_error("Terminal Error [Memory not enough]");
          return NULL;
        }
      }
      buffer[i] = c;
      i++;
      x++;
    }
  }
  return buffer;
}

char **parse(char *str){

  char *dels = " \t";
  char *token;
  int i = 0;
  int bufferSize = 100;

  char **string = malloc(sizeof(char *) * (bufferSize));
  if(string == NULL){

    on_i_error("Terminal Error [Memory not enough]");
    return NULL;
  }

  token = strtok(str, dels);
  while(token != NULL){

    if(i >= bufferSize){
      string = realloc(string, sizeof(char *) * (bufferSize += 2));
      if(string == NULL){

        on_i_error("Terminal Error [Memory not enough]");
        return NULL;
      }
    }

    string[i] = token;
    token = strtok(NULL, dels);
    i++;
  }
  string[i] = NULL;

  return string;
}

void run_shell(void){

  echo();

  keypad(win.terminal, true);
  scrollok(win.terminal, true);
  char *word;
  char **commands;

  int y, x;
  //int max_y, max_x;

  while (1){

    getyx(win.terminal, y, x);

    mvwprintw(win.terminal, y, 1, "\nshell ~>");
    y++;
    wrefresh(win.terminal);

    word = read_buffer();
    wrefresh(win.terminal);
    if(word == NULL) return;

    if(word[0] == '\0'){

      free(word);
      continue;
    }
    if(cmpr(word, "exit")){

      free(word);
      return;
    }
    commands = parse(word);
    if(commands == NULL) return;

    if(cmpr(commands[0], "cd")){

      if(commands[1] == NULL){

        chdir(".");
        on_i_error("changed to current directory");
        continue;
      }
      changeDir(commands[1]);
      on_i_error("directory changed");
    }else{

      if(start_process(commands) == -1){
        continue;
      }
    }
    wrefresh(win.terminal);
  }
}
void show_terminal(void){

  WinConfig size;

  assign_sizes(&size, 20 - 8, (COLS/2) - 2, 1, 1, 1, 1);
  win.terminal = create_newder(win.hold_terminal, &size);

  wrefresh(win.terminal);

  keypad(win.terminal, true);
}

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

            wborder(win.hold_file_browser, '|',  '|', '~', '~', '+', '+', '+', '+');
            on_i_error("file browser : navigation = [Arrow up/down] : exit = [CTRL + q]");

            //listen_file();

            on_i_error("menu bar : navigation = [Arrow left/right], [CTRL + f], [CTRL + e], [CTRL + t], [CTRL + d], [CTRL + q]");

            box(win.hold_file_browser, 0, 0);
            mvwprintw(win.hold_file_browser, 0, 1, "Files");
            wrefresh(win.hold_file_browser);
            break;

          case 2:

            wborder(win.hold_terminal, '|', '|', '~', '~', '+', '+', '+', '+');
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

void show_menu(void){

  WinConfig size;

  assign_sizes(&size, 3, COLS, 0, 0, 1, 1);
  win.menubar = create_new(&size);
  keypad(win.menubar, true);

  display_content(0, size.y, size.x);
  wrefresh(win.menubar);
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
