#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
int main()
{
    pid_t pid;
    printf("test nginx\n");


    for(;;)
    {
        sleep(1);
        printf("进程休息1秒\n");
    }
    printf("nginx end\n");
    return 0;
}
//ps -eo pid,ppid,sid,tty,pgrep,comm |grep -E 'bash|PID|nginx'