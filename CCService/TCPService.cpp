#include "Initsock.h"
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace std;

CInitSock initSock;
struct threadItem
{
	thread* thread;
	char isUse;
	SOCKET _sClient;
};
threadItem threadPoll[3];
threadItem othersPoll[3];

bool find(threadItem** outthread)
{
	for(int i=0;i<3;i++)
	{
		auto item=&threadPoll[i];
		if(item->isUse==0)
		{
			*outthread=item;
			return true;
		}
	}
	return false;
}
bool findothers(SOCKET _sClient)
{
	memset(othersPoll,0,sizeof(othersPoll));
	int index=0;
	for(int i=0;i<3;i++)
	{
		auto item=&threadPoll[i];
		if(item->isUse==1&&item->_sClient!=_sClient)
		{
			othersPoll[index++]=*item;
		}
	}
	if(index==0)
		return false;
	else 
		return true;
}

void _send(SOCKET _sClient)
{
	while(TRUE)
	{
		char szText[256] = " TCP Server Demo! \r\n";
		//gets(szText) ;
		auto code=::send(_sClient, szText, strlen(szText), 0);
		if(code==-1)
		{
			printf("对方已经关闭连接");
			break;
		}
		Sleep(2000); 
	}
}

void _sendMeg(SOCKET _sClient,char* szText)
{
	auto code=::send(_sClient, szText, strlen(szText), 0);
	if(code==-1)
	{
		printf("对方已经关闭连接");

	}
}
void _recv(SOCKET _sClient)
{
	while(TRUE)
	{
		// 从客户端接收数据
		char buff[256] ;
		int nRecv = ::recv(_sClient, buff, 256, 0);
		if(nRecv==-1)
			break;
		if(nRecv > 0)
		{
			buff[nRecv-1] = 0;
			printf("接收到数据：%s\n", buff);
		}
		if(findothers(_sClient))
		{
			for(int i=0;i<3;i++)
			{
				auto item=&othersPoll[i];
				if(item->isUse==1)
				{
					_sendMeg(item->_sClient,buff);
				}
			}
		};

	}
}
void request(threadItem* _thread)
{
	auto sClient=_thread->_sClient;
	_thread->isUse=1;

	thread tdrecv(_recv,sClient);
	tdrecv.join();
	// 关闭同客户端的连接
	::closesocket(sClient);
	_thread->isUse=0;
	//_thread->thread;
}
int _service()
{
	SOCKET sListen =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sListen==INVALID_SOCKET)
	{
		printf(" failed Socket()");
		return 0;
	}

	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port= htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}

	if(listen(sListen, 2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}

	sockaddr_in remoteAddr; 
	int nAddrLen = sizeof(remoteAddr);


	while(true)
	{
		// 接受一个新连接
		SOCKET sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(sClient == INVALID_SOCKET)
		{
			printf("Failed accept()");
		}

		threadItem* item=0;
		if(!find(&item))
		{
			printf("poll is full \r\n");
			::closesocket(sClient);
			continue;
		}
		printf("接受到一个连接：%s 端口:%d\r\n", inet_ntoa(remoteAddr.sin_addr),remoteAddr.sin_port);
		item->_sClient=sClient;
		item->thread=new thread(request,item);


	}

	// 关闭监听套节字
	::closesocket(sListen);
	return 0;
}


int main()
{
	memset(threadPoll,0,sizeof(threadPoll));


	_service();
	getchar();
	return 0;

}