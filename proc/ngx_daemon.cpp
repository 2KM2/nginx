#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>     //errno
#include <sys/stat.h>
#include <fcntl.h>



#include "ngx_macro.h"
#include  "ngx_conf.h"
#include  "ngx_utils.h"




int ngx_daemon()
{
    switch (fork())
    {
    case -1:
        printf("ngx_daemon()中fork()失败! \n");
        break;
    case 0:
        break;
    default:
        return 1;
    }

    //只有fork()出来的子进程才能走到这个流程
    ngx_parent = ngx_pid;     //ngx_pid是原来父进程的id，因为这里是子进程，所以子进程的ngx_parent设置为原来父进程的pid
    ngx_pid = getpid();       //当前子进程的id要重新取得

    //(2)脱离终端，终端关闭，将跟此子进程无关
    if (setsid() == -1)  
    {
        printf("ngx_daemon()中setsid()失败! \n");
        return -1;
    }
    //(3)设置为0，不要让它来限制文件权限，以免引起混乱
    umask(0); 
    //(4)打开黑洞设备，以读写方式打开
    int fd = open("/dev/null", O_RDWR);
    if(fd==-1)
    {
        printf("ngx_daemon()中open(\"/dev/null\")失败! \n");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) == -1) //先关闭STDIN_FILENO[这是规矩，已经打开的描述符，动他之前，先close]，类似于指针指向null，让/dev/null成为标准输入；
    {
        printf("ngx_daemon()中dup2(STDIN)失败! \n");        
        return -1;
    }

    if (fd > STDERR_FILENO)  //fd应该是3，这个应该成立
    {
        if (close(fd) == -1)  //释放资源这样这个文件描述符就可以被复用；不然这个数字【文件描述符】会被一直占着；
        {
            printf("ngx_daemon()中close(fd)失败! \n");
            return -1;
        }
    }

    return 0;//子进程返回
}