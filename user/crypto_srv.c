#include "kernel/types.h"
#include "user/user.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "kernel/fcntl.h"
#include "kernel/crypto.h"

int main(void) {
  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  printf("crypto_srv: starting\n");

  // TODO: implement the cryptographic server here
  if(getpid()!=2){
    printf("Server did not start with PID 2\n");
    exit(1);
  }
  for(;;){
    void *mapped_request_va;
    uint64 request_size;
    if(take_shared_memory_request(&mapped_request_va,&request_size)==0){
        struct crypto_op* processed_op=( struct crypto_op*)mapped_request_va;
        //sanity checks
        if(processed_op->state!=CRYPTO_OP_STATE_INIT || (processed_op->type!=CRYPTO_OP_TYPE_DECRYPT 
        && processed_op->type!=CRYPTO_OP_TYPE_ENCRYPT) ){
            asm volatile ("fence rw,rw" : : : "memory");
            processed_op->state=CRYPTO_OP_STATE_ERROR;
        }

        int key_index=0;
        for(int i=0;i<(processed_op->data_size);i++){
            (processed_op->payload+processed_op->key_size)[i]=((processed_op->payload+processed_op->key_size)[i])^(processed_op->payload[key_index]);
            key_index=key_index+1;
            key_index=key_index%(processed_op->key_size);
        }
        asm volatile ("fence rw,rw" : : : "memory");
        processed_op->state=CRYPTO_OP_STATE_DONE;
        remove_shared_memory_request(mapped_request_va,request_size);
    }

  }


  exit(0);
}
