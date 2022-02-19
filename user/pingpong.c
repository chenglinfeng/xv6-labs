#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char * argv[])
{
    int pipe_fd[2];
    int pipe1_fd[2];
    int pid;
    char buf[4];
    pipe(pipe_fd);
    pipe(pipe1_fd);
    pid = fork();
    if(pid < 0)
    {
        printf("failed to fork \n");
        exit(0);
    }
    else if(pid == 0)
    {
        read(pipe1_fd[0],buf,4);
        printf("%d: received pong\n",getpid());
        write(pipe_fd[1],"pong",4);
    }
    else{
        printf("%d: received ping\n",getpid());
        write(pipe1_fd[1],"ping",4);
        read(pipe_fd[0],buf,4);
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    close(pipe1_fd[0]);
    close(pipe1_fd[1]);
    exit(0);
}