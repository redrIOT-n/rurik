#ifndef FILE_HANDLE
#define FILE_HANDLE

#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

typedef struct {

  int no_of_lines;
  int max_cols;
  int fileSize;
} CurrentFile;

int copy_file(CurrentFile *f, char *filename, char *cpyfilename);
char *open_file(CurrentFile *f, char *filename);

#endif
