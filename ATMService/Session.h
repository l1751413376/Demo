#pragma once
#include <WinSock2.h>
#include "Entity.h"
#include <thread>
#include "Business.h"
using namespace std;

class Session
{
public:
	Session(void);
	~Session(void);

	thread* _thread;//会话所占用的线程
	byte isUse;//是否使用
	byte isLogin;//是否登陆
	SOCKET _sClient;//套接字
	static void Recv(Session* session);

	static void Request(Session* session);
};

