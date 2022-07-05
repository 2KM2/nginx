
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include  "ngx_global.h"
#include  "ngx_conf.h"
#include  "ngx_utils.h"



//和设置标题有关的全局量
char **g_os_argv;            //原始命令行参数数组,在main中会被赋值
char *gp_envmem = NULL;      //指向自己分配的env环境变量的内存
int  g_environlen = 0;       //环境变量所占内存大小

int main(int argc, char *const *argv)
{
    g_os_argv = (char **) argv;
    ngx_init_setproctitle();    //把环境变量搬家
    //我们在main中，先把配置读出来，供后续使用 
    CConfig *p_config = CConfig::GetInstance(); //单例类
    if(p_config->Load("nginx.conf") == false) //把配置文件内容载入到内存
    {
        printf("配置文件载入失败，退出!\n");
        exit(1);
    }
    /*for (int i = 0; environ[i]; i++)
    {
        printf("evriron[%d]地址=%x    " ,i,(unsigned int)((unsigned long)environ[i]));
        printf("evriron[%d]内容=%s\n" ,i,environ[i]);
    }*/

    ngx_setproctitle("nginx: master process");
    printf("argc=%d,argv[0]=%s\n",argc,argv[0]);
    printf("evriron[0]=%s\n" , environ[0]);
    printf("evriron[1]=%s\n" , environ[1]);
    printf("evriron[2]=%s\n" , environ[2]);
    printf("evriron[3]=%s\n" , environ[3]);
    printf("evriron[4]=%s\n" , environ[4]);
    for (;;)
    {
        sleep(1);
    }

    //对于因为设置可执行程序标题导致的环境变量分配的内存，我们应该释放
    if(gp_envmem)
    {
        delete[] gp_envmem;
        gp_envmem = NULL;
    }
    return 0;
}