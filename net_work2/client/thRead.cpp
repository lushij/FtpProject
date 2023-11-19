#include"thRead.h"
#include"Function.h"
int InitThread(Thread*pTh,int workNum)
{
    pTh->exitFlag=0;
    pTh->pthNum=workNum;
    pTh->tid=(pthread_t*)calloc(workNum,sizeof(Thread));
    pTh->tasks.size=0;
    // pTh->cmd={0};
    // pTh->name={0};
    // pTh->tasks.task;
    pthread_mutex_init(&pTh->tasks.mutex,NULL);
    pthread_cond_init(&pTh->tasks.cond,NULL);
}
int EnQueue(Task*tasks,cmd_t cmd)
{   
    tasks->task.push(cmd);
    ++tasks->size;
}
int DeQueue(Task*tasks)
{
    tasks->task.pop();
    --tasks->size;
}

//clean资源清理函数定义
void cleanFunc(void *arg)
{
    Thread* pth=(Thread*)arg;
    pthread_mutex_unlock(&pth->tasks.mutex);
}
void *handel(void*arg)
{
    Thread*pth=(Thread*)arg;
    while(1)
    {
        int socFd;
        string cmd;
        string name;
        string md5;
        while (1)
        {
            // printf("I am free ,tid = %lu\n",pthread_self());
            //先加锁
            pthread_mutex_lock(&pth->tasks.mutex);
            pthread_cleanup_push(cleanFunc,(void*)pth);//定义符合要求的资源清理函数
            while (pth->tasks.size == 0 && pth->exitFlag==0)
            {
            pthread_cond_wait(&pth->tasks.cond,&pth->tasks.mutex);//此函数内部有个队列，要是退出此函数会自动解锁
            }
            //现在子线程苏醒了
            socFd=pth->tasks.task.front().netFd;//拿到队头的netFd
            cmd = pth->tasks.task.front().cmd;
            name=pth->tasks.task.front().name;
            md5=pth->tasks.task.front().md5;
            DeQueue(&pth->tasks);//拿到后就弹出该元素
            pthread_cleanup_pop(1);//这个必须和pthread_cleanup_push成对出现，因为内部是个{}，否则会出现错误
            printf("I am Work ,tid = %lu\n",pthread_self());
            transFile(socFd,cmd,name,md5);
            printf("done\n");
            close(socFd);
        }

    }
}
int makeChild(Thread*pThread)
{
    for(int i=0;i<pThread->pthNum;++i)
    {
        pthread_create(&pThread->tid[i],NULL,handel,(void*)pThread);
    }
}

