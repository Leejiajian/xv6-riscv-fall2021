// xargs 命令的作用是将标准输入转化为命令行参数
/*
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_BUF 512
#define MAXARGS 15
void fmt_args(char**args, char *param, int *args_cnt, int len){
    char store[MAXARGS];
    // 每个参数长度
    int curLength = 0;
    for(int i = 0; i < len; ++i) {
        if((param[i] == ' ' || param[i] == '\n') && curLength) { 
            args[*args_cnt] = malloc(curLength+1);
            memcpy(args[*args_cnt], store, curLength);
            args[*args_cnt][curLength] = 0; 
            //memset(store, 0, curLength);
            ++(*args_cnt);
            curLength = 0;
        }
        else {
            store[curLength] = param[i];
            ++curLength;
        }
    }
}
int main(int argc, char *argv[]) {
    char *args[MAXARG + 1];
    char param[MAX_BUF];
    int args_cnt = 0;
    // xargs 拷贝了之后的命令行参数
    for(int i = 1; i < argc; ++i){
        args[args_cnt] = malloc(sizeof(argv[i]));
        memcpy(args[args_cnt++], argv[i], sizeof(argv[i]));
    }
    int read_cnt;
    if((read_cnt = read(0, param, sizeof(param))) > 0) {
        fmt_args(args, param, &args_cnt, strlen(param));
    }
    exec(args[0], args);      
    exit(0);
}
*/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "Usage: xargs command...");
        exit(1);
    }
    char *_argv[MAXARG];
    char argument[1000]; 
    for(int i = 1; i < argc; ++i) {
        _argv[i - 1] = argv[i];
    }
    int argv_cnt, pos, cnt, stat = 1;
    char ch;
    while(stat) {
        pos = cnt = 0, argv_cnt = argc - 1;

        while(1) {
            stat = read(0, &ch, 1);
            // 由于最后都有换行符，所以都已经写入
            if(stat == 0) {
                exit(0);
            }
            if(ch == ' ' || ch == '\n') {
                argument[cnt++] = 0;
                _argv[argv_cnt++] = &argument[pos]; 
                pos = cnt;
            }else {
                argument[cnt++] = ch;

            }
            if(ch == '\n') { break; }

        }
        if(fork() == 0) {
            exec(_argv[0], _argv);

        }
        else {
            wait(0);
            
        }   
 
    }
    exit(0);

}







