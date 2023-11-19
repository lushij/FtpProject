#include "head.h"
// #include "md5.h"
#include "Function.h"
// #include "thRead.h"
#include"MysqlFunc.h"
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
    int putsSocFd,getsSocFd;
    
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
    menuShow();
    int cmd = 0;
    string usrname;
    string userpwd1,userpwd2;
    scanf("%d", &cmd);
    if (cmd == 1)
    {
   
        // 登录
        // string userpwd1;
        cout << "username:";
        // string username;
        cin >> usrname;
         
    rePwd:
            userpwd1 = getpass("userpwd:");
            //连接
            // 传递密码用带有状态的火车协议，对方收三次
            socFd = socket(AF_INET, SOCK_STREAM, 0);
            ret = connect(socFd, (struct sockaddr *)&addr, sizeof(addr));
            ERROR_CHECK(ret, -1, "connect");
            //先发送密码
            trainState tStat;
            tStat.state = LOGIN;
            tStat.data_len = userpwd1.size();
            strcpy(tStat.buf, userpwd1.c_str());
            send(socFd, &tStat, sizeof(tStat.data_len) + tStat.data_len + sizeof(tStat.state), 0);
            //发送name
            train_t.len=usrname.size();
            strcpy(train_t.buf,usrname.c_str());
            send(socFd, &train_t, sizeof(train_t.len) + tStat.data_len , 0);
            int res;
            recvn(socFd,&res,sizeof(int));
            if(res == 0)
            {
                cout<<"登陆成功"<<endl;
            }else{
                cout<<"密码错误,请重新输入"<<endl;
                bzero(&userpwd1[0],userpwd1.size());
                goto  rePwd;
            }
       

        

    }
    else if (cmd == 2)
    {
        // 注册
        cout << "根据提示完成注册" << endl;
reName:
        cout << "username:";
        cin >> usrname;
        //conghsujuku chaxun youchongfumingzi
        // 传递用户名 jinxingcahchng
        socFd = socket(AF_INET, SOCK_STREAM, 0);
        ret = connect(socFd, (struct sockaddr *)&addr, sizeof(addr));
        ERROR_CHECK(ret, -1, "connect");
        trainState tStat;
        tStat.state = CHECKNAME;
        tStat.data_len = usrname.size();
        strcpy(tStat.buf, usrname.c_str());
        send(socFd, &tStat, sizeof(tStat.data_len) + tStat.data_len + sizeof(tStat.state), 0);
        char retVal[128]={0};
        int res;
        recvn(socFd,&res,sizeof(int));
         //-1 daibiao buchongfu
        if(res == -1)
        {
            userpwd1 = getpass("userpwd:");
            userpwd2=userpwd1;
            cout<<userpwd2<<endl;
            bzero(&userpwd1[0],userpwd1.size());
            userpwd1 = getpass("Please re-enter your userpwd:");
            cout<<userpwd1<<endl;
            /*********************************/
            if (userpwd1!=userpwd2)
            {
                // cout<<ret<<endl;
                // 注册失败
                printf("注册失败\n");
                printf("两次输入不一致，即将回到主界面\n");
                bzero(&usrname[0],usrname.size());
                bzero(&userpwd1[0],userpwd1.size());
                bzero(&userpwd2[0],userpwd2.size());
                sleep(2);
                goto login_begin;
            }
            else
            {
                // 注册成功
                cout<<"注册成功"<<endl;
                // usrName.push_back(username);//进行查重 想法应该错了，这个一次性，应该数据库
                // char*name=(char*)usrname.c_str();
                // UsrAcepdata(name);
                // // 传递用户名
                // train_t.len = usrname.size();
                // strcpy(train_t.buf, usrname.c_str());
                // send(socFd, &train_t, sizeof(train_t.len) + train_t.len, 0);
                // 传递密码用带有状态的火车协议，对方收三次
                trainState tStat;
                tStat.state = REGISTER;
                tStat.data_len = userpwd1.size();
                strcpy(tStat.buf, userpwd1.c_str());
                send(socFd, &tStat, sizeof(tStat.data_len) + tStat.data_len + sizeof(tStat.state), 0);
                // 接收token
                int lenth;
                char token[128] = {0};
                recvn(socFd, &lenth, sizeof(int));
                recvn(socFd, token, lenth);
                // 存储token
                string stoken(token); // 转化为string类型
                usrToken[usrname] = stoken;
            }
        }
        else
        {
            printf("name is exit\n");
            sleep(1);
            system("clear");
            goto reName;
        }
        
    }
    else if (cmd == 3)
    {
        // 功能
        printf("1.vip\n");
        printf("2.tasking\n");
        int num=0;
        cin>>num;
        if(num == 1)
        {
            printf("shifoukaitong vip\n");
            
        }else
        {

        }
    }
    else
    {
        // 退出
        exit(0);
    }
    //把socfd加入监听
    epollAdd(socFd, epfd);
    char BUF[1024]={0};
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
                    string cmd(buf+3);
                    trainState tStat;
                    tStat.data_len=cmd.size();
                    tStat.state=LS;
                    strcpy(tStat.buf,cmd.c_str());
                    send(socFd,&tStat,sizeof(tStat.data_len)+tStat.data_len+sizeof(tStat.state),0);

                }
                if (strncmp(buf, "cd", 2) == 0)
                {
                    // doCd();
                    bzero(&train_t,sizeof(train_t));
                    string cmd(buf+3);
                    trainState tStat;
                    tStat.data_len=cmd.size();
                    tStat.state=CD;
                    strcpy(tStat.buf,cmd.c_str());
                    send(socFd,&tStat,sizeof(tStat.data_len)+tStat.data_len+sizeof(tStat.state),0);
                }
                if (strncmp(buf, "pwd", 3) == 0)
                {
                    // doPwd();
                    bzero(&train_t,sizeof(train_t));
                    bzero(&train_t,sizeof(train_t));
                    string cmd(buf+3);
                    trainState tStat;
                    tStat.data_len=cmd.size();
                    tStat.state=PWD;
                    strcpy(tStat.buf,cmd.c_str());
                    send(socFd,&tStat,sizeof(tStat.data_len)+tStat.data_len+sizeof(tStat.state),0);
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
                    //先socFd
                    getsSocFd=socket(AF_INET, SOCK_STREAM, 0);
                    ret = connect(getsSocFd, (struct sockaddr *)&addr, sizeof(addr));
                    ERROR_CHECK(ret, -1, "connect");
                    // 传递token用带有状态的火车协议，对方收三次
                    string token=usrToken[usrname];
                    trainState tStat;
                    tStat.state = GETS;
                    tStat.data_len =token.size();
                    strcpy(tStat.buf, userpwd1.c_str());
                    send(getsSocFd, &tStat, sizeof(tStat.data_len) + tStat.data_len + sizeof(tStat.state), 0);
                    epollAdd(getsSocFd,epfd);
                    string name = buf+5;
                    string md5=0;
                    //再加锁
                    pthread_mutex_lock(&pThread.tasks.mutex);
                    //把getsSocFd cmd name入队
                    cmd_t cmd={getsSocFd,"gets",name,md5};
                    EnQueue(&pThread.tasks,cmd);
                    printf("new Task\n");
                    //通知
                    pthread_cond_signal(&pThread.tasks.cond);
                    //解锁
                    pthread_mutex_unlock(&pThread.tasks.mutex);
                }
                if (strncmp(buf, "puts", 4) == 0)
                {
                    // 子线程
                    // doPuts();
                    putsSocFd=socket(AF_INET, SOCK_STREAM, 0);
                     // 传递token用带有状态的火车协议，对方收三次
                    string token=usrToken[usrname];
                    trainState tStat;
                    tStat.state = PUTS;
                    tStat.data_len =token.size();
                    strcpy(tStat.buf, userpwd1.c_str());
                    send(putsSocFd, &tStat, sizeof(tStat.data_len) + tStat.data_len + sizeof(tStat.state), 0);
                    epollAdd(putsSocFd,epfd);
                    // int netFd=accept(socFd,NULL,NULL);
                    string name = buf+5;//tiqumingzi
                    const char*path=name.c_str();
                    //MD5
                    setFileMd5(path);
                    ret = Compute_file_md5(file_path, md5_str);
                    // md5_str存放的就是MD5值；
                    if (0 == ret)
                    {
                        printf("%s\t%s\n", md5_str, file_path);
                    }
                    string md5(md5_str);//tiqumd5
                     //再加锁
                    pthread_mutex_lock(&pThread.tasks.mutex);
                    //把netFd cmd name入队
                    cmd_t cmd={putsSocFd,"puts",name,md5};
                    EnQueue(&pThread.tasks,cmd);
                    printf("new Task\n");
                    // pThread.cmd="gets";
                    //通知
                    pthread_cond_signal(&pThread.tasks.cond);
                    //解锁
                    pthread_mutex_unlock(&pThread.tasks.mutex);

                }
            }
            if (readArr[i].data.fd == socFd)
            {
                // 接收信息 ls deng的内容 cd 的内容 
                while (1)
                {
                    
                    int length;
                    ret = recvn(socFd,&length,sizeof(int));
                    ret = recvn(socFd,BUF,length);
                    ERROR_CHECK(ret,-1,"recvDuan");
                    puts(BUF);
                    if(length == 0)
                    {
                        break;
                    }
                }
                
               
            }
            if(readArr[i].data.fd == getsSocFd)
            {
                //下载成功
            }
             if(readArr[i].data.fd == putsSocFd)
            {
                //上传成功
                int rest;
                recvn(socFd,&rest,sizeof(int));
                if(rest == 0)
                {
                    puts("puts successful");
                }
            }
        }
    }


}