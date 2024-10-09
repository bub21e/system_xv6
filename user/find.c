#include "../kernel/types.h"
#include "../kernel/fs.h"
#include "../kernel/stat.h"
#include "user.h"

int find(char* path,char* name);

int main(int argc,char* argv[]){
    int find_success = 0;

    if(argc<2 || argc>3){
        fprintf(2,"argc error!");
    }
    else if(argc == 2){
        find_success = find(".",argv[1]);
    }
    else{
        find_success = find(argv[1],argv[2]);
    }
    if(find_success==0){
    }
    exit(0);
}

int find(char* path,char* name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    int find_success = 0;

    if((fd = open(path,0))<0){
        fprintf(2,"find: can not open %s\n",path);
        return -1;
    }

    if((fstat(fd,&st))<0){
        fprintf(2,"find: can not stat %s\n",path);
        close(fd);
        return -1;
    }
    //original route
    strcpy(buf,path);
    p = buf + strlen(path);
    *p++ = '/';
    
    while(read(fd,&de,sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        else if(!strcmp(de.name,".") || !strcmp(de.name,".."))
            continue;
        else{
            memmove(p,de.name,DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf,&st)<0){
                printf("find: can not stat %s\n",buf);
                continue;
            }
            switch (st.type)
            {
            case T_FILE:
                if(!strcmp(de.name,name)){
                    printf("%s\n",buf);
                    find_success = 1;
                }
                break;
            case T_DIR:
                if(!strcmp(de.name,name)){
                    printf("%s\n",buf);
                    find_success = 1;
                }
                find_success = find(buf,name);
                break;
            default:
                break;
            }
        }
    }
    close(fd);
    return find_success;
}