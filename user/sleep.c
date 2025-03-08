#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]){
    if(argc<2){
        fprintf(2,"Usage: sleep seconds\n");
        exit(1);
    }
    int i=sleep(atoi(argv[1]));
    if(i != 0){
        fprintf(2, "sleep remains %d\n",i);
      }
    exit(0);
}