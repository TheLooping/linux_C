/**
 * TCP服务器程序(tcpserver.c)
 * Usage: ./tcpserver port
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>


define BUFSIZE 1024

/* 错误报告 */
static void bail(const char *on_what){
    fputs(strerror(error),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

/* 父进程捕获SIGCHLD信号,设置子进程退出SIGCHLD信号处理程序 */
static void sigchild_handler(int signo){
    pid_t pid;//结束子进程ID
    int status;//子进程退出状态
    char msg[] = "SIGCHLD caught\n";

    write(STDOUT_FILENO,msg,sizeof(msg));
    /* 等待所有退出的子进程 */
    do{
        pid = waitpid(-1,&status,WNOHANG);//-1:任意子进程； WNOHANG:继续运行，非阻塞；
        if(pid == -1){
            bail("waitpid()");
        }
        if(pid > 0){
            if(WIFEXITED(status)){
                printf("Child %d terminated with exit status %d\n",pid,WEXITSTATUS(status));
            }else if(WIFSIGNALED(status)){
                printf("Child %d terminated by signal %d\n",pid,WTERMSIG(status));
            }else if(WIFSTOPPED(status)){
                printf("Child %d stopped by signal %d\n",pid,WSTOPSIG(status));
            }else if(WIFCONTINUED(status)){
                printf("Child %d continued\n",pid);
            }
        }
    }while(pid > 0);

}

int main (int argc,char *argv[]){
    int socket_fd;//服务器套接字 fd:file descriptor
    struct sockaddr_in server_addr;//服务器地址
    int port;//端口号
    int child_fd;//子进程套接字
    socklen_t sin_size;//地址长度
    struct sockaddr_in client_addr;//客户端地址
    char reqBuf[BUFSIZE];//请求缓冲区
    char resBuf[BUFSIZE];//响应缓冲区
    int readlen;//读取长度，read()返回值

    /* SIGCHLD信号处理函数初始化 */
    struct sigaction child_sa;//sa:signal action
    memset(&child_sa,0,sizeof(child_sa));
    child_sa.sa_handler = sigchild_handler;
    child_sa.sa_flags |= SA_RESTART;//SA_RESTART:系统调用被信号中断后自动重启
    if(sigaction(SIGCHLD,&child_sa,NULL) == -1){
        bail("sigaction()");
    }
    if(argc != 2){
        fprintf(stderr,"Usage:%s port\n",argv[0]);
        exit(1);
    }
    if((port = atoi(argv[1])) < 0){
        fprintf(stderr,"Usage:%s port\n",argv[0]);
        exit(1);
    }

    /* 创建套接字 */
    if((socket_fd = socket(PF_INET,SOCK_STREAM,0)) == 0){
        bail("socket()");
    }

    /* 绑定IP地址和端口号 */
    //初始化服务器地址
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = PF_INET;//IPv4
    server_addr.sin_port = htons(port);//端口号
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//将服务器的IP地址设置为0.0.0.0，表示接受任意IP地址的连接
    //绑定
    if((bind(socket_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))) == -1){
        bail("bind()");
    }

    /* 监听端口 */
    if((listen(socket_fd,128)) == -1){
        bail("listen()");
    }

    /* 服务器主循环，接受客户端连接 */
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        fprintf(stdout,"Waiting for a client...\n");
        /* 接受客户端连接 */
        if((child_fd = accept(socket_fd,(struct sockaddr *)(client_addr),&sin_size)) == -1){
            bail("accept()");
        }
        fprintf(stdout,"Client %s:%d connected\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

        /* 创建子进程处理客户端请求 */
        pid = fork();
        switch (pid){
            case -1:
                bail("fork()");
                break;
            case 0: //子进程
                fprintf(stdout,"Child process %d created\n",getpid());
                /* 循环处理客户端请求 */
                while(1){
                    readlen = read(child_fd,reqBuf,BUFSIZE);
                    if(readlen == -1){
                        bail("read()");
                    }
                    else if(readlen == 0){
                        fprintf(stdout,"Client %s:%d disconnected\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                        break;
                    }
                    else{
                        reqBuf[readlen] = '\0';
                        /* 判断是否为quit请求 */
                        if(strcmp(reqBuf,"quit") == 0){
                            fprintf(stdout,"Client %s:%d quit\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                            break;
                        }
                        /* 响应客户端请求 */
                        //demo:将客户端请求的字符串转换为大写
                        for(int i = 0;i < readlen;i++){
                            if(reqBuf[i] >= 'a' && reqBuf[i] <= 'z'){
                                resBuf[i] = reqBuf[i] - 32;
                            }else{
                                resBuf[i] = reqBuf[i];
                            }
                        }

                        /* 发送响应 */
                        if((write(child_fd,resBuf,readlen)) < 0){
                            bail("write()");
                        }                        
                    }                    
                }
            default://pid > 0 父进程中的子进程ID
                /* 关闭子进程套接字，实际上是减少child_fd的引用计数 */
                close(child_fd);
                break;
        }
    }

}

