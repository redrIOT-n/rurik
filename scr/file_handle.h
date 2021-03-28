#ifndef FILE_HANDLE
#define FILE_HANDLE

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct {

  int no_of_lines;
  int max_cols;
  int fileSize;
} CurrentFile;

int copy_file(char *filename, char *cpyfilename);
void open_file(char *filename);

#endif
