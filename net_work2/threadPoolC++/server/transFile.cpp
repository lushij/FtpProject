#include"work.h"

int transFile(int netFd)
{
    //打开文件
    int fd=open("file",O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    train_t train;
    train.length=4;
    strcpy(train.buf,"file");
    int ret = send(netFd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);//注意：要把整个小火车都传过去
    ERROR_CHECK(ret,-1,"send");

    struct stat st;
    ret = fstat(fd,&st);
    ERROR_CHECK(ret,-1,"fstat");
    //  开始修改，大文件加进度条
    int fileSize = st.st_size;//强转为int型
    train.length=4;
    //这样就要进行传送二进制文件了
    memcpy(train.buf,&fileSize,sizeof(int));
    ret = send(netFd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);//注意：要把整个小火车都传过去
    ERROR_CHECK(ret,-1,"send");
    while(1)//xianduwenjian
    {
        bzero(&train,sizeof(train));
        ret=read(fd,train.buf,sizeof(train.buf));//每次只能读取train.buf个字节，只能少不能多
        if(ret == 0){
            break;
        }
        train.length=ret;
        send(netFd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    }
    train.length=0;//最后发个0表示发完了，接收方可以根据长度为0进行退出接收
    send(netFd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    close(fd);
}