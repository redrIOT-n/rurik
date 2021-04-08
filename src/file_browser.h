#ifndef FILE_BROWSER_
#define FILE_BROWSER_

#include "main.h"
#include <cdk.h>
#include "file_handle.h"
#include "editor.h"

typedef struct cdk_scrns FileScreen;

//struct to hold cdk widgets
struct cdk_scrns{

  CDKSCREEN *file_screen;
  CDKSCROLL *cdk_file_browser;
  CDK_CONST char **file_list;
};
extern FileScreen fs;
extern int no_of_files;

void create_scroll(int x, int y);
void listen_file(void);
CDK_CONST char **find_files(void);
int show_files(void);

#endif
