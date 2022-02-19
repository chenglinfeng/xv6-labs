#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// void match(char *path, char *name)
// {
//     char *p;
//     int i,j;
//     // Find first character after last slash.
//     for (p = path + strlen(path); p >= path && *p != '/'; p--)
//         ;
//     p++;
//     for( i = 0; i < strlen(p); i++)
//     {
//         j = 0;
//         while(name[j] == p[i+j] && j < strlen(name))
//         {
//             j++;
//         }
//         if(j == strlen(name))
//         {
//             printf("%s\n",path);
//             break;
//         }        
//     }
//     return;

// }

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
        // match(path, name);
        for (p = path + strlen(path); p >= path && *p != '/'; p--)
            ;
        p++;
        if(strcmp(p,name) == 0)
            printf("%s\n",path);
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0) continue;
            if (de.name[0] == '.' && de.name[1] == 0) continue;
            if (de.name[0] == '.'&&de.name[1] =='.'&&de.name[2] == 0) continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            find(buf, name);
        }
        break;
    }
    close(fd);
    return ;
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("error return\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
