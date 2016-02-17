#include "Session.h"
 
#include <thread>

using namespace std;
CBusiness Business;

Session::Session(void)
{
}


Session::~Session(void)
{
}
void Session::Recv(Session* session)
{
	while(TRUE)
	{
		// 从客户端接收数据
		char buff[256]={0};
		int nRecv = ::recv(session->_sClient, buff, 256, 0);
		if(nRecv==-1)
			break;
		if(nRecv > 0)
		{
			buff[nRecv-1] = 0;
			printf("接收到数据：%s\n", buff);
		}
		PDataExchange request=(PDataExchange)buff;
		DataExchange Response;
		auto type=typeid(LoginEntity).name();
		if(strcmp(request->dataType,type)==0)
		{
			PLoginEntity entity=(PLoginEntity)request->data;
			auto ret=Business.Login(entity);
			if(ret==0)
			{
				strcpy(Response.mes,"登陆失败");
				strcpy(Response.dataType,typeid(ErrorEntity).name());
				ErrorEntity error;
				error.Code=1;
				memmove(Response.data,&error,sizeof(ErrorEntity));

			}
		}
		::send(session->_sClient,(char *)&Response, 256, 0);
	}
}
void Session::Request(Session* session){
	thread tdrecv(Session::Recv,session);
	tdrecv.join();
	// 关闭同客户端的连接
	::closesocket(session->_sClient);
	session->isUse=0;
}