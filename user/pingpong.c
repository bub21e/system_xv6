#include "../kernel/types.h"
#include "user.h"

int main(int argc,char* argv[])
{
    int p[2];
    pipe(p);

    char read_text[10];

    int pid_child=-1,pid_father=getpid();
    int pid = fork();
    

    if(pid < 0){
        printf("error created!");
    }
    else if(pid == 0){
        pid_child = getpid();
        read(p[0],read_text,10);
        printf("%d: received ping from pid %d\n",pid_child,pid_father);
        write(p[1],"pong",10);
        exit(0);
    }
    else{
        write(p[1],"ping",10);
        wait(0);
        read(p[0],read_text,10);
        printf("%d: received pong from pid %d\n",pid_father,pid);
        exit(0);
    }
    return 0;
}