#include "terminal.h"

/*####################### terminal ######################*/ 

/***warning
 * terminal implemented in this program is not very powerful.
 * it is built on top of ncurses and weird behaviours are very common
 * but i have no idea on how to implement a good terminal
 * one i implemented is just a rip off of my previous project `BURUWA shell`
 * i tried vterm but failed to fing any documentaion.
 * i may not be able to implement another terminal
 * but if you can, or have any idea about improving this terminal,
 * please dont hesitate to implement or inform me,
 *                                                 -redrIOT-n
 */  

//change directory of the process (shell)
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


//start a child process using user input and waiting for output.
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

    int fd = open(filename, O_WRONLY | O_SYNC | O_CREAT | O_TRUNC, 0777);     //open a file `filename` with write only on child process

    if(fd == -1){

      on_i_error("open failed");
      abort();
    }

    dup2(fd, STDOUT_FILENO);                                                  //replace stdout with open file descriptor fd
    dup2(fd, STDERR_FILENO);                                                  //replace stderr with open file descriptor fd
    close(fd);

    if(execvp(string[0], string) == -1){

      abort();
    }
  }
  else {

    do{
      pid = waitpid(p, &wstatus, WUNTRACED);                                  //waiting for child process to exit
    }while(!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

    int fd1 = open(filename, O_RDONLY);                                       //open file `filename` for reading
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

    selret = select(fd1 + 1,                                                   //waiting 20 seconds till data is available for reading 
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

      while((s = fgets(buffer, sizeof(buffer), fh))){                           //get line from file untill EOF reached

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


//read buffer from user
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


//break `char *` to `char *[]`
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


//start the shell by priting `shell ~>`
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


//show terminal to user that it exists
void show_terminal(void){

  WinConfig size;

  assign_sizes(&size, 20 - 8, (COLS/2) - 2, 1, 1, 1, 1);
  win.terminal = create_newder(win.hold_terminal, &size);                     //creating a subwindow(window inside another) to run shell 
                                                                              //without destroying borders of the window
  wrefresh(win.terminal);

  keypad(win.terminal, true);
}
