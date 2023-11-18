#pragma once
#include"head.h"
#include "md5.h"
// #include "func.h"
#include <mysql/mysql.h>
#include "thRead.h"

int mySqlInit(MYSQL **conn,char *query);
//新注册成功的用户添加到数据库
int UsrAcepdata(char *usrA);
//新注册成功的用户添加到数据库
int UsrAcepdata(char *usrA);
int checkAddress(char *usrA,char *salt);
//匹配密码
int checkPwd(char *usrA,char *pwd);

//chachaong
int checkName (char *name);
//salt
char *getSalt();
int saveServerInfo(char*usrname,char*salt,char*cryptPwd,char*curPwd);
int dataInsert(int procode,char *fileName,char *belongID,char *filetype,char *md5Str);