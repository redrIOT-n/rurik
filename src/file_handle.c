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

    //now_max_cols++;
    //if(buffer[0] == '\n'){

    //  f->no_of_lines++;
    //  if(now_max_cols > prev_max_cols) f->max_cols = now_max_cols;
    //}
    write(cpyfile, buffer, sizeof(buffer));
    ret = read(orgfile, buffer, sizeof(buffer));
  }
  close(orgfile);
  close(cpyfile);

  //f->fileSize = f->max_cols * f->no_of_lines;
  return 0;
}

char *open_file(CurrentFile *f, char *filename){

  //int align  = 64000 - f->fileSize;
  f->fileSize = 640000;  
  FILE *fh = fopen(filename, "r+");
  if(fh == NULL) return NULL;

  char *buffer = calloc(f->fileSize, sizeof(char));
  if(buffer == NULL) return NULL;
  
  fread(buffer, sizeof(char), f->fileSize, fh);
  return buffer;
}
