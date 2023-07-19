#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**
 *鏈€鍩虹鐨刦ork渚嬪瓙
 **/
int main(int argc, char const *argv[])
{
    pid_t pid;
    //鍒ゆ柇1
    if ((pid=fork()) < 0)
    {
        perror("fork error");
    }
    //鍒ゆ柇2
    else if (pid == 0)//瀛愯繘绋�
    {
         printf("child getpid()=%d\n", getpid());
    }
    //鍒ゆ柇3
    else if(pid > 0)//鐖惰繘绋�
    {
        printf("parent getpid()=%d\n", getpid());
    }
 
    return 0;
}
