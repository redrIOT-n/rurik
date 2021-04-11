#include "file_browser.h"

int no_of_files;
FileScreen fs;

//create scroll window
void create_scroll(int x, int y){

  chtype c = 0 | A_REVERSE;
  fs.cdk_file_browser = newCDKScroll(fs.file_screen, x, y, 
                                      RIGHT, 0, 0, NULL, fs.file_list, 
                                      no_of_files, false, c,              
                                      false, false);

  drawCDKScroll(fs.cdk_file_browser, false);
}

//start_listening from file browser
void listen_file(void){

  int ret = activateCDKScroll(fs.cdk_file_browser, 0);
  int bufferSize = strlen(fs.file_list[ret]) + 1;
  char filename[bufferSize];

  memcpy(filename, fs.file_list[ret], bufferSize);

  free(fs.file_list);

  CurrentFile f;
  
  char copyfile[bufferSize + 6];
  if(copy_file(&f, filename, copyfile) == 1){

    on_i_error("Error copying file");
    return;
  }

  int i = 0;
  char *mapped_buffer = open_file(&f, copyfile);

  print_screen(mapped_buffer, &f);
}

//find files.. ignore all directories
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


//show found files in the scroll window
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
