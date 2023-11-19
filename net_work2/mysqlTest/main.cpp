// #include"head.h"
// #include "md5.h"
// #include "func.h"
#include <mysql/mysql.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
// #include "thRead.h"
int mySqlInit(MYSQL **conn,char *query)
{
    
    const char* server="localhost";//需要在调用函数前准备好数据库三件套，query填入要执行的mysql语句，conn为传入传出 参数返回执行后的值
    const char* user="root";//若使用了MYSQL_RES 需要记得free_result 和closs conn
    const char* password="123";
    const char* database="Ftp1";//要访问的数据库名称
    int t;
    *conn=mysql_init(NULL);
    if(!mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(*conn));
        return -1;
    }
    t=mysql_query(*conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(*conn));
        mysql_close(*conn);
        return -1;
    }
    else
    {
        return 0;
    }
}

//新注册成功的用户添加到数据库
int UsrAcepdata(char *usrA)
{
    MYSQL *conn;
    char query[300];
    sprintf(query,"insert into clientUser(name,time) value ('%s',now());",usrA) ;
    printf("%s\n",usrA);
    int ret = mySqlInit(&conn,query);
    if(!ret)//成功关闭数据库，这个成功返回值和平常的不一样，相反
        mysql_close(conn);
    else 
        return 0;
}

//插入什么？？？
int UsrMoveData(char *usra,char *cmd,char *opr)
{
    MYSQL *conn;
    char query[300]={0};
    sprintf(query,"insert into UsrMove (UsrA,UsrCMD,UsrOpr,Time) value ('%s','%s','%s',now());",usra,cmd,opr);
    printf("query:%s\n",query);
    int ret = mySqlInit(&conn,query);
    if(!ret)
        mysql_close(conn);
    else 
        return 0;
    
}

//从服务端数据库取出salt
int checkAddress(char *usrA,char *salt)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select * from serveruser;" ;
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {	
                printf("num=%d\n",mysql_num_fields(res));//列数
                //列数和数组下标一样，从0开始
                //我的数据表为 id ， username ， salt ，cryptPwd ，curPwd
                if(strcmp(row[1],usrA)==0)//匹配用户名成功,则取盐值  
                {
                    strcpy(salt,row[2]);
                    break;   
                }
            }
            if(row == NULL)//没匹配到
            {           
                printf("none of this usr Account\n");
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    else 
        return -1;
}
int main()
{
    char salt[256]={0};
    checkAddress("lu",salt);
   printf("%s\n",salt);
}