// #pragma once
#include"MysqlFunc.h"
#include"Function.h"
//处理各种数据库操作，各种命令的处理函数，mysql数据库操作可以参考
//MD5插入数据库
// int Compute_file_md5(char*,char*);
// int GetMd5Data(char *file_name,char *md5_str)
// {
// 	int ret;
// 	char file_path[30] ="./";
//     strcat(file_path,file_name);
 
// 	ret = Compute_file_md5(file_path, md5_str);
//     ERROR_CHECK(ret,-1,"compute md5");

//     //插入数据库
    
// 	return 0;
// }


int mySqlInit(MYSQL **conn,char *query)
{
    
    const char* server="localhost";//需要在调用函数前准备好数据库三件套，query填入要执行的mysql语句，conn为传入传出 参数返回执行后的值
    const char* user="root";//若使用了MYSQL_RES 需要记得free_result 和closs conn
    const char* password="123";
    const char* database="Ftp1";//要访问的数据库名称
    int t;
    *conn=mysql_init(NULL);
    // *conn=mysql_init(*conn);
    if(!mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(*conn));
        return -1;
    }
    t=mysql_query(*conn,query);
    //t为0表示成功
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
    sprintf(query,"select * from serveruser where usrname='%s';",usrA);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            char a[200]={0};
            char b[200]={0};
            sprintf(a,"%s",row[3]);
            sprintf(b,"%s",pwd);
            if(strcmp(a,b)==0)//匹配密wen成功 返回0  
            {
                printf("connect success\n");
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else 
            {
                printf("fail\n");
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }
        }
    }
}

//查重用户名

int checkName (char *name)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]={0};
    sprintf(query,"select * from serveruser where username='%s';",name);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            char a[200]={0};
            char b[200]={0};
            sprintf(a,"%s",row[1]);
            sprintf(b,"%s",name);
            if(strcmp(a,b)==0)//查重姓名成功 返回0  
            {
                printf("connect success\n");
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else 
            {
                printf("fail\n");
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }
        }
    }
}


int saveServerInfo(char*usrname,char*salt,char*cryptPwd,int curPwd)
{
    MYSQL *conn;
    char query[300]={0};
    sprintf(query,"insert into serveruser (username,salt,cryptPwd,curPwd) value ('%s','%s','%s','%d');",usrname,salt,cryptPwd,curPwd);
    int ret = mySqlInit(&conn,query); 
    mysql_close(conn);
    if(!ret)
    {
        //成功
	    return 0;
    }
    else 
        return -1;
}




int saveServerToken(char*usrname,char*token)
{
    MYSQL *conn;
    char query[300]={0};
    sprintf(query,"update serveruser  set token='%s' where username = '%s';",token,usrname);
    int ret = mySqlInit(&conn,query); 
    mysql_close(conn);
    if(!ret)
    {
	    return 0;
    }
    else 
        return -1;
}




















int dataInsert(int parentId,char *fileName,int ownerid,char *md5,int filesize,char*type)
{
    MYSQL *conn;
    char query[300]={0};
    sprintf(query,"insert into virtualFile (parentId,fileName,ownerId,md5,fileSize,type) value ('%d','%s','%d','%s','%d','%s');",parentId,fileName,ownerid,md5,filesize,type);
    int ret = mySqlInit(&conn,query); 
    mysql_close(conn);
    if(!ret)
    {
	    return 0;
    }
    else 
        return -1;
}

// int SendFileName(,int filenum,int tran_fd)
// {
//     pfilename pot = fnames,repot;
//     pot = fnames;
//     while(pot->nextfn!=NULL)
//     {
//         pot = pot->nextfn;
//         filenum++;
//     }
//     sendCycle(tran_fd,(char*)&filenum,sizeof(int));
//     pot = fnames;
//     while(filenum)
//     {
//         sendCycle(tran_fd,pot->nextfn->fname,sizeof(pot->nextfn->fname));
//         repot = pot->nextfn->nextfn;
//         free(pot->nextfn);
//         pot->nextfn = repot;
        
//         filenum--;
//     }
//     return 0;

// }


int checkMd5(char*buf)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]={0};
    sprintf(query,"select * from serveruser where md5='%s';",buf);
    int ret = mySqlInit(&conn,query);
    if(ret == -1){
        return -1;//biaoshimeiyouzhaodao
    }
    else{
        return 0;//
    }
}

