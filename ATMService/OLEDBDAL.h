#pragma once
#import "msado15.dll" no_namespace rename("EOF","adoEOF")
class COLEDBDAL
{
public:
	COLEDBDAL(char *szFilePath=0);
	~COLEDBDAL(void);
	_ConnectionPtr m_pCon;
	_RecordsetPtr Execute(char *sql);

};

