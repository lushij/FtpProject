#include"head.h"
typedef struct cmd_s{
    int netFd;
    string cmd;
    string name;
    string md5;
}cmd_t;

typedef struct task_s{
    queue<cmd_t>task;//renwuduilie
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int size;
}Task;

typedef struct threadPool_s{
    pthread_t *tid;//dingyi
    Task tasks;
    int pthNum;
    int exitFlag;
}Thread;

int InitThread(Thread*pTh,int workNum);
int EnQueue(Task*tasks,cmd_t cmd);
int DeQueue(Task*tasks);
int makeChild(Thread*pThread);