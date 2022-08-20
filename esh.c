#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char line[128];
char *args[30];

int main(int argc, char *argv[], char *env[])
{
	printf("input command fmt: cmd arg1 arg2 ...\n");
	//char *env[] = {0,NULL};
        // 读取命令和参数，格式为: cmd arg1 arg2 ...
        line[strcspn(fgets(line, 128, stdin), "\r\n")] = 0;

        // 按空格为分割符解析命令行字符串
        int i = 0;
        args[i] = (char *)strtok(line, " ");
        while(args[++i] = (char *)strtok(NULL, " "));

        if(fork())
        { // 父进程
                int *status;
                wait(status);
                printf("Child exit status: %d\n", *status);
                return 0;
        }

        // 子进程
        int execRV = execve(args[0], args, env);
        printf("execve return value: %d\n", execRV);
        perror(NULL);

        return 0;
}
