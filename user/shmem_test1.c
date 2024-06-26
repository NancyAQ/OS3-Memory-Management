#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  //placehorder code
  char* greeting=(char*)malloc(12*sizeof(char)); //allocate mem for string
  strcpy(greeting,"Hello child");
  int papa_pid=getpid();
  if(fork()==0){ //child
    uint64 new_va=map_shared_pages(papa_pid,(uint64)greeting,12);
    sleep(2);
    char * papa_calling=(char*) new_va;
    printf("papa is calling: %s\n",papa_calling);
    sleep(3);
    exit(0);
  }
  else{ 
    wait(0); //waiting for child to finish
  }
  exit(0);
}