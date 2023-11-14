#pragma once//防止多次重复定义
#include"head.h"
typedef struct task_s{
    queue<int>queNetFd;//队列存储netFd
    pthread_mutex_t muxtex;//锁
    pthread_cond_t cond;//条件变量
    int size;//对列的长度
}taskQue;

typedef struct threadPool_s{
    pthread_t*tid;//子线程的数组
    int pthNum;//子线程的数量
    taskQue tql;//任务队列
    int exitFlag;//退出标志
}threadPool;

//初始化
void initThr(threadPool*pThr,int WorkNum);
//我这个是pth总的参数
void EnQue(taskQue*pTql,int netFd);
void DeQue(taskQue*pTql);