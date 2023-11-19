#include "Function.h"
#define STR_LEN 8
char str[STR_LEN + 1] = {0};
int epollAdd(int fd, int epfd)
{
    struct epoll_event evs;
    evs.data.fd = fd;
    evs.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evs);
    ERROR_CHECK(ret, -1, "epoll_ctl");
}

int epollDel(int fd, int epfd)
{
    struct epoll_event evs;
    evs.data.fd = fd;
    evs.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evs);
    ERROR_CHECK(ret, -1, "epoll_ctl");
}

int recvn(int sockFd, void *pstart, int len)
{
    int total = 0;
    int ret;
    char *p = (char *)pstart;
    while (total < len)
    {
        ret = recv(sockFd, p + total, len - total, 0);
        total += ret;
    }
    return 0;
}

// chanshengsalt
char *getSalt()
{
   
    int i, flag;
    srand(time(NULL));
    for (i = 0; i < STR_LEN; i++)
    {
        flag = rand() % 3;
        switch (flag)
        {
        case 0:
            str[i] = rand() % 26 + 'a';
            break;
        case 1:
            str[i] = rand() % 26 + 'A';
            break;
        case 2:
            str[i] = rand() % 10 + '0';
            break;
        }
    }
    // printf("%s\n", str); // 输出生成的随机数。
    return str;
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

int transFile(int socFd, string cmd,string name,string md5)
{
    if(cmd == "gets"){//下载
        const char*path=name.c_str();//zhuanhau
        int fd = open(path,O_RDWR);
     //打开文件
        ERROR_CHECK(fd,-1,"open");
        train_s train;
        train.len=4;
        strcpy(train.buf,"file");
        int ret = send(socFd,&train,sizeof(train.len)+train.len,MSG_NOSIGNAL);//注意：要把整个小火车都传过去
        ERROR_CHECK(ret,-1,"send");

        struct stat st;
        ret = fstat(fd,&st);
        ERROR_CHECK(ret,-1,"fstat");
        //  开始修改，大文件加进度条
        int fileSize = st.st_size;//强转为int型
        train.len=4;
        //这样就要进行传送二进制文件了
        memcpy(train.buf,&fileSize,sizeof(int));
        ret = send(socFd,&train,sizeof(train.len)+train.len,MSG_NOSIGNAL);//注意：要把整个小火车都传过去
        ERROR_CHECK(ret,-1,"send");
        while(1)//xianduwenjian
        {
            bzero(&train,sizeof(train));
            ret=read(fd,train.buf,sizeof(train.buf));//每次只能读取train.buf个字节，只能少不能多
            if(ret == 0){
                break;
            }
            train.len=ret;
            send(socFd,&train,sizeof(train.len)+train.len,MSG_NOSIGNAL);
        }
        train.len=0;//最后发个0表示发完了，接收方可以根据长度为0进行退出接收
        send(socFd,&train,sizeof(train.len)+train.len,MSG_NOSIGNAL);
        close(fd);


    }else{//上传
        //jiinxingshujukuduibile
        //chaxunshujukude md5shifouyiyang

    }
}