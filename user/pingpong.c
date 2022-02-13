#include "user/user.h"
#include "kernel/types.h"
int main(int argc, char* argv[]) {
    
    //创建两个管道
    // attention p1[0]是出口， p1[1]是入口
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char received[10] ;
    int pid = fork();
    if(pid < 0) {
        fprintf(2 , "fork error\n");
        exit(1);
    }
    //child process
    else if(pid == 0) {
        close(p1[1]);
        close(p2[0]);
        int n = read(p1[0], received, 4);
        close(p1[0]);
        if(n > 0)
            printf("%d: received %s\n", getpid(), received);
        write(p2[1], "pong", 4);
        close(p2[1]);
    }
    // father process
    else{
        close(p1[0]);
        close(p2[1]);
        write(p1[1],"ping", 4);
        close(p1[1]);
        //wait(0);
        int n = read(p2[0], received, 4);
        close(p2[0]);
        if(n > 0)
            printf("%d: received %s\n", getpid(), received);
        
    }
    exit(0);
    
    /*
    //创建两个管道
    // attention p1[0]是出口， p1[1]是入口
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char received;
    int pid = fork();
    if(pid < 0) {
        fprintf(2 , "fork error\n");
        exit(1);
    }
    //child process
    else if(pid == 0) {
        close(p1[1]);
        close(p2[0]);
        int n = read(p1[0], &received, 1);
        close(p1[0]);
        if(n > 0)
            printf("%d: received ping\n", getpid());
        write(p2[1], "0", 4);
        close(p2[1]);
    }
    // father process
    else{
        close(p1[0]);
        close(p2[1]);
        write(p1[1],"1", 4);
        close(p1[1]);
        //wait(0);
        int n = read(p2[0], &received, 4);
        close(p2[0]);
        if(n > 0)
            printf("%d: received pong\n", getpid());
        
    }
    exit(0);
    */
}
    