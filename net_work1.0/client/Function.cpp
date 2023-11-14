#include"Function.h"
int epollAdd(int fd,int epfd)
{
    struct epoll_event evs;
    evs.data.fd=fd;
    evs.events=EPOLLIN;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&evs);
    ERROR_CHECK(ret,-1,"epoll_ctl");
}

int epollDel(int fd,int epfd)
{
    struct epoll_event evs;
    evs.data.fd=fd;
    evs.events=EPOLLIN;
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&evs);
    ERROR_CHECK(ret,-1,"epoll_ctl");
}

void menuShow()
{
    printf("/******************/\n");
    printf("/*****1. 登录******/\n");
    printf("/*****2. 注册******/\n");
    printf("/*****3. 功能******/\n");
    printf("/*****0. 退出******/\n");
    printf("/******************/\n");
}


int Login()
{
    char username[128]={0};
    char userpwd[128]={0};
    cout<<"userName:";
    cin>>username;
    cout<<"userPwd:";
    cin>>userpwd;
    
}
int recvn(int sockFd,void *pstart,int len)
{
    int total = 0;
    int ret;
    char *p = (char *)pstart;
    while(total < len){
        ret = recv(sockFd,p+total,len-total,0);
        total += ret;
    }
    return 0;
}


int doLs()
{

}
int doCd()
{

}
int doPwd()
{

}
int doRm()
{

}
int doMkdir()
{

}

int doGets()
{

}
int doPuts()
{
    
}

int transFile(int netFd, char*cmd)
{
    
}