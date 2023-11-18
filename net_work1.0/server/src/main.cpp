#include "head.h"
#include "thRead.h"
#include "md5.h"
#include "Function.h"
#include "MysqlFunc.h"
int exitPipe[2];//实现异步拉取同步
//信号函数
void sigFunc(int sigNum)
{
    printf("sigNum = %d\n",sigNum);
    write(exitPipe[1],"1",1);//随便输入就行，目的为了使exitPipe[0]就绪
    printf("process is going to die!\n");
}

//md5 
const char *file_path;
char md5_str[MD5_STR_LEN + 1];
int setFileMd5(const char *path)
{
    file_path = path;
}

int main(int argc, char *argv[]) //.server ../conf/server.conf
{


    pipe(exitPipe);//穿管无名管道
    if(fork()!=0)
    {
        //父进程，先注册个信号，等待子进程终止，最后自己终止
        close(exitPipe[0]);
        signal(SIGUSR1,sigFunc);
        wait(NULL);//等待退出
        exit(0);//子进程全部退出完毕自己退出
    }
    //子进程开辟线程池
    close(exitPipe[1]);
    char ip[16], port[10];
    ObtainIpAndPort(argv[1], ip, port); // 获取ip和port
    int workNum=10;
    Thread thrPool;//定义线程池，为线程池的任务队列申请内存等
    InitThread(&thrPool,workNum);//初始化线程池的相关操作
    makeChild(&thrPool);//创建若干个子线程
    int socFd;
    //等待客户端连接
    tcpInit(&socFd,ip,port);
    int epfd=epoll_create(1);//大于0就行
    epollAdd(socFd,epfd);//把客户端监听
    epollAdd(exitPipe[0],epfd);//把无名管道监听
    struct epoll_event readArr[10];
    while(1)
    {
        int readNum=epoll_wait(epfd,readArr,10,-1);
        for(int i=0;i<readNum;++i)
        {
            if(readArr[i].data.fd==socFd)
            {
                //先accept
                int netFd=accept(socFd,NULL,NULL);
                int len;
                int stat;
                char buf[1024]={0};
                recvn(netFd,&len,sizeof(int));//xiainhuoqu
                recvn(netFd,&stat,sizeof(int));
                recvn(netFd,buf,len);
                if(stat==CHECKNAME)
                {
                    //shujuku
                    int res = checkName (buf);
                    if(res == -1){
                        send(netFd,&res,sizeof(int),0);//biaoshigaimingzikeyong
                        char usrname[1024]={0};
                        strcpy(usrname,buf);//cunchu name;
                        bzero(buf,sizeof(buf));
                        len = 0;
                        recvn(netFd,&len,sizeof(int));//xiainhuoqu
                        recvn(netFd,&stat,sizeof(int));
                        recvn(netFd,buf,len);
                        if(stat == REGISTER)
                        {
                            //zhuce
                            //chansheng salt
                            char *salt=getSalt();
                            //sehngcehng miwen
                            char *cryptPwd=crypt(buf,salt);
                            //cunru shujuku
                            saveServerInfo(usrname,salt,cryptPwd,0);
                            //cunru shujuku wenjianbiao
                            // dataInsert(0,NULL,);
                            //shengchegg   token baocunshujuku biingfasoagkehuduan
                        }
                    }
                    else
                    {
                        send(netFd,&res,sizeof(int),0);//no
                    }
                }
                if(stat == LOGIN)
                {
                    //denglu 

                    char*pwd;
                    strcpy(pwd,buf);
                    bzero(buf,sizeof(buf));
                    len =0;
                    recvn(netFd,&len,sizeof(int));
                    recvn(netFd,buf,len);//shoude shiyonghuming
                    //huoqu salt(shujuku)
                    char *salt;
                    checkAddress(buf,salt);
                    char*crypwd=crypt(pwd,salt);
                    //duibi shujuku
                    int ret = checkPwd(buf,crypwd); // 0 biaoshi chenggoong
                    send(netFd,&ret,sizeof(int),0);


                }
                
                if(stat == LS)
                {
                    //zhijieshujukucaozuo

                }
                if(stat == CD)
                {

                }
                if(stat == PWD)
                {

                }
                if(stat == REMOVE)
                {

                }
                if(stat == TOKEN)
                {

                }


                //XIANCHENGCVHI
                if(stat == GETS)
                {

                }
                if(stat == PUTS)
                {
                    //shangcahun
                    //cishi buf nei cunru de shi md5
                    //jinxing chashi
                    int ret = checkMd5(buf);
                    if(ret == -1)
                    {
                        //kaishishangchaun
                    }
                    else
                    {
                        //miaochuan
                        send(netFd,&ret,sizeof(int),0);
                    }

                }


















            //     //再加锁
            //     pthread_mutex_lock(&thrPool.tql.muxtex);
            //     //把netFd入队
            //     EnQue(&thrPool.tql,netFd);
            //     // thrPool.tql.queNetFd.push(netFd);
            //     // ++thrPool.tql.size;
            //     printf("new Task\n");
            //     //通知
            //     pthread_cond_signal(&thrPool.tql.cond);
            //     //解锁
            //     pthread_mutex_unlock(&thrPool.tql.muxtex);
            // }
            else if(readArr[i].data.fd==exitPipe[0])
            {
                //表示要退出了
                thrPool.exitFlag=1;//变标志位1
                pthread_cond_broadcast(&thrPool.tasks.cond);//广播
                for(int j=0;j<thrPool.pthNum;++j)
                {
                    pthread_join(thrPool.tid[j],NULL);//等待回收
                }
                pthread_exit(NULL);//子线程全部关闭，主线程退出
            }
        }
    }
}
}