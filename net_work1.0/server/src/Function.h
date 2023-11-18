#pragma once
#include"head.h"

enum {
    REGISTER,
    LOGIN,
    CHECKNAME,
    LS,
    CD,
    PWD,
    REMOVE,
    GETS,
    PUTS,
    TOKEN,
};
//带有状态码的火车
typedef struct{
    int data_len;//存放buf，也就是火车车厢中数据长度
    int state;//状态码
    char buf[1000];//火车车厢长度
}trainState;

//小火车（不带状态码的）
typedef struct train_s{
    int len;
    char buf[128];
}Train;
//用户相关的信息
typedef struct User_S{
    char name[128];
    char pwd[128];
    char token[128];
}user;
//epoll相关
int epollAdd(int fd,int epfd);
int epollDel(int fd,int epfd);
//菜单
void menuShow();

int Login();
int recvn(int sockFd,void *pstart,int len);
int doLs();
int doCd();
int doPwd();
int doRm();
int doMkdir();

int tcpInit(int *socFd,char*ip,char*port);
int ObtainIpAndPort(char *path, char *ip, char *port);
static void ftp_arg_set(char *buf, char *arg);
int doGets();
int doPuts();
int transFile(int netFd, string cmd,string name,string md5);

// vectcor<string>profile;//dinghyi  wenjian ming
typedef struct pro_s{
    char profile[256];
    pro_s*next; 
}profie,*ProFile;