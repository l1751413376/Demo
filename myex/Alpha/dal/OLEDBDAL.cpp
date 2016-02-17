#include "OLEDBDAL.h"
#include "../app.h"
#include <stdarg.h>

_ConnectionPtr m_pCon;
void InitOLEDBDAL(char *szFilePath)
{
	try
	{
		::CoInitialize(NULL);
		HRESULT hr;
		hr=m_pCon.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			m_pCon->ConnectionTimeout=3;
			char *pix="Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
			char *fullStr=new char[strlen(pix)+strlen(szFilePath)+1];
			strcat(strcpy(fullStr,pix),szFilePath);
			hr=m_pCon->Open(fullStr,"","",adModeUnknown);
			
		}
	}
	catch(_com_error e)
	{
		printf("连接数据库错误信息:%ls",e.ErrorMessage());
	}
}


void CloseOLEDBDAL()
{
	m_pCon->Close();
}
_RecordsetPtr Execute(char *sql,...)
{
	char buff[256];
	va_list argp; 
	va_start(argp, sql);
	auto v1=va_arg(argp,void*);
	auto v2=va_arg(argp,void*);
	auto v3=va_arg(argp,void*);
	auto v4=va_arg(argp,void*);
	sprintf(buff,sql,v1,v2,v3,v4);
	va_end(argp); 
	return m_pCon->Execute(_bstr_t(buff), nullptr,adCmdText);
}

