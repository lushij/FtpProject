#include "head.h"
#include "md5.h"
#include "Function.h"
#include"thRead.h"

// unordered_map<string,string>usrToken;//存放usrname和token
/***********************************/
// 设置md5的文件相关路径
const char *file_path;
char md5_str[MD5_STR_LEN + 1];
int setFileMd5(const char *path)
{
    file_path = path;
}
/***********************************/



int main(int argc, char *argv[])
{
    vector<char *> usrName;
    unordered_map<string, string> usrToken; // 存放usrname和token
    //./client 192.168.0.158 2000
    ARGS_CHECK(argc, 3);
    // 先连接 客户端 socket connect
    int socFd, ret;
    ERROR_CHECK(socFd, -1, "socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    // int ret = connect(socFd, (struct sockaddr *)&addr, sizeof(addr));
    // ERROR_CHECK(ret, -1, "connect");
    int epfd = epoll_create(1);
    // epollAdd(socFd, epfd);
    epollAdd(STDIN_FILENO, epfd);
    struct epoll_event readArr[3];
    char buf[1024] = {0};
    Thread pThread;//DINGYIXIANCHENGCHI
    InitThread(&pThread,10);
    makeChild(&pThread);
    Train train_t; // 定义小火车
login_begin:
    // 菜单
    system("clear");
    void menuShow();
    int cmd = 0;

    scanf("%d", &cmd);
    if (cmd == 1)
    {
        // 登录
    }
    else if (cmd == 2)
    {
        // 注册
        cout << "根据提示完成注册" << endl;
        char username[128] = {0};
    recin:
        char *userpwd1;
        char *userpwd2;
        cout << "username:";
        cin >> username;
        for (auto ch : usrName)
        {
            if (strcmp(ch, username))
            {
                cout << "该用户已存在,请重新输入" << endl;
                bzero(username, sizeof(username));
                system("clear");
                goto recin;
            }
        }
        userpwd1 = getpass("userpwd:");
        userpwd2 = getpass("Please re-enter your userpwd:");
        if (strcmp(userpwd1, userpwd2) != 0)
        {
            // 注册失败
            printf("两次输入不一致，即将回到主界面\n");
            sleep(1);
            goto login_begin;
        }
        else
        {
            // 注册成功
            // user usr;
            // strcpy(usr.name, username);
            // strcpy(usr.pwd, userpwd1);
            // UserInfo.push_back(usr); // 存进数组中
            usrName.push_back(username);
            socFd = socket(AF_INET, SOCK_STREAM, 0);
            ret = connect(socFd, (struct sockaddr *)&addr, sizeof(addr));
            ERROR_CHECK(ret, -1, "connect");
            // 传递用户名
            train_t.len = strlen(username) + 1;
            strcpy(train_t.buf, username);
            send(socFd, &train_t, sizeof(train_t.len) + train_t.len, 0);
            // 传递密码用带有状态的火车协议，对方收三次
            trainState tStat;
            tStat.state = REGISTER;
            tStat.data_len = strlen(userpwd1) + 1;
            strcpy(tStat.buf, userpwd1);
            send(socFd, &tStat, sizeof(tStat.data_len) + tStat.data_len + sizeof(tStat.state), 0);
            // 接收token
            int lenth;
            char token[128] = {0};
            recvn(socFd, &lenth, sizeof(int));
            recvn(socFd, token, lenth);
            // 存储token
            string stoken(token); // 转化为string类型
            usrToken[username] = stoken;
        }
    }
    else if (cmd == 3)
    {
        // 功能
    }
    else
    {
        // 退出
        exit(0);
    }
    epollAdd(socFd, epfd);
    while (1)
    {
        int readNum = epoll_wait(epfd, readArr, 3, -1);
        for (int i = 0; i < readNum; i++)
        {
            if (readArr[i].data.fd == STDIN_FILENO)
            {
                bzero(buf, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                // 需要进行解析命令和长短命令分离
                // 短命令，操作的还是
                if (strncmp(buf, "ls", 2) == 0)
                {
                    // doLs();
                    bzero(&train_t,sizeof(train_t));
                    train_t.len=2;
                    strcpy(train_t.buf,"ls");
                    send(socFd,&train_t,sizeof(train_t),0);
                }
                if (strncmp(buf, "cd", 2) == 0)
                {
                    // doCd();
                    bzero(&train_t,sizeof(train_t));
                    train_t.len=2;
                    strcpy(train_t.buf,"cd");
                    send(socFd,&train_t,sizeof(train_t),0);
                }
                if (strncmp(buf, "pwd", 3) == 0)
                {
                    // doPwd();
                    bzero(&train_t,sizeof(train_t));
                    train_t.len=3;
                    strcpy(train_t.buf,"pwd");
                    send(socFd,&train_t,sizeof(train_t),0);
                }
                if (strncmp(buf, "rm", 2) == 0)
                {
                    // doRm();
                    bzero(&train_t,sizeof(train_t));
                    train_t.len=2;
                    strcpy(train_t.buf,"rm");
                    send(socFd,&train_t,sizeof(train_t),0);
                }
                if (strncmp(buf, "mkdir", 5) == 0)
                {
                    // doMkdir();
                    bzero(&train_t,sizeof(train_t));
                    train_t.len=5;
                    strcpy(train_t.buf,"mkdir");
                    send(socFd,&train_t,sizeof(train_t),0);
                }
                if (strncmp(buf, "gets", 4) == 0)
                {
                    // 子线程
                    // doGets();
                    // pthread_create();
                     //先accept
                    int netFd=accept(socFd,NULL,NULL);
                    //再加锁
                    pthread_mutex_lock(&pThread.tasks.mutex);
                    //把netFd入队
                    EnQueue(&pThread.tasks,netFd);
                    printf("new Task\n");
                    pThread.cmd="gets";
                    //通知
                    pthread_cond_signal(&pThread.tasks.cond);
                    //解锁
                    pthread_mutex_unlock(&pThread.tasks.mutex);
                }
                if (strncmp(buf, "puts", 4) == 0)
                {
                    // 子线程
                    // doPuts();
                }
            }
            if (readArr[i].data.fd == socFd)
            {
                // 接收信息
            }
        }
    }

//MD5
    setFileMd5(argv[1]);
    ret = Compute_file_md5(file_path, md5_str);
    // md5_str存放的就是MD5值；
    if (0 == ret)
    {
        printf("%s\t%s\n", md5_str, file_path);
    }
    return 0;
}