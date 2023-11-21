#include <mysql/mysql.h>
#include<cstdio>
#include<func.h>
#include<iostream>
using namespace std;
int mySqlInit(MYSQL **conn,char *query)
{
    
    const char* server="localhost";//需要在调用函数前准备好数据库三件套，query填入要执行的mysql语句，conn为传入传出 参数返回执行后的值
    const char* user="root";//若使用了MYSQL_RES 需要记得free_result 和closs conn
    const char* password="123";
    const char* database="newTest";//要访问的数据库名称
    int t;
    *conn=mysql_init(NULL);
    if(mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(*conn));
        return -1;
    }
    t=mysql_query(*conn,query);
    //t为0表示成功
    if(t!=0)
    {
        printf("Error making query:%s\n",mysql_error(*conn));
        mysql_close(*conn);
        return -1;
    }
    else
    {
        return 0;//表示成功
    }
}

//从服务端数据库取出salt
int checkAddress(char *usrA,char *salt)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select * from newT;" ;
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {	
                //printf("num=%d\n",mysql_num_fields(res));//列数
                //id | username | salt | cryptPwd | curPwd
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

//匹配密码密文
int checkPwd(char *usrA,char *pwd)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]={0};
    sprintf(query,"select * from newT where name='%s';",usrA);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if (row)
            {
                char a[200]={0};
                char b[200]={0};
                sprintf(a,"%s",row[2]);
                cout<<"a="<<a<<endl;
                sprintf(b,"%s",pwd);
                cout<<"b="<<b<<endl;
                if(strcmp(a,b)==0)//匹配密wen成功 返回0  
                {
                    printf("connect success\n");
                    mysql_free_result(res);
                    mysql_close(conn);
                    return 0;
                }
            }  
            printf("fail\n");
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
            
        }
    }
}
//查重用户名

int checkName (char *name)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW rows;
    char query[300]={0};
    sprintf(query,"select * from newT where name='%s'",name);
    // sprintf(query,"select * from newT");
    int ret = mySqlInit(&conn,query);//0表示成功
    cout<<"ret = "<<ret<<endl;
    if(ret != 0)
    {
        cout<<"失败，没有此人"<<endl;
        return -1;
    }
    cout<<"改名字已被占用"<<endl;
    return 0;
    // if(ret == 0)
    // {
    //     res=mysql_store_result(conn);
    //     printf("the row number = %ld\n",mysql_num_rows(res));
    //     if(res)
    //     {
    //         int row=mysql_num_rows(res);//行数
    //         int col=mysql_num_fields(res);//列数
    //         while((rows = mysql_fetch_row(res)) != NULL)
    //         {
    //             if(strcmp(rows[1],name)==0)
    //             {
    //                 //表示找到了
    //                 cout<<"有重复名字"<<endl;
    //                 mysql_free_result(res);
    //                 mysql_close(conn);
    //                 return 0;
    //             }
    //         }
    //     }
    //     printf("无重复名字\n");
    //     mysql_free_result(res);
    //     mysql_close(conn);
    //     return -1;
    // }
    // return -1;
}

void test()
{
    MYSQL*conn=NULL;
     const char* server="localhost";//需要在调用函数前准备好数据库三件套，query填入要执行的mysql语句，conn为传入传出 参数返回执行后的值
    const char* user="root";//若使用了MYSQL_RES 需要记得free_result 和closs conn
    const char* password="123";
    const char* database="newTest";//要访问的数据库名称
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        exit(-1);
    }
    char query[126] = "select * from newT where name ='xiao'";
    // //  char query[126] = "select * from newT ";
    t=mysql_query(conn,query);
    cout<<"查询结果为："<<t<<endl;
    // if(t != 0)
    // {
    //     exit(-1);
    // }
    MYSQL_RES* res=mysql_store_result(conn);
    if(res == NULL)
    {
        printf("error query\n");
        exit(-1);
    }
    MYSQL_ROW row;
    int rows = mysql_num_rows(res);

    cout<<rows<<endl;
}

int main()
{
    char a[12]="xaio";
    // char name[12]="xiao";
    // char b[12]="90";
    // // checkPwd(a,b);
    checkName(a);
    checkName(name);
    test();

}