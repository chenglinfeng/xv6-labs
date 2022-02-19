#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int rd){
    int n;
    if(read(rd, &n, 4) < 4)
        exit(0);
    printf("prime %d\n", n);
    int p[2];
    int pid;
    int num;
    pipe(p);
    if((pid = fork()) > 0)
    {
        close(p[0]);
        while(read(rd, &num, 4) != 0){
            if(num % n != 0){
                write(p[1], &num, 4);
            }
        }
        close(rd);
        close(p[1]);
        wait(0);
    }
    else if(pid == 0)
    {
        close(p[1]);
        prime(p[0]);
    }
}

int
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    int pid = fork();
    if(pid != 0){
        // first
        close(p[0]);
        for(int i = 2; i <= 35; i++){
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait(0);
    }else{
        close(p[1]);
        prime(p[0]);
    }
    exit(0);
}