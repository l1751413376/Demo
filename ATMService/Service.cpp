#include "Service.h"

#include "Initsock.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "Session.h"


CInitSock initSock;
Session session[3];
bool find(Session** outsession);

CService::CService(void)
{
	memset(session,0,sizeof(session));
}


CService::~CService(void)
{
}


void CService::service()
{
	SOCKET sListen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sListen==INVALID_SOCKET)
	{
		printf(" failed Socket()");
		return;
	}

	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port= htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return;
	}

	if(listen(sListen, 2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return;
	}

	while(true)
	{
		sockaddr_in remoteAddr; 
		int nAddrLen = sizeof(remoteAddr);
		// 接受一个新连接
		SOCKET sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(sClient == INVALID_SOCKET)
		{
			printf("Failed accept()");
		}

		Session* item=0;
		if(!find(&item))
		{
			printf("poll is full \r\n");
			::closesocket(sClient);
			continue;
		}
		printf("接受到一个连接：%s 端口:%d\r\n", inet_ntoa(remoteAddr.sin_addr),remoteAddr.sin_port);
		item->isUse=1;
		item->_sClient=sClient;
		

		item->_thread=new thread(Session::Request,item);
	}

	// 关闭监听套节字
	::closesocket(sListen);
	return;
};

//查询是否存在未使用的会话
bool find(Session** outsession)
{

	auto size=sizeof(session)/sizeof(Session);
	for(int i=0;i<3;i++)
	{
		auto item=&session[i];
		if(item->isUse==0)
		{
			*outsession=item;
			return true;
		}
	}
	return false;
}
