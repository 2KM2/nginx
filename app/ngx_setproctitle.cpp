
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //env
#include <string.h>

#include "ngx_global.h"
//设置可执行程序标题相关函数：分配内存，并且把环境变量拷贝到新内存中来
void ngx_init_setproctitle()
{
    //这里无需判断penvmen == NULL,有些编译器new会返回NULL，有些会报异常，但不管怎样，如果在重要的地方new失败了，你无法收场，让程序失控崩溃，助你发现问题为好； 
    gp_envmem = new char[g_envneedmem]; 
    memset(gp_envmem,0,g_envneedmem);  //内存要清空防止出现问题

    char *ptmp = gp_envmem;
    //把原来的内存内容搬到新地方来
    for (int i = 0; environ[i]; i++) 
    {
        size_t size = strlen(environ[i])+1 ; //不要拉下+1，否则内存全乱套了，因为strlen是不包括字符串末尾的\0的
        strcpy(ptmp,environ[i]);      //把原环境变量内容拷贝到新地方【新内存】
        environ[i] = ptmp;            //然后还要让新环境变量指向这段新内存
        ptmp += size;
    }
    return;
}

//设置可执行程序标题
void ngx_setproctitle(const char *title)
{
    size_t ititlelen = strlen(title); 
    size_t e_environlen = 0;     //e表示局部变量
    for (int i = 0; g_os_argv[i]; i++)  
    {
        e_environlen += strlen(g_os_argv[i]) + 1;
    }

    size_t esy = e_environlen + g_envneedmem;
    if( esy <= ititlelen)
    {
        return;
    }

    g_os_argv[1] = NULL;
      //(4)把标题弄进来，注意原来的命令行参数都会被覆盖掉，不要再使用这些命令行参数,而且g_os_argv[1]已经被设置为NULL了
    char *ptmp = g_os_argv[0]; //让ptmp指向g_os_argv所指向的内存
    strcpy(ptmp,title);
    ptmp += ititlelen; //跳过标题

    //(5)把剩余的原argv以及environ所占的内存全部清0，否则会出现在ps的cmd列可能还会残余一些没有被覆盖的内容；
    size_t cha = esy - ititlelen;  //内存总和减去标题字符串长度(不含字符串末尾的\0)，剩余的大小，就是要memset的；
    memset(ptmp,0,cha);    
}