// int FindProcode(ProFile pcur,int code)
// {//返回找到的procode值
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     sprintf(query,"select * from virtualFile where =%d and belongID='%s';",code,pcur->next);
//     int ret = mySqlInit(&conn,query);
//     if(!ret)
//     {
//         res=mysql_use_result(conn);
//         if(res)
//         {
//             row=mysql_fetch_row(res);
//             int len = strlen(row[0]);
//             int b = row[0][0]-48;
//             for(int i = 1;i<len;i++)
//             {
//                 b = b*10+row[0][i]-48;
//             }
//             mysql_free_result(res);
//             mysql_close(conn);
//             return b;
//         }
//     }
//     mysql_free_result(res);
//     mysql_close(conn);
//     return -1;
// }
// //传入precode来列出对应目录下的所有文件
// int LS_func(pfilename fnames,int cdvalue,pNode_t pcur) 
// {//循环发送procode目录下的文件名
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     int filenum=0;
//     int tran_fd = pcur->tran_fd;
//     pfilename pot = fnames;
//     sprintf(query,"select * from filelist where procode=%d and belongID='%s';",cdvalue,pcur->belongID);
//     printf("%s\n",query);
//     int ret = mySqlInit(&conn,query);
//     if(!ret)
//     {
//         res=mysql_use_result(conn);
//         if(res)
//         {
//             if((row=mysql_fetch_row(res))!= NULL) 
//             {
//                 pfilename newfn = (pfilename)calloc(1,sizeof(filename));
//                 strcpy(newfn->fname,row[2]);
//                 pot->nextfn = newfn;
//                 pot = newfn;


//                 while((row=mysql_fetch_row(res))!= NULL)
//                 {
//                     pfilename newfn = (pfilename)calloc(1,sizeof(filename));
//                     strcpy(newfn->fname,row[2]);
//                     pot->nextfn = newfn;
//                     pot = newfn;
//                 }
//             }
//             else
//             {
//                 pcur->ercode = DIREMPTY;
//                 send(pcur->tran_fd,(char*)&pcur->ercode,sizeof(int),0);
//                 mysql_free_result(res);
//                 mysql_close(conn);
//                 return pcur->ercode;
//             }
//             send(pcur->tran_fd,(char*)&pcur->ercode,sizeof(int),0);
//             SendFileName(fnames,filenum,tran_fd);

//             mysql_free_result(res);
//             mysql_close(conn);
//             return 0;
//         }
//         else 
//         {
//             mysql_free_result(res);
//             mysql_close(conn);
//             return -1;
//         }
//     }
// }


// int CD_func(pfilename fnames,pNode_t pcur)
// {//成功返回目录的code
    
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     int procode=-1;
//     int len;
//     int b;
//     sprintf(query,"select * from filelist where belongID='%s' and filetype = '%s' and filename='%s';",pcur->belongID,"d",pcur->rvCmd.Operate);
//     int ret = mySqlInit(&conn,query);
//     pfilename pot = fnames;
//     if(!ret)//成功执行
//     {
//         res=mysql_use_result(conn);
//         if(res)
//         {//取得结果成功
//             fflush(stdin);
//             if((row=mysql_fetch_row(res))!= NULL)
//             {
//                 pfilename newfn = (pfilename)calloc(1,sizeof(filename));
//                 strcpy(newfn->fname,row[2]);
//                 len = strlen(row[1]);
//                 b = row[1][0]-48;
//                 for(int i = 1;i<len;i++)
//                 {
//                     b = b*10+row[1][i]-48;
//                 }
//                 //procode等于该目录的code
//                 procode = b;

//                 pot->nextfn = newfn;
//                 pot = newfn;
//                 while((row=mysql_fetch_row(res))!= NULL)
//                 {
//                     pfilename newfn = (pfilename)calloc(1,sizeof(filename));
//                     strcpy(newfn->fname,row[2]);
//                     len = strlen(row[1]);
//                     b = row[1][0]-48;
//                     for(int i = 1;i<len;i++)
//                     {
//                         b = b*10+row[1][i]-48;
//                     }
//                     //procode等于该目录的code
//                     procode = b;
//                     pot->nextfn = newfn;
//                     pot = newfn;
//                 }
//                 mysql_free_result(res);
//                 mysql_close(conn);
//                 pcur->ercode=DIREMPTY;
//                 return procode;//返回一个错误码，代表目录为空

//             }
//             mysql_free_result(res);
//             mysql_close(conn);
//             pcur->ercode=ERRORDIR;//返回一个错误码，代表目录不存在 
//             return 0;
//         }
//     }
//     mysql_free_result(res);
//     mysql_close(conn);
//     return -1;
// }
// //删除存在一个问题，就是不同用户上传了同名文件
// int REMOVE_func(pNode_t pcur)
// {//成功返回procode

//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     int procode=-1;
//     sprintf(query,"select * from filelist where belongID='%s' and filename='%s';",pcur->belongID,pcur->rvCmd.Operate);
//     printf("%s\n",query);
//     int ret = mySqlInit(&conn,query);
//     if(!ret)
//     {
//         res=mysql_use_result(conn);
//         if(res)
//         {
//             row=mysql_fetch_row(res);
//             char md5[150]={0};
//             strcpy(md5,row[5]);
//             //取md5的值
//             sprintf(query,"delete from filelist where belongID='%s' and filename='%s';",pcur->belongID,pcur->rvCmd.Operate);
//             ret = mySqlInit(&conn,query);
//             //删除数据库中内容
//             if(!ret)
//             {
//                 sprintf(query,"select * from filelist where md5sum='%s';",md5);
//                 printf("%s\n",query);
//                 ret = mySqlInit(&conn,query);
//                 //查找是否还有其他用户拥有该文件
//                 if(!ret)
//                 {
//                     res=mysql_use_result(conn);
//                     if(res)
//                     {
//                         if((row=mysql_fetch_row(res))== NULL)//存在其他用户
//                         {
//                             sprintf(query,"./USRSFILE/%s",pcur->rvCmd.Operate);
//                             printf("%s\n",query);
//                             remove(query);
//                         }
//                         mysql_free_result(res);
//                         mysql_close(conn);
//                         return procode;   
//                         //检查是否还有用户拥有该文件，md5相同，不同belongID


//                     }

//                 }

//             }
//         }

//     }
//     mysql_free_result(res);
//     mysql_close(conn);
//     return -1;   

// }
    
// int getFileCode(pNode_t pcur,char *filename)
// {
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     sprintf(query,"select * from filelist where belongID='%s' and filename='%s';",pcur->belongID,filename);
//     printf("%s\n",query);
//     int ret = mySqlInit(&conn,query);//取得文件的code
//     if(!ret)
//     {
//         res=mysql_use_result(conn);
//         if(res)
//         {
//             row=mysql_fetch_row(res);
        
//             int len = strlen(row[1]);       
//             int b = row[1][0]-48;
//             for(int i = 1;i<len;i++)
//             {
//                 b = b*10+row[1][i]-48;
//             }
//             mysql_free_result(res);
//             mysql_close(conn);
//             return b;
//         }
//     }
//     mysql_free_result(res);
//     mysql_close(conn);
//     return -1;

// }

// int getCdvalue(pNode_t pcur)
// {
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     int code=-1;
//     int p=0;
//     sprintf(query,"select * from UsrMove where UsrCMD='cd' and UsrA='%s' order by ID desc;",pcur->belongID);
//     printf("%s\n",query);//会出现返回上次登陆的目录的问题，待修改，思路是登陆时将结点的信息设置信号
//     int ret = mySqlInit(&conn,query);//取得所有cd记录
//     if(!ret)
//     {
//         res=mysql_use_result(conn);
//         if(res)
//         {
            
//             while((row=mysql_fetch_row(res))!=NULL )
//             {
//                 if(!strcmp(row[3],".."))//若等于。。
//                 {
//                     p++;
//                 }
//                 else
//                     break;
//             }
//             //此时row[2]为第一个遇见的cd值（最新）
//             code = getFileCode(pcur,row[3]);
//             if(p)//若p之前有。。的操作,回溯上级p次
//             {
//                 for(int i =0;i < p && code!=0 ;i++)
//                 {
//                     code = FindProcode(pcur,code);
//                     printf("find ecode = %d\n",code);
//                 }
//                 //找到目录的p个上级返回其code
//             }
//             mysql_free_result(res);
//             mysql_close(conn);
//             return code;   
//             //循环结束dir为所需要的目录的名字

//         }
//     }
//     mysql_free_result(res);
//     mysql_close(conn);
//     return -1;   
// }


// int PWD_func(pfilename fnames,pNode_t pcur)
// {
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     char query[300]={0};
//     int dirnum=0;
//     int code=getCdvalue(pcur);//找到当前cd值
//     int tran_fd = pcur->tran_fd;
//     int ret;
//     pfilename pot = fnames;
//     if(!code)
//     {
//         pcur->ercode=ONROOT;
//         ret = send(pcur->tran_fd,(char*)&pcur->ercode,sizeof(int),0);
//         if(!ret)
//             return 0;
//         else 
//             return -1;
//     }
//     else
//     {
//         while(code)
//         {
//             sprintf(query,"select * from filelist where code=%d and belongID='%s';",code,pcur->belongID);
//             ret = mySqlInit(&conn,query);//读取当前目录的名字并取procode再读procode的名字存入filename
//             if(!ret)
//             {
//                 res=mysql_use_result(conn);
//                 if(res)
//                 {
//                     row=mysql_fetch_row(res);
//                     pfilename newfn = (pfilename)calloc(1,sizeof(filename));
//                     strcpy(newfn->fname,row[2]);
//                     pot->nextfn = newfn;
//                     pot = newfn;
                    
                
//                     int len = strlen(row[0]);//将procode值转换后赋给code
//                     int b = row[0][0]-48;
//                     for(int i = 1;i<len;i++)
//                     {
//                         b = b*10+row[0][i]-48;
//                     }
//                     code = b;
//                 }
//                 //b即为获得的code值
//             }
//         }
//         pcur->ercode=0;
//         send(pcur->tran_fd,(char*)&pcur->ercode,sizeof(int),0);
//         if(ret) 
//         {
//             mysql_free_result(res);
//             mysql_close(conn);
//             return -1;

//         }
//         SendFileName(fnames,dirnum,tran_fd);
//         mysql_free_result(res);
//         mysql_close(conn);
//         return 0;
//     }
// }
