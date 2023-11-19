#include "head.h"
#include "thRead.h"
#include "md5.h"
#include "Function.h"
#include "MysqlFunc.h"
int exitPipe[2]; // 实现异步拉取同步
// 信号函数
void sigFunc(int sigNum)
{
    printf("sigNum = %d\n", sigNum);
    write(exitPipe[1], "1", 1); // 随便输入就行，目的为了使exitPipe[0]就绪
    printf("process is going to die!\n");
}

// md5
const char *file_path;
char md5_str[MD5_STR_LEN + 1];
int setFileMd5(const char *path)
{
    file_path = path;
}

int main(int argc, char *argv[]) //.server ../conf/server.conf
{

    pipe(exitPipe); // 穿管无名管道
    if (fork() != 0)
    {
        // 父进程，先注册个信号，等待子进程终止，最后自己终止
        close(exitPipe[0]);
        signal(SIGUSR1, sigFunc);
        wait(NULL); // 等待退出
        exit(0);    // 子进程全部退出完毕自己退出
    }
    // 子进程开辟线程池
    close(exitPipe[1]);
    char ip[16], port[10];
    ObtainIpAndPort(argv[1], ip, port); // 获取ip和port
    int workNum = 10;
    Thread thrPool;                // 定义线程池，为线程池的任务队列申请内存等
    InitThread(&thrPool, workNum); // 初始化线程池的相关操作
    makeChild(&thrPool);           // 创建若干个子线程
    int socFd;
    // 等待客户端连接
    int optval = 1;
    tcpInit(&socFd, ip, port);
    setsockopt(socFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    int epfd = epoll_create(1);  // 大于0就行
    epollAdd(socFd, epfd);       // 把客户端监听
    epollAdd(exitPipe[0], epfd); // 把无名管道监听
    struct epoll_event readArr[10];
    int netFd;
    while (1)
    {
        int readNum = epoll_wait(epfd, readArr, 10, -1);
        for (int i = 0; i < readNum; ++i)
        {
            if (readArr[i].data.fd == socFd)
            {
                // 先accept
                netFd = accept(socFd, NULL, NULL); 
                cout << "connect" << endl;
                epollAdd(netFd, epfd);
               
            }
            else if (readArr[i].data.fd == exitPipe[0])
            {
                // 表示要退出了
                thrPool.exitFlag = 1;                        // 变标志位1
                pthread_cond_broadcast(&thrPool.tasks.cond); // 广播
                for (int j = 0; j < thrPool.pthNum; ++j)
                {
                    pthread_join(thrPool.tid[j], NULL); // 等待回收
                }
                pthread_exit(NULL); // 子线程全部关闭，主线程退出
            }
            else
            {
                // netfd
                int len;
                int stat;
                char buf[1024] = {0};
                recvn(netFd, &len, sizeof(int)); // xiainhuoqu
                recvn(netFd, &stat, sizeof(int));
                cout << "stat=" << stat << endl;
                recvn(netFd, buf, len);
                cout << "name=" << buf << endl;
                bool rest = (stat == CHECKNAME);
                cout << rest << endl;
                cout<<"继续";
                /***********************错误*********************************/
                // if (stat == CHECKNAME)
                // {
                //     // 数据库
                //     int res = checkName(buf);
                //     cout << "查重：" << res << endl;
                //     if (res == -1)
                //     {
                //         send(netFd, &res, sizeof(int), 0); // 表示名字可用
                //         char usrname[1024] = {0};
                //         strcpy(usrname, buf); // 存储 name;
                //         bzero(buf, sizeof(buf));
                //         len = 0;
                //         recvn(netFd, &len, sizeof(int)); // 先获取长度
                //         recvn(netFd, &stat, sizeof(int));
                //         recvn(netFd, buf, len);
                //         cout << buf << endl;
                //         if (stat == REGISTER)
                //         {
                //             // 注册
                //             // 创建 salt
                //             char *randStr = getSalt();
                //             char salt[128] = {0};
                //             sprintf(salt, "$6$%s$", randStr);
                //             // 生成密文
                //             char *cryptPwd = crypt(buf, salt);
                //             // 存储到数据库
                //             saveServerInfo(usrname, salt, cryptPwd, 0);
                //             // 存入数据库文件表
                //             //  dataInsert(0,NULL,);
                //             // 生成token   token 存入数据库并发送客户端
                //             char *tokStr = getSalt();
                //             char token[128] = {0};
                //             sprintf(token, "$token$%s$%s", usrname, tokStr);
                //             // 存入数据库
                //             saveServerToken(usrname, token);
                //             // 发送token
                //             Train stat_t;
                //             stat_t.len = strlen(token) + 1;
                //             strcpy(stat_t.buf, token);
                //             send(netFd, &stat_t, sizeof(stat_t.len) + stat_t.len, 0);
                //         }
                //     }
                //     else
                //     {
                //         send(netFd, &res, sizeof(int), 0); // no
                //     }
                // }
            }
        }
    }
}
