#include"work.h"
//clean资源清理函数定义
void cleanFunc(void *arg)
{
    threadPool* pth=(threadPool*)arg;
    pthread_mutex_unlock(&pth->tql.muxtex);
}


void* handle(void*arg)//传参，子线程函数
{
    //首先强转
    threadPool*pth=(threadPool*)arg;
    int netFd;
    while (1)
    {
        printf("I am free ,tid = %lu\n",pthread_self());
        //先加锁
        pthread_mutex_lock(&pth->tql.muxtex);
        pthread_cleanup_push(cleanFunc,(void*)pth);//定义符合要求的资源清理函数
        while (pth->tql.size == 0 && pth->exitFlag==0)
        {
           pthread_cond_wait(&pth->tql.cond,&pth->tql.muxtex);//此函数内部有个队列，要是退出此函数会自动解锁
        }
        //现在子线程苏醒了
        netFd=pth->tql.queNetFd.front();//拿到队头的netFd
        DeQue(&pth->tql);//拿到后就弹出该元素
        pthread_cleanup_pop(1);//这个必须和pthread_cleanup_push成对出现，因为内部是个{}，否则会出现错误
        printf("I am Work ,tid = %lu\n",pthread_self());
        transFile(netFd);
        printf("done\n");
        close(netFd);
    }

}

int makeChild(threadPool*pthrPool)//创建若干个子线程
{
    for(int i=0;i<pthrPool->pthNum;i++)
    {
        pthread_create(&pthrPool->tid[i],NULL,handle,(void*)pthrPool);
    }
}