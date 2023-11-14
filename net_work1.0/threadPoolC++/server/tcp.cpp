#include"work.h"
//socket bind listen 
int tcpInit(int *socFd,char*ip,char*port)
{
    *socFd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(port));
    addr.sin_addr.s_addr=inet_addr(ip);
    bind(*socFd,(struct sockaddr*)&addr,sizeof(addr));
    listen(*socFd,10);
}