#pragma once
#include"head.h"
#include"workQue.h"
int ObtainIpAndPort(char *path, char *ip, char *port);
static void ftp_arg_set(char *buf, char *arg);
int makeChild(threadPool*thrPool);//创建若干个子线程
void* handle(void*arg);
int epollAdd(int fd,int epFd);
int epollDel(int fd,int epFd);
int tcpInit(int *socFd,char*ip,char*port);
int transFile(int netFd);
typedef struct train_s{
    int length;
    char buf[1000];
} train_t;