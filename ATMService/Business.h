#pragma once
#include "OLEDBDAL.h"
#include "Entity.h"


class CBusiness
{
public:
	CBusiness(void);
	~CBusiness(void);
	COLEDBDAL* OLEDBDAL;
	int Transfers(PTransfersEntity data);
	int WithDrawals(PCommonEntity data); 
	int Deposit(PCommonEntity data); 
	int Search(PCommonEntity data); 
	int Login(PLoginEntity data); 
};

