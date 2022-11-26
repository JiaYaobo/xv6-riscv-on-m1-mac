#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
  int p[2];
  int c[2];
  char buf[1];
  pipe(p);
  pipe(c);
  write(p[1], "a", 1);

  if (fork() == 0){
    close(p[1]);
    close(c[0]);
    read(p[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(c[1], buf, 1);
    close(c[1]);
    exit(0);
  }else {
    close(p[0]);
    close(c[1]);
    write(p[1], buf, 1);
    close(p[1]);
    read(c[0], buf, 1);
    printf("%d: received pong\n", getpid());
    exit(0);
  }
  return 0;
}
