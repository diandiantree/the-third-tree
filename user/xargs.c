#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc,char *argv[]){
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args]\n");
        exit(1);
    }
    char buf[1024];
    int index=0;
    char c;
    while(read(0,&c,1)>0){
        if(c=='\n')
        {
            buf[index]='\0';
            index=0;
            char *result[100];
            int i=1;
            while(i<=argc-1){
                result[i-1]=argv[i];
                i++;
            }
            result[i-1]=buf;
            result[i] = (void *)0;
            if(fork()==0){
                if(exec(argv[1],result)<0){
                    fprintf(2, "exec failed\n");
                    exit(1);
                }
            }
            else{
                wait(0);
            }
        }
        else
        {
            buf[index]=c;
            index++;
        }
    }
    exit(0);
}