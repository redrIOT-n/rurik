#include "file_handle.h"

int copy_file(CurrentFile *f, char *filename, char copyfilename[]){

  int ret;
  char buffer[1];
  f->no_of_lines = 0;
  f->max_cols = 0;
  int now_max_cols = 0;
  int prev_max_cols = 0;

  snprintf(copyfilename, strlen(filename) + 5, "%s.cpy", filename);

  int orgfile; 
  int cpyfile;

  orgfile = open(filename, O_RDONLY);
  if(orgfile == -1) return -1;

  cpyfile = open(copyfilename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(cpyfile == -1) return -1;

  ret = read(orgfile, buffer, sizeof(buffer));
  while(ret == 1){

    now_max_cols++;
    if(buffer[0] == '\n'){

      f->no_of_lines++;
      if(now_max_cols > prev_max_cols) f->max_cols = now_max_cols;
    }
    write(cpyfile, buffer, sizeof(buffer));
    ret = read(orgfile, buffer, sizeof(buffer));
  }
  close(orgfile);
  close(cpyfile);

  f->fileSize = f->max_cols * f->no_of_lines;
  return 0;
}

char *open_file(CurrentFile *f, char *filename){

  int fd;
  
  f->fileSize = f->fileSize + 640;
  fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  char *buffer = mmap(NULL, f->fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  return buffer;
}
