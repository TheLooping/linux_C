#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    pid_t child_pid;
    /*创建子进程*/
    child_pid=fork();
    //判断
    if(child_pid > 0)
    {
	    sleep(1000);
    }
    else if(child_pid == 0)
    {
    	exit(0);
    }
 
    return 0;
}
