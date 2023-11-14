#include"work.h"
int epollAdd(int fd,int epFd)
{
    struct epoll_event evs;
    evs.data.fd=fd;
    evs.events=EPOLLIN;
    int ret = epoll_ctl(epFd,EPOLL_CTL_ADD,fd,&evs);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}

int epollDel(int fd,int epFd)
{
    struct epoll_event evs;
    evs.data.fd=fd;
    evs.events=EPOLLIN;
    int ret = epoll_ctl(epFd,EPOLL_CTL_DEL,fd,&evs);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}