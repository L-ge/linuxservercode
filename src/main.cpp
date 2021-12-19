#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>

#include "amyconfig.h"
#include "threadpool.h"
#include "http_conn.h"

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000

extern int addfd( int epollfd, int fd, bool one_shot );
extern int removefd( int epollfd, int fd );

void addsig( int sig, void( handler )(int), bool restart = true )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    if( restart )
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

void show_error( int connfd, const char* info )
{
    printf( "%s", info );
    send( connfd, info, strlen( info ), 0 );
    close( connfd );
}

#include <iostream>

int main( int argc, char* argv[] )
{
    AMyConfig myConfig("./config/serverbase.ini");
    std::string sIP = myConfig.getCfgValue("base","ip");
    std::string sPort = myConfig.getCfgValue("base","port");
    printf("Start mylinuxserver: IP:%s, Port:%s \n", sIP.c_str(), sPort.c_str());

    // if( argc <= 2 )
    // {
    //     printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
    //     return 1;
    // }
    // const char* ip = argv[1];
    // int port = atoi( argv[2] );

    const char* ip = sIP.c_str();
    int nPort = atoi(sPort.c_str());

    // 忽略“Broken pipe向一个没有读端的管道写数据”信号
    addsig(SIGPIPE, SIG_IGN);

    // 创建线程池
    threadpool<http_conn>* pThreadPool = nullptr;
    try
    {
        pThreadPool = new threadpool<http_conn>;
    }
    catch(...)
    {
        return 1;
    }

    // 创建连接用户数组，最大连接数 MAX_FD
    http_conn* pUsers = new http_conn[MAX_FD];
    assert(pUsers);
    //int user_count = 0;

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    /*
     * TCP提供了异常终止一个连接的方法，即给对方发送一个复位报文段。
     * 一旦发送了复位报文段，发送端所有排队等待发送的数据都将被丢弃。
     * 应用程序可以使用socket选项SO_LINGER来发送复位报文段，以异常终止一个连接。
    */
    struct linger tmp = { 1, 0 };
    setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof( tmp ));

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(nPort);

    int ret = 0;
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret >= 0);

    ret = listen( listenfd, 5 );
    assert( ret >= 0 );

    epoll_event events[MAX_EVENT_NUMBER];   // 传出参数
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    while(true)
    {
        // 第四个参数-1表示阻塞
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if(number < 0 && errno != EINTR)  // 失败返回-1
        {
            printf( "epoll failure\n" );
            break;
        }

        for( int i = 0; i < number; i++ )
        {
            int sockfd = events[i].data.fd;
            if( sockfd == listenfd )    // 有新客户接入
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                if ( connfd < 0 )
                {
                    printf( "errno is: %d\n", errno );
                    continue;
                }
                if( http_conn::m_user_count >= MAX_FD )
                {
                    show_error( connfd, "Internal server busy" );
                    continue;
                }
                
                pUsers[connfd].init( connfd, client_address );
            }
            else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) )  // 有异常
            {
                pUsers[sockfd].close_conn();
            }
            else if( events[i].events & EPOLLIN )   // 读事件
            {
                if(pUsers[sockfd].read())
                {
                    pThreadPool->append(pUsers + sockfd);   // 直接将pUsers+sockfd作为指针
                }
                else
                {
                    pUsers[sockfd].close_conn();    // 读失败，关闭连接
                }
            }
            else if( events[i].events & EPOLLOUT )  // 写事件
            {
                if(false == pUsers[sockfd].write())
                {
                    pUsers[sockfd].close_conn();    // 写失败，关闭连接
                }
            }
            else
            {

            }
        }
    }

    close(epollfd);
    close(listenfd);
    delete[] pUsers;
    delete pThreadPool;
    return 0;
}
