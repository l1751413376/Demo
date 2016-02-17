#include "Business.h"
#include <stdio.h>

//CBusiness Business;
CBusiness::CBusiness(void)
{
	this->OLEDBDAL=new COLEDBDAL("canyin.mdb");
}


CBusiness::~CBusiness(void)
{
}


int CBusiness::Deposit(PCommonEntity data)
{
	return 0;
}
int CBusiness::Login(PLoginEntity data)
{
	char sql[256];
	sprintf(sql,"select * from AccountInfo where [No]=%d and [Pwd]=%d",data->No,data->Pwd);
	_RecordsetPtr info=this->OLEDBDAL->Execute(sql);
	if(info->adoEOF==0)
		return 1;
	else 
		return 0;
}