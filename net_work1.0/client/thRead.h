#include"head.h"
// typedef struct cmd_s{
//     int netFd;
//     string cmd;
// }cmd_t;

typedef struct task_s{
    queue<int>task;//renwuduilie
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int size;
}Task;

typedef struct threadPool_s{
    pthread_t *tid;//dingyi
    Task tasks;
    int pthNum;
    int exitFlag;
    char *cmd;
}Thread;

int InitThread(Thread*pTh,int workNum);
int EnQueue(Task*tasks,int netFd);
int DeQueue(Task*tasks);
int makeChild(Thread*pThread);