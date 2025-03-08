#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 递归查找函数，查找路径为 path 的目录下是否有目标文件 target
void find(char* path, char* target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    // 打开目录
    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    // 获取目录的状态信息
    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // 文件、目录分别处理
    if(st.type==T_FILE) {
    // 如果是文件，检查文件名是否与目标文件名匹配
        if(strcmp(path + strlen(path) - strlen(target), target) == 0) {
            printf("%s\n", path);
        }
    }
    // 如果是目录
   else if(st.type==T_DIR){
        // 检查路径长度是否超出缓冲区大小
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';//如果path是a，这时候就已经变成了a/
        // 读取目录项
        while (read(fd, &de, sizeof(de)) == sizeof(de)) { //循环读取目录项，即读取目录中所有的文件名，read会根据fd的文件偏移量读取数据，如果读完了，read会返回0
            if(de.inum == 0) { //如果读取的文件的inode为0，说明这个文件或者/目录有可能是空的，跳过
                continue;
            }
            // 修改此处，使用 strcpy 替代 memmove
            memmove(p, de.name, DIRSIZ);//将目录项的名称拷贝到缓冲区中。注意这个p指针一直指向的是数组的末尾，所以每次拷贝完之后都要将指针移动到末尾，否则就会覆盖之前的内容,假设目录中第一个文件名是b，这时候buf就变成了a/b
            p[DIRSIZ] = 0;            
            //strcpy(p,de.name);用这一行代替上面两行也可以  
            // 获取目录项的状态信息
            if (stat(buf, &st) < 0) { // 获取目录项的状态信息，buf是a/b
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            // 排除 "." 和 ".." 目录
            if(strcmp(buf + strlen(buf) - 2, "/.") != 0 && strcmp(buf + strlen(buf) - 3, "/..") != 0) { //跳过.和..目录，因为这两个目录是默认存在的，所以不需要再递归了
                find(buf, target); // 递归查找子目录
            }
        }
   }
    close(fd); // 关闭目录
}

int main(int argc, char *argv[]) {
    if(argc < 3) { // 如果参数不足，退出程序
        fprintf(2, "please input find [path] [filename]\n");
        exit(0);
    }
    char target[512];
    strcpy(target, argv[2]); // 将目标文件名存储在 target 中
    find(argv[1], target); // 调用查找函数
    exit(0);
}