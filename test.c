#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>


void ErrorHandler(const char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    int szClntAddr;
    char message[] = "Hello, world!";
    if(argc != 2)
    {
        printf ("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        ErrorHandler("WSAStartup failed");
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if(hServSock == INVALID_SOCKET)
        ErrorHandler("socket error");
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hServSock, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        ErrorHandler("bind socket error");
    }

    if(listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandler("listen socket error");
    
    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR *)&clntAddr, &szClntAddr);
    if(hClntSock == INVALID_SOCKET)
        ErrorHandler("accept error");
    send(hClntSock, message, sizeof(message), 0);
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

void ErrorHandler(const char *message)
{
    fputs(message, stderr);

    fputc('\n', stderr);
    exit(1);
}
