#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"


int main(int argc, char *argv[]){
  struct sysinfo info;
  sysinfo(&info);
  printf("%d %d\n", info.freemem, info.nproc);
  exit(0);
}
