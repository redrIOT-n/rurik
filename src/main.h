#ifndef MAIN_H
#define MAIN_H

//#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <ncurses.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <errno.h>
#include <cdk.h>
#include <math.h>

#include "debugger.h"
#include "terminal.h"
#include "menubar.h"
#include "editor.h"
#include "file_browser.h"
//#include "file_handle.h"
//#include "xmlparse.h"
//#include "attr.h"

#define CNTRL_KEY(x) ((x) & 0x1f)

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
extern Window win;

void assign_sizes(WinConfig *win, int height, int width, int starty, int startx, int y, int x);
WINDOW *create_new(WinConfig *win);
WINDOW *create_newder(WINDOW *org, WinConfig *win);
bool cmpr(char *cmp1, char *cmp2);
void on_i_error_clear(void);
void on_i_error(const char *error);

#endif
