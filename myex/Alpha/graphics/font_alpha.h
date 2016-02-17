#pragma once
#include "../app.h"
#include <Windows.h>


namespace font_alpha
{
	#define align_left 0
	#define align_center 1

	void InitFont(HWND hwnd);
	void Draw_Text(char * str,RECT* rect,DWORD colour=0xffffffff,int align=0);
	class font_a
	{
	public: 
		int size;//�ֺ�
		int height;//ʵ�ʸ߶�
		int width;//ʵ�ʿ��
		UINT code;//�ַ���
		DWORD* buff;//�ֽ�
	public:
		font_a(UINT code,int size=16);
		~font_a()
		{
			if(buff!=NULL)
			{
				delete buff;
			}
		};
	};
}