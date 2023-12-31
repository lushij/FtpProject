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
    printf("******************\n");
    printf("******1.登录******\n");
    printf("******2.注册******\n");
    printf("******3.功能******\n");
    printf("******0.退出******\n");
    printf("******************\n");
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


int recvFile(int sockFd){
    // //先获取文件名
    // char name[1024] = {0};
    int dataLength;
    // int ret = recvn(sockFd,&dataLength,sizeof(int));
    // ERROR_CHECK(ret,-1,"recv");
    // ret = recvn(sockFd,name,dataLength);
    // ERROR_CHECK(ret,-1,"recv");
    // int fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    // ERROR_CHECK(fd,-1,"open");
    int fileSize;
    recvn(sockFd,&dataLength,sizeof(int));
    recvn(sockFd,&fileSize,dataLength);
    printf("fileSize = %d\n", fileSize);
    char buf[1000] = {0};
    time_t timeBeg,timeEnd;
    timeBeg = time(NULL);
    int pipefds[2];
    pipe(pipefds);
    int total = 0;
    while(total < fileSize){
        int ret = splice(sockFd,NULL,pipefds[1],NULL,4096,SPLICE_F_MORE);
        total += ret;
        // splice(pipefds[0],NULL,fd,NULL,ret,SPLICE_F_MORE);
    }
    recvn(sockFd,&dataLength,sizeof(int));
    printf("dataLength = %d\n", dataLength);
    timeEnd = time(NULL);
    printf("total time = %ld\n", timeEnd - timeBeg);
}