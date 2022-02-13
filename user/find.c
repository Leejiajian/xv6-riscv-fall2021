#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(const char *path, const char *next, const char *fileName, int bootstrap){
    int fd;
    struct dirent de;
    struct stat st;
    char this_path[512 + 1];
    strcpy(this_path, path);
    if(bootstrap == 0) {
        int len = strlen(this_path);
        this_path[len] = '/';      
        strcpy(this_path+len+1, next);
    }

    if((fd = open(this_path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type) {
    case T_FILE:
        if(strcmp(next, fileName) == 0)
            printf("%s\n", this_path);
        break;
    case T_DIR:
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0)
                continue;
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            find(this_path, de.name, fileName, 0);        
        } 
        break;  
    }
    close(fd);


}
int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(2, "find: Usage...");
        exit(1);
    }
    const char *path = argv[1];
    const char *fileName = argv[2];
    find(path, "", fileName, 1);
    exit(0);// 为什么这里return 0会报错
}
