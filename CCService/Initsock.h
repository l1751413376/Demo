#pragma once
#include <WinSock2.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

class CInitSock
{
public :
	CInitSock(BYTE minor=2,BYTE majoy=2)
	{
		WSADATA wasData;
		WORD sockVersion=MAKEWORD(minor,majoy);
	    auto err=WSAStartup(sockVersion,&wasData);
		if(err!=0)
		{
			exit(0);
		}
	}
	~CInitSock()
	{
		::WSACleanup();
	}
};