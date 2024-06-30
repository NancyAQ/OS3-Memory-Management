#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int pid;
  char* shared_data=(char*)malloc(12*sizeof(char));
  int parent_id=getpid();
  if((pid=fork())==0){ //child proc
    printf("Size before mapping is %d\n",get_size());
    uint64 new_va=map_shared_pages(parent_id,(uint64)shared_data,sizeof(shared_data));
    printf("Size after mapping is %d\n",get_size());
    strcpy((char*)new_va,"Hello daddy");
    unmap_shared_pages(new_va,sizeof(shared_data));
    printf("Size after unmapping is %d\n",get_size());
    char* st=(char*)malloc(12*sizeof(char));
    strcpy(st,"Hello daddy");
    printf("string is %s\n",st);
    printf("Size after allocating is %d\n",get_size());
    exit(0);
  }
  else{ //parent proc
  sleep(2); //wait until child writes
    printf("My kids calling: %s\n",shared_data);
  wait(0);
  }
  exit(0);
}
