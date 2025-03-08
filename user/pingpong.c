#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(){
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buf=' ';
    if(fork()==0){
    read(p1[0],&buf,1);
    printf("%d: received ping\n",getpid());
    write(p2[1],&buf,sizeof(buf));
    close(p2[1]);
    exit(0);
    }
    else
    {
    write(p1[1],&buf,sizeof(buf));
    close(p1[1]);
    read(p2[0],&buf,1);
    printf("%d: received pong\n",getpid());
    wait(0);
    }
    close(p1[0]);
    close(p2[0]);
    exit(0);
}