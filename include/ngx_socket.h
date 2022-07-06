#pragma once

#include <vector>
using namespace std;
#define NGX_LISTEN_BACKLOG  511

typedef struct ngx_listening_s  //和监听端口有关的结构
{
	int            port;   //监听的端口号
	int            fd;     //套接字句柄socket
}ngx_listening_t,*lpngx_listening_t;



class CSocekt
{
public:
	CSocekt();
	virtual ~CSocekt();
public:
    virtual bool Initialize();                            //初始化函数
public:
	int ngx_epoll_init();
	int ngx_epoll_add_event();
private:
	void ReadConf(); 
	bool ngx_open_listening_sockets();
	void ngx_close_listening_sockets();
	bool setnonblocking(int sockfd);
private:
	int  m_ListenPortCount;     //所有监听得端口数量
	std::vector<lpngx_listening_t> m_ListenSocketList;//监听套接字队列
};