#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  
  int time = argc;
  if (time < 0)
  {
    fprintf(2, "time must be positive integer\n");
    exit(1);
  }
  else
  {
    sleep(time);
    exit(0);
  }
}
