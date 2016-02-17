#pragma once
//转账
typedef struct _TransfersEntity
{
	int No;//账号
	int TargetNo;//转账账号
	int Money;
} TransfersEntity,*PTransfersEntity;
//取款
typedef struct _CommonEntity
{
	int No;//账号
	int Money;
}CommonEntity,*PCommonEntity;

//登陆
typedef struct _LoginEntity
{
	int No;//账号
	int Pwd;
}LoginEntity,*PLoginEntity;

//登陆
typedef struct _ErrorEntity
{
	int Code;//错误代码
	char Mes[64];//描述
}ErrorEntity,*PErrorEntity;

typedef struct _DataExchange
{
	char mes[64];//消息
	char dataType[64];//数据类型
	char data[128];//数据
}*PDataExchange,DataExchange;

