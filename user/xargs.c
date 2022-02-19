#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


//read a line 
char* readline() {
    char* buf = malloc(100);
    char* p = buf;
    while(read(0, p, 1) != 0){
        if(*p == '\n' || *p == '\0'){
            *p = '\0';
            return buf;
        }
        p++;
    }
    if(p != buf) return buf;
    free(buf);
    return 0;
}


//split a line by space into string array nargv[]
//返回值为该行被切割成的块数，即数组nargv[]的长度
int split(char *buf,char *nargv[])
{
    int j = 0;
    int k = 0;
    int i;
    int flag = 0;//标志传进来的buf是否为空,flag = 1 则buf非空 
    for( i = 0; i < strlen(buf); i++)
    {
        flag = 1;
        if(buf[i] == ' ')
        {
            j++;
            k = 0;
        }
        else{
            *(nargv[j] + k++) = buf[i];
        }
    }
    if(flag == 0)
        return 0;
    return j + 1;
}


//为argv[]分配内存空间
void mallocargv(char *argv[], int argc)
{
    for (int i = 0; i < argc; i++)
        argv[i] = malloc(8);
    return;
}

//link two string_array argv[] and nargv[] into nargv[]
int catargv(char *argv[], char *nargv[], int argc)
{
    char *buf;
    int nargc;
    buf = readline();
    while(buf != 0){
        if(!fork())
        {
            mallocargv(nargv, MAXARG);
            for (int i = 1; i < argc; i++)
            {
                strcpy(nargv[i - 1], argv[i]);
            }
            break;
        }
        else
        {   
            wait(0);
            buf = readline();
            continue;
        }
    }
    if(buf == 0)
        exit(0);
    nargc = split(buf,nargv + argc - 1);
    // printf("%d\n",nargc);
    // for(int i = 0; i < nargc; i++)
    //     printf("%s\n",nargv[i+argc-1]);
    return nargc + argc-1;
}



//free space for argv
void freeargv(char *argv[], int argc)
{
    for (int i = 0; i < argc; i++)
    {
        free(argv[i]);
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("no parameters\n");
        exit(0);
    }
    

    char *nargv[MAXARG];
    int nargc;

    nargc = catargv(argv, nargv, argc);

    if(nargc == argc - 1)
    {
        exit(0);
    }


    nargv[nargc] = 0;
    //将nargc[]的后一位置为null，使得nargc[]符合exec参数的要求
    exec(nargv[0],nargv);
    freeargv(nargv, MAXARG);
    // for(int i = 0; i < nargc; i++)
    // {
    //     free(nargv[i]);
    // }

    //exec("echo",nargv);
    //t exec is not ok
    exit(0);
}