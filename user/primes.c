#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void childProcess(int p[]){
    close(p[1]);
    int i,n;
    // 说明管道中没有元素了
    if(read(p[0], &i, sizeof(i)) == 0) {
        close(p[0]);
        exit(0);
    }
    printf("prime %d\n", i);
    int newP[2];
    pipe(newP);
    //int n;
    // 子进程
    if(fork() == 0) {
        close(p[0]);
        close(newP[1]);
        childProcess(newP);
    }
    else{
        close(newP[0]);
        while(read(p[0], &n, sizeof(n)) != 0) {
            if(n % i != 0){
                write(newP[1], &n, sizeof(n));
               //printf("n = %d", n);
            }
        }
        close(p[0]);
        close(newP[1]);
        wait(0);
    }
    exit(0);
}
int main(int argc, char* argv[]) {
    
    //读入所有的整数
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid < 0) {
        fprintf(2, "fork error!!!");
        exit(1);
    }// 子进程执行递归操作
    else if( pid == 0) {
        //close(p[1]);
        childProcess(p);    
    }
    else {
        close(p[0]);
        for(int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}
