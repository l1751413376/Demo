#pragma once
//com��� ���;�̬���� ����ʱ����Ҫdll
#import "../com/msado15.dll" no_namespace rename("EOF","adoEOF")

void InitOLEDBDAL(char *szFilePath=0);
void CloseOLEDBDAL();

//�������4���ɱ����
_RecordsetPtr Execute(char *sql,...);



