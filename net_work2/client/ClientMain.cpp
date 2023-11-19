#include "head.h"
// #include "md5.h"
#include "Function.h"

#include"MysqlFunc.h"

// 设置md5的文件相关路径
const char *file_path;
char md5_str[MD5_STR_LEN + 1];
int setFileMd5(const char *path)
{
    file_path = path;
}
/***********************************/
 string usrname,usrpwd1,usrpwd2;
int check;//定义全局 用来检查是否正确
unordered_map<string, string> usrToken; // 存放usrname和token
int main()
{
    vector<char *> usrName;
  
    //./client 
    //先连接
    //  客户端 socket connect
    int socFd= socket(AF_INET,SOCK_STREAM,0);
    int ret;
    int putsSocFd,getsSocFd; 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi("2000"));
    addr.sin_addr.s_addr = inet_addr("192.168.0.158");
    ret = connect(socFd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connet");
    cout<<"已连接"<<endl;
    int epfd = epoll_create(1);
    epollAdd(STDIN_FILENO, epfd);
    epollAdd(socFd,epfd);
    struct epoll_event readArr[3];
    char buf[1024] = {0};
    Thread pThread;//定义线程组
    InitThread(&pThread,10);
    makeChild(&pThread);
   trainState sTat;//定义带有状态的火车
   printf("1.注册\n2.登录\n");
   char cmd[1024]={0};

   while (1)
   {
        int readNum=epoll_wait(epfd,readArr,3,-1);
        for(int i=0;i<readNum;++i)
        {
            if(readArr[i].data.fd == STDIN_FILENO)
            {
                read(STDIN_FILENO,buf,sizeof(buf));
                if(strncmp(buf,"1",1)==0)
                {
                    //注册
                    registerUser(socFd);
                }
                if(strncmp(buf,"2",1)==0)
                {
                    //登录

                }
            }
            if(readArr[i].data.fd==socFd)
            {
                int len;
                int stats;
                recvn(socFd,&len,sizeof(int));
                recvn(socFd,&stats,sizeof(int));
                switch(stats)
                {
                    case REGISTER:
                        recvn(socFd,cmd,len);
                        cout<<"cmd ="<<cmd<<endl;
                        if(strcmp(cmd,"true")==0)
                        {
                            check=-1;//表示可用
                            cout<<check<<endl;
                        }else{
                            check = 0;
                            cout<<check<<endl;
                        }
                    break;
                    
                    case Name:
                        char n[128]={0};
                        recvn(socFd,n,len);
                        break;
                }
            }
        }
   }
   


}