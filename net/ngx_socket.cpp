#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>    //uintptr_t
#include <stdarg.h>    //va_start....
#include <unistd.h>    //STDERR_FILENO等
#include <sys/time.h>  //gettimeofday
#include <time.h>      //localtime_r
#include <fcntl.h>     //open
#include <errno.h>     //errno
#include <sys/socket.h>
#include <sys/ioctl.h> //ioctl
#include <arpa/inet.h>

#include  "ngx_conf.h"
#include  "ngx_macro.h"
#include  "ngx_global.h"
#include  "ngx_utils.h"
#include  "ngx_socket.h"



CSocekt::CSocekt()
{
    m_ListenPortCount = 1;
    return ;
}


CSocekt::~CSocekt()
{
    //释放必须的内存
    std::vector<lpngx_listening_t>::iterator pos;	
	for(pos = m_ListenSocketList.begin(); pos != m_ListenSocketList.end(); ++pos) //vector
	{		
		delete (*pos); //一定要把指针指向的内存干掉，不然内存泄漏
	}//end for
	m_ListenSocketList.clear(); 
    return;
}


bool CSocekt::Initialize()
{
    bool reco = ngx_open_listening_sockets();
    return reco;
}


//监听端口【支持多个端口】，这里遵从nginx的函数命名
//在创建worker进程之前就要执行这个函数；
bool CSocekt::ngx_open_listening_sockets()
{
    CConfig *p_config = CConfig::GetInstance(); //单例类
    m_ListenPortCount = p_config->GetIntDefault("ListenPortCount",m_ListenPortCount);

    int                isock;                //socket
    struct sockaddr_in serv_addr;            //服务器的地址结构体
    int                iport;                //端口
    char               strinfo[100];         //临时字符串 

    //初始化
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                //选择协议族为IPV4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//监听本地所有的IP地址；INADDR_ANY表示的是一个服务器上所有的网卡（服务器可能不止一个网卡）多个本地ip地址都进行绑定端口号，进行侦听。
    for(int i = 0; i < m_ListenPortCount; i++) //要监听这么多个端口
    {
        isock = socket(AF_INET,SOCK_STREAM,0);
        if(-1==isock)
        {
            printf("CSocekt::Initialize() sock failed ,i =%d\n",i);
            return false;
        }
        ///设置 SO_REUSEADDR，目的第五章第三节讲解的非常清楚：主要是解决TIME_WAIT这个状态导致bind()失败的问题
        int reuseaddr = 1;  //1:打开对应的设置项
        if(setsockopt(isock,SOL_SOCKET, SO_REUSEADDR,(const void *) &reuseaddr, sizeof(reuseaddr)) == -1)
        {
            printf("CSocekt::Initialize setsockopt (SO_REUSEADDR) i=%d\n",i);
            close(isock); //无需理会是否正常执行了                                                  
            return false;
        }

        //设置该socket为非阻塞
        if(setnonblocking(isock)==false)
        {
            printf("CSocekt::Initialize setnonblocking i=%d\n",i);
            close(isock); //无需理会是否正常执行了                                                  
            return false;
        }
        
        //设置本服务器要监听的地址和端口，这样客户端才能连接到该地址和端口并发送数据 
        strinfo[0] = 0;
        sprintf(strinfo,"ListenPort%d",i);

        iport = p_config->GetIntDefault(strinfo,10000);
        serv_addr.sin_port = htons((in_port_t)iport); 

        //绑定服务器地址结构
        if(bind(isock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        {
            printf("CSocekt::Initialize bind i=%d\n",i);
            close(isock); //无需理会是否正常执行了                                                  
            return false;
        }

        //开始监听
        if(listen(isock,NGX_LISTEN_BACKLOG) == -1)
        {
            printf("CSocekt::Initialize listen i=%d\n",i);
            close(isock); //无需理会是否正常执行了                                                  
            return false;
        }

        //可以 放到列表里来
        lpngx_listening_t p_listensocketitem = new ngx_listening_t;
        memset(p_listensocketitem,0,sizeof(ngx_listening_t));    
        p_listensocketitem->port = iport;
        p_listensocketitem->fd = isock;
        printf("listen port %d  successed\n",iport);
        m_ListenSocketList.push_back(p_listensocketitem);
    } // end for 

    return true;
}

bool CSocekt::setnonblocking(int sockfd) 
{
    int nb=1; //0：清除，1：设置  
    if(ioctl(sockfd, FIONBIO, &nb) == -1) //FIONBIO：设置/清除非阻塞I/O标记：0：清除，1：设置
    {
        return false;
    }
    return true;
}

//关闭socket，什么时候用，我们现在先不确定，先把这个函数预备在这里
void CSocekt::ngx_close_listening_sockets()
{
    for(int i = 0; i < m_ListenPortCount; i++) //要关闭这么多个监听端口
    {  
        //ngx_log_stderr(0,"端口是%d,socketid是%d.",m_ListenSocketList[i]->port,m_ListenSocketList[i]->fd);
        close(m_ListenSocketList[i]->fd);
        printf("CSocekt close port:%d\n",m_ListenSocketList[i]->port);
    }//end for(int i = 0; i < m_ListenPortCount; i++)
    return;
}
