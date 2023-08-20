/**
 * UDP服务器端单进程程序(udpserver.c)
 * Usage: ./udpserver port
 * quit请求断开连接
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
    static void
    bail(const char *on_what)
{
    fputs(strerror(error), stderr);
    fputs(": ", stderr);
    fputs(on_what, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int socket_fd;                  // 服务器套接字 fd:file descriptor
    struct sockaddr_in server_addr; // 服务器地址
    int port;                       // 端口号
    int child_fd;                   // 子进程套接字
    socklen_t sin_size;             // 地址长度
    struct sockaddr_in client_addr; // 客户端地址
    char reqBuf[BUFSIZE];           // 请求缓冲区
    char resBuf[BUFSIZE];           // 响应缓冲区
    int recvLen;                    // 读取长度，recvfrom()返回值
    int resLen;                     // 响应长度，sendto()返回值

    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s port\n", argv[0]);
        exit(1);
    }
    if ((port = atoi(argv[1])) < 0)
    {
        fprintf(stderr, "Usage:%s port\n", argv[0]);
        exit(1);
    }

    /* 创建套接字 */
    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == 0)
    {
        bail("socket()");
    }

    /* 绑定IP地址和端口号 */
    // 初始化服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;                // IPv4
    server_addr.sin_port = htons(port);              // 端口号
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 将服务器的IP地址设置为0.0.0.0，表示接受任意IP地址的连接
    // 绑定
    if ((bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1)
    {
        bail("bind()");
    }

    /* 服务器主循环，接受客户端连接 */
    sin_size = sizeof(struct sockaddr_in); // 客户端地址长度
    while (1)
    {
        fprintf(stdout, "Waiting for a client...\n");
        /* 接受客户端连接 */
        if ((recvLen = recvfrom(socket_fd, reqBuf, BUFSIZE, 0, (struct sockaddr *)&client_addr, &sin_size)) == -1)
        {
            bail("recvfrom()");
        }
        fprintf(stdout, "Client %s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        if (recvLen == 0)
        {
            fprintf(stdout, "Client %s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            break;
        }
        else
        {
            reqBuf[recvLen] = '\0';
            /* 判断是否为quit请求 */
            if (strcmp(reqBuf, "quit") == 0)
            {
                fprintf(stdout, "Client %s:%d quit\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                break;
            }
            /* 响应客户端请求 */
            // demo:将客户端请求的字符串转换为大写
            for (int i = 0; i < recvLen; i++)
            {
                if (reqBuf[i] >= 'a' && reqBuf[i] <= 'z')
                {
                    resBuf[i] = reqBuf[i] - 32;
                }
                else
                {
                    resBuf[i] = reqBuf[i];
                }
            }
            resLen = recvLen;

            /* 发送响应 */
            if ((sendto(socket_fd, resBuf, resLen, 0, (struct sockaddr *)&client_addr, sin_size)) == -1)
            {
                bail("sendto()");
            }
        }
    }
    /* 关闭套接字 */
    close(socket_fd);
    return 0;
}
