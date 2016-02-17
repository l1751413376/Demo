#pragma once
//com组件 类型静态链接 运行时不需要dll
#import "../com/msado15.dll" no_namespace rename("EOF","adoEOF")

void InitOLEDBDAL(char *szFilePath=0);
void CloseOLEDBDAL();

//最多允许4个可变参数
_RecordsetPtr Execute(char *sql,...);



