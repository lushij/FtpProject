#include"workQue.h"
//任务队列的相关操作
void initThr(threadPool*pThr,int WorkNum)
{
    pThr->pthNum=WorkNum;
    pThr->tid=(pthread_t*)calloc(WorkNum,sizeof(threadPool)); 
    pThr->tql.size=0;
    pthread_mutex_init(&pThr->tql.muxtex,NULL);
    pthread_cond_init(&pThr->tql.cond,NULL);
    pThr->exitFlag=0;
}

void EnQue(taskQue*pTql,int netFd)
{
    pTql->queNetFd.push(netFd);
    ++pTql->size;
}
void DeQue(taskQue*pTql)
{
    pTql->queNetFd.pop();
    --pTql->size;
}