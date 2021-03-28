#include "file_handle.h"

int copy_file(CurrentFile *f, char *filename, char *copyfilename){

  char c, prevc;
  char buffer[1];
  f->no_of_lines = 0;
  f->max_cols = 0;
  int now_max_cols = 0;
  int prev_max_cols = 0;

  snprintf(copyfilename, strlen(filename) + 2, "%s.cpy", filename);

  FILE *orgfile, *cpyfile;

  orgfile = fopen(filename, "r");
  if(orgfile == NULL) return -1;

  cpyfile = fopen(copyfilename, "w");
  if(cpyfile == NULL) return -1;

  while(c = fgetc(orgfile) != EOF){

    now_max_cols++;
    if(c == '\n'){

      f->no_of_lines++;
      if(now_max_cols > prev_max_cols) f->max_cols = now_max_cols;
    }
    if(fputc(c, orgfile) == EOF) return -1;
  }
  fclose(orgfile);
  fclose(cpyfile);
  return 0;
}

void open_file(CurrentFile *f, char *filename){

  int fd;
  
  f->fileSize = f->fileSize + 2048;
  fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  char *buffer = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

}
