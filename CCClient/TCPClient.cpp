

#include "Initsock.h"
#include <stdio.h>
#include <iostream>
#include <thread>
using namespace std;
SOCKET s;
CInitSock initSock;

void _recv()
{
	char buff[256];
	while(true)
	{
		int nRecv=recv(s,buff,256,0);
		if(nRecv==-1||nRecv==0)
		{
			printf("服务器已经关闭");
			exit(0);
		}
		if(nRecv>0)
		{
			buff[nRecv]=0;
			printf("接收到数据：%s\n", buff);
		}
	}
}
void _send()
{
	char szTest[256];
	while(true)
	{
		cin>>szTest;
		//szTest[256]=0;//结尾
		send(s,szTest,sizeof(szTest),0);
		if(szTest[0]=='e'&&szTest[1]=='n'&&szTest[2]=='d')
		{
			break;
		}
	}
}

int main()
{
	s=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(s==INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	sockaddr_in servAddr;
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(4567);
	servAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");


	if(::connect(s,(sockaddr*)&servAddr,sizeof(sockaddr_in))==-1)
	{
		printf(" Failed connect() \n");
		return 0;
	}




	thread tdrecv(_recv);
	thread tdsend(_send);

	tdsend.join();
	tdrecv.join();
	closesocket(s);
	return 0;
}