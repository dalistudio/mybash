//
// 编译命令： gcc mybash.c -o mybash
// 原理：“fork-exec”模式，第一步，调用 fork()生成一个子进程，第二步，进程替换
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// 功能：解析命令行的命令和参数
// 参数1：命令行
// 参数2：命令分解成字符串数组
// 返回：命令名
char * get_cmd(char buff[], char * myargv[])
{
	if(buff == NULL || myargv == NULL)
	{
		return NULL;
	}
	
	// 以空格分解字符串
	char * s = strtok(buff, " ");
	int i=0;
	while(s != NULL)
	{
		myargv[i++] = s;
		s = strtok(NULL, " ");
	}
	return myargv[0]; // 返回第一个元素，即为命令程序名
}


/*
    终端颜色格式如下：
    转义以ESC开头，即/33
    \033[显示方式;前景色;背景色m
    \033[0m 默认
    \033[1;32;40m 绿色
    \033[1;31;40m 红色
    显示方式：
    0=默认值；1=高亮；22=非粗体；4=下划线；24=非下划线；5=闪烁；25=非闪烁；7=反显；27=非反显
    前景色：
    30=黑色；31=红色；32=绿色；33=黄色；34=蓝色；35=洋红色；36=青色；37=白色
    背景色：
    40=黑色；41=红色；42=绿色；43=黄色；44=蓝色；45=洋红色；46=青色；47=白色
*/

int main()
{
	while(1)
	{
		// 命令行缓冲大小
		char buff[1024] = {0};
		//printf("uboss$");
		printf("\033[0;32muboss\033[0;34m>\033[0m");
		fflush(stdout); // 刷新缓冲区，使printf函数中的内容输出
	
		fgets(buff, 128, stdin); // 键盘输入的命令和参数
		buff[strlen(buff)-1] = 0; // 去掉最后的 \n
	
		char * myargv[32] = {0}; // 32个参数的指针数组
		char * cmd = get_cmd(buff,myargv); // 解析命令行
		if(cmd == NULL)
		{
			continue; // 如果命令为空，结束本次循环，重新输入
		}
		else if(strcmp(cmd, "exit") == 0)
		{
			break; // 如果输入的是 exit 就直接退出程序
		}
		else
		{
			// fork() 进程并 execp 执行
			pid_t pid = fork();
			if(pid == -1)
			{
				//printf("fork error\n");
				perror(NULL);
				continue;
			}
			if(pid == 0)
			{
				// 替换进程
				// 注意myargv最后一个元素必须为空
				execvp(cmd,myargv); // 如果程序执行到本行，则替换失败
				//printf("exec error\n");
				perror(NULL);
				exit(0);
			}
			wait(NULL); // 父进程调用wait()，避免产生僵尸进程，因为不用关心子进程的退出
			            // 只是不想让子进程变成僵尸进程，所以参数为NULL
		}
	}
}































