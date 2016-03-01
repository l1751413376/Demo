#pragma once
#include <wchar.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <iostream>
#include <io.h>
#pragma warning( disable : 4996 )//���Էǰ�ȫ����
#pragma warning( disable : 4244 )//����float=>intת��
//���ļ�
class __file
{
public:
	__file() {};
public:
	DWORD fsize;
	FILE *pfile;
public:

	void open(const char * path)
	{
		pfile = fopen(path, "rb");

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
	void seek(int pos, int flag = 1)
	{
		fseek(pfile, pos, flag);
	}
	int read(void * Dst, int size, int count = 1)
	{
		fread(Dst, size, count, pfile);
		return size*count;
	};
	void close()
	{
		if (pfile != NULL)
			fclose(pfile);
		//pfile=NULL;
	}

};
class __bfile
{
public:
	__bfile() {};
	BYTE *pfhead;//�ļ�ͷ
	BYTE *pfmoving;//�ļ���ǰ
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
	//word�����ȡ(0x21(l:1,h:2)=>0x12(h:1 l:2))
	WORD read_mword()
	{
		WORD a = 0;
		auto temp = pfmoving;
		__asm
		{
			mov esi, temp
			xor eax, eax
				lodsw
				xchg al, ah
				mov a, ax
				mov temp, esi
		}
		pfmoving = temp;
		return a;
	}

	BYTE read_byte()
	{
		BYTE ch = *pfmoving;
		pfmoving += 1;
		return ch;
	}

	void seek(int pos, int flag = 0)
	{
		if (flag == 0)
			pfmoving = pfhead + pos;
		else
			pfmoving += pos;
	}
	int read(void * Dst, int size, int count = 1)
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
		//���ļ�
		HANDLE file = (HANDLE)OpenFile(filename, &f_info, OF_READ);

		if (file == INVALID_HANDLE_VALUE)
		{
			return;
		}
		//��ȡ�ļ���С
		DWORD dwFileSize = GetFileSize(file, NULL);

		BYTE *pFileData = new BYTE[dwFileSize];

		//��ȡ�ļ�
		DWORD dwLen;
		ReadFile(file, (void*)pFileData, sizeof(BYTE)*dwFileSize, &dwLen, NULL);


		if (dwFileSize != dwLen)
		{
			return;
		}
		CloseHandle(file);
		//��������
		pfhead = pfmoving = pFileData;
	}
	void close()
	{
		if (pfhead != NULL)
			delete pfhead;
		pfhead = NULL;
	}
};