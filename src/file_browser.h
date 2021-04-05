#ifndef _FILE_BROWSER_
#define _FILE_BROWSER_

#include "main.h"
#include "file_handle.h"

//struct to hold cdk widgets
struct cdk_scrns{

  CDKSCREEN *file_screen;
  CDKSCROLL *cdk_file_browser;
  CDK_CONST char **file_list;
};

typedef struct cdk_scrns FileScreen;
//ooopps more globals
FileScreen fs;

//another one :)
int no_of_files;

void create_scroll(int x, int y);
void listen_file(void);
CDK_CONST char **find_files(void);
int show_files(void);

#endif
