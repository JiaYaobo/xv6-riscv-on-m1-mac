#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int i;
  char buf[512];
  char *nargv[MAXARG];
  int n, args_index = 0;
  for (i = 1; i < argc && i < MAXARG; i++)
  {
    nargv[args_index ++ ] = argv[i];
  }

  // read from stdin
  while ((n = read(0, buf, 512)) > 0)
  {
    if (fork() == 0)
    {
      char *child_args = (char *)malloc(sizeof(buf));
      int index = 0;
      for (i = 0; i < n; i++)
      {
        if (buf[i] == ' ' || buf[i] == '\n')
        {
          child_args[index] = 0;
          nargv[args_index ++ ] = child_args;
          index = 0;
          child_args = (char *)malloc(sizeof(buf));
        }
        else
        {
          child_args[index ++ ] = buf[i];
        }
      }
      child_args[index] = 0;
      nargv[args_index] = 0;
      exec(argv[1], nargv);
    }else wait((int *) 0);
  }
  exit(0);
}
