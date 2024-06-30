#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int pid;
  // int parent_id=getpid();
  if((pid=fork())==0){ //child proc
  
  }
  else{ //parent proc
    wait(0);
  }
  exit(0);
}
