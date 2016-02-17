#pragma once
#include <wchar.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <iostream>
#include <io.h>
#pragma warning( disable : 4996 )//忽略非安全函数
#pragma warning( disable : 4244 )//忽略float=>int转换
//大文件
class __file
{
public:
	__file(){};
public:
	DWORD fsize;
	FILE *pfile;
public:

	void open(const char * path)
	{
		pfile =fopen(path, "rb");
		
		fsize = filelength(fileno(pfile));
	};
	DWORD read_dword() 
	{
		DWORD ch = 0;
		fread(&ch, sizeof(DWORD), 1, pfile);
		return ch;
	};
	WORD read_word()
	{
		WORD ch = 0;
		fread(&ch, sizeof(WORD), 1, pfile);
		return ch;
	};
	BYTE read_byte()
	{
		BYTE ch = 0;
		fread(&ch, sizeof(BYTE), 1, pfile);
		return ch;
	};
	void seek(int pos,int flag=1)
	{
		fseek(pfile,pos,flag);
	}
	int read(void * Dst,int size,int count=1)
	{
		fread(Dst,size,count,pfile);
		return size*count;
	};
	void close()
	{
		if(pfile!=NULL)
			fclose(pfile);
		//pfile=NULL;
	}

};
class __bfile
{
public:
	__bfile(){};
	BYTE *pfhead;//文件头
	BYTE *pfmoving;//文件当前
	DWORD read_dword()
	{
		DWORD ch = 0;
		memcpy(&ch, pfmoving, 4);
		pfmoving += 4;
		return ch;
	}

	WORD read_word()
	{
		WORD ch = 0;
		memcpy(&ch, pfmoving, 2);
		pfmoving += 2;
		return ch;
	}
	//word逆向读取(0x21(l:1,h:2)=>0x12(h:1 l:2))
	WORD read_mword()
	{
		WORD a = 0;
		auto temp=pfmoving;
		__asm 
		{
			mov esi,temp	
				xor eax,eax		
				lodsw				
				xchg al,ah		
				mov a,ax			
				mov temp,esi
		}	
		pfmoving=temp;
		return a;
	}

	BYTE read_byte()
	{
		BYTE ch = *pfmoving;
		pfmoving += 1;
		return ch;
	}

	void seek(int pos,int flag=0)
	{
		if (flag == 0)
			pfmoving = pfhead + pos;
		else
			pfmoving += pos;
	}
	int read(void * Dst,int size,int count=1)
	{
		for (int i = 0; i < count; i++)
		{
			memcpy(Dst, pfmoving, size);
			pfmoving += size;
		}
		return size*count;
	};
	void open(char *filename)
	{
		OFSTRUCT f_info;
		//打开文件
		HANDLE file = (HANDLE)OpenFile(filename, &f_info, OF_READ);

		if (file == INVALID_HANDLE_VALUE)
		{
			return;
		}
		//获取文件大小
		DWORD dwFileSize = GetFileSize(file, NULL);

		BYTE *pFileData = new BYTE[dwFileSize];

		//读取文件
		DWORD dwLen;
		ReadFile(file, (void*)pFileData, sizeof(BYTE)*dwFileSize, &dwLen, NULL);


		if (dwFileSize != dwLen)
		{
			return;
		}
		CloseHandle(file);
		//处理数据
		pfhead=pfmoving=pFileData;
	}
	void close()
	{
		if(pfhead!=NULL)
			delete pfhead;
		pfhead=NULL;
	}
};
/* 
R   =   R1   *   Alpha1   +   R2   *   Alpha2   *   (1-Alpha1)   
G   =   G1   *   Alpha1   +   G2   *   Alpha2   *   (1-Alpha1)   
B   =   B1   *   Alpha1   +   B2   *   Alpha2   *   (1-Alpha1)    
Alpha   =   1   -   (1   -   Alpha1)   *   (   1   -   Alpha2)   
R   =   R   /   Alpha     G   =   G   /   Alpha     B   =   B   /   Alpha  

RGBA alpha 透明度混合算法*/
////等级
//	int level;
//	//base power point力量=20+level*1
//	int bpp;
//	//base magic point魔力=20+level*1
//	int bmp;
//	//base stamina point耐力=20+level*1
//	int bsp;
//	//base physique point体质=20+level*1
//	int bpp;
//	//base attack point基础攻击力=10+level*1
//	int bap;
//	//base defancess point基础防御力=10+level*1
//	int bdp;
//	//base ling point灵力bpp*0.2+bmp*0.3+bsp*0.3+bpp*0.2
//	int blp;
//	//base health point基础生命(50+level*5)
//	int bhp;
//	//base magic durable基础魔法(30+level*3)
//	int bmd;
//	//experience point当前经验
//	int exp;
//	//total experience point升级所需经验100+level*100
//	int texp;
//	//attack point攻击力(bap)*(1+bpp*0.01)+eap;
//	int ap;
//	//defancess point防御力(bdp)*(1+bpp*0.01)+ebp
//	int dp;