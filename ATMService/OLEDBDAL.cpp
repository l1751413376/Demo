#include "OLEDBDAL.h"

COLEDBDAL::COLEDBDAL(char *szFilePath)
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
		printf("连接数据库错误信息:%s",e.ErrorMessage());
	}
}


COLEDBDAL::~COLEDBDAL(void)
{
	m_pCon->Close();
}
_RecordsetPtr COLEDBDAL::Execute(char *sql)
{
	try{
		auto v=(_bstr_t)sql;
		return this->m_pCon->Execute(v,NULL,adCmdText);
	}
	catch( _com_error e)
	{
		printf("连接数据库错误信息:%s",e.ErrorMessage());
		return 0;
	}
}

