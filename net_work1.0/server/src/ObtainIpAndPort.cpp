#include"Function.h"

static void ftp_arg_set(char *buf, char *arg)
{
    char *ptr = strchr(buf, '='); // 切割字符
    strcpy(arg, ptr + 1);          // 从切割字符后拷贝
}
//获取ip和port
int ObtainIpAndPort(char *path, char *ip, char *port)
{
    FILE*fp_conf=fopen(path,"r");
    ERROR_CHECK(fp_conf,NULL,"fopen");
    char buf[128]={0};
    //获取ip
    char *ret =fgets(buf,sizeof(buf),fp_conf);
    ERROR_CHECK(ret,NULL,"fgets");
    buf[strlen(buf)-1]='\0';//因为文件内容第一行有换行符需手动截取有效位置
    ftp_arg_set(buf,ip);
    //获取port
    bzero(buf,sizeof(buf));
    ret = fgets(buf,sizeof(buf),fp_conf);//第二行已经是最后一行所以不需要手动了
    ERROR_CHECK(ret,NULL,"fgets");
    ftp_arg_set(buf,port);
}