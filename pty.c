//
// 编译：gcc pty.c -o pty
//

#include <signal.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

pid_t child = 0;

// 信号处理
void sighandler(int signum) {
	if (child > 0) {
  		killpg(child, signum);
  		exit(signum);
	}
}

// Run a command in a pty.
// 在pty中运行一个命令
// Usage: /path/to/this/binary command to run
// 用法：./pty ps -f
int main(int argc, char *argv[]) {
	if(argc < 2) {
		return EX_USAGE;
	}
	
	int master;
	child = forkpty(&master, NULL,NULL, NULL);
	
	if(child == -1) {
		perror("failed to fork pty"); // fork pty 失败
		return EX_OSERR;
	}
	
	if(child == 0) {
		// we're in the child process, so replace it with the command
		// 我们在子进程中，所以用命令替换它
		execvp(argv[1],argv+1);
		perror("failed to execute command"); // 执行命令失败
		return EX_OSERR;
	}
	
	// trap kill signals and forward them to child process
	// 捕获终止信号，并将其转发给子进程
	signal(SIGHUP, sighandler); // 终止进程 挂起信号
	signal(SIGINT, sighandler); // 终止进程 Ctrl + C
	signal(SIGTERM, sighandler); // 终止进程 kill
	
	const int buf_size = 1024;
	char buf[buf_size];
	fd_set fds;
	ssize_t bytes_read;
	
	// forward the output continuously
	// 连续向前输出
	while(1) {
		FD_ZERO(&fds);
		FD_SET(master, &fds);
		
		if(select(master+1, &fds, NULL, NULL, NULL)>0 && FD_ISSET(master, &fds)){
			bytes_read = read(master, buf, buf_size);
			if(bytes_read <= 0){
				return EXIT_SUCCESS;
			}
			
			if(write(STDOUT_FILENO, buf, bytes_read)!=bytes_read){
				perror("failed to write stdout"); // 写入标准输出失败
				return EX_OSERR;
			}
		}
	}
}
































