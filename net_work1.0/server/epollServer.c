#include<func.h>
int main(int argc,char*argv[])
{
    //fuwuduan socket setsockopt bind listen accept
    ARGC_CHECK(argc,3);
    int socFd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(argv[2]));
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    int optval=1;
    int ret = setsockopt(socFd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret = bind(socFd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    ret = listen(socFd,10);
    int netFd=-1;
    int epfd=epoll_create(1);
    //epoll 1.struct epoll_event event,2.event... 3.epoll_ctl 4. epoll_wait
    struct epoll_event event;
    event.data.fd=STDIN_FILENO;
    event.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    struct epoll_event readyArr[10];
    event.data.fd=socFd;
    event.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,socFd,&event);
    char buf[4096]={0};
    while(1);
    while(1)
    {
        int readNum = epoll_wait(epfd,readyArr,11,-1);
        for(int i=0;i<readNum;i++)
        {
            if(readyArr[i].data.fd == socFd)
            {
                netFd=accept(socFd,NULL,NULL);
                event.data.fd=netFd;
                event.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,netFd,&event);
            }
            if(readyArr[i].data.fd==STDIN_FILENO)
            {
                bzero(buf,sizeof(buf));
                ret  = read(STDIN_FILENO,buf,sizeof(buf));
                if(ret == 0)
                {
                    goto end;
                }
                send(netFd,buf,ret,0);
            }
            if(readyArr[i].data.fd=netFd)
            {
                bzero(buf,sizeof(buf));
                ret = recv(netFd,buf,sizeof(buf),0);
                if(ret == 0)
                {
                    goto end;
                }
                puts(buf);
            }
        }
        
    }
    end:
    close(socFd);
    close(netFd);

}