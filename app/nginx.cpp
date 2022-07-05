#include "common.h"
#include "ngx_conf.h"
int main()
{
    printf("Nginx  start\n");
    //我们在main中，先把配置读出来，供后续使用 
    CConfig *p_config = CConfig::GetInstance(); //单例类
    if(p_config->Load("nginx.conf") == false) //把配置文件内容载入到内存
    {
        printf("配置文件载入失败，退出!\n");
        exit(1);
    }

    return 0;
}