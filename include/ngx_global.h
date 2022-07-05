#pragma once

// 一些比较通用的定义放在这里


//
typedef struct
{
    char ItemName[20];
    char ItemContent[500];
}CConfItem,*LPCConfItem;



//外部全局量声明

extern char **g_os_argv;
extern char *gp_envname;
extern int   g_environlen; 