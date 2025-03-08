#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int read_fd){
    int prime;
    if(read(read_fd,&prime,sizeof(prime))==0){
        close(read_fd);
        return;
    }
    printf("prime %d\n",prime);

    int p[2];
    pipe(p);

    int pid=fork();

    if(pid==0){
        close(p[0]);
        int num;
        while (read(read_fd, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }
        close(read_fd);
        close(p[1]);
        exit(0);
    }
    else{
        close(p[1]);
        sieve(p[0]);
        close(p[0]);
        wait(0);
    }
}

int 
main(){
    int p[2];
    pipe(p);
    int pid=fork();
    if(pid==0){
        close(p[0]);
        for(int i=2;i<=35;i++){
        write(p[1],&i,sizeof(i));
        }
        close(p[1]);
        exit(0);
    }
    else{
        close(p[1]);
        sieve(p[0]);
        close(p[0]);
        exit(0);
    }

    return 0;
}