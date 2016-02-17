#pragma once
#include <windows.h>
#include "../g_logic/g_main.h"

typedef void (*event_onclick)(void * eventargs);
namespace ui{
	class div
	{
	public:
		div(){};
		~div()
		{

		};
		event_onclick onclick;
	public:

		/*x=range.left,y=range.top
		�ؼ����귶Χ
		*/
		union 
		{
			RECT range;
			struct 
			{
				LONG x;
				LONG y; 
			};
		};
		//��ɫ
		DWORD *color;
		//��ȡ�����ÿ��
		int width(int size=-1)
		{
			if(size>-1) 
			{
				range.right=range.left+size;
				return size;
			}
			else
			{
				return range.right-range.left;
			}
		};
		//��ȡ�����ø߶�
		int height(int size=-1)
		{
			if(size>-1) 
			{
				range.bottom=range.top+size;
				return size;
			}
			else
			{
				return range.bottom-range.top;
			}
		};
		//��ȡ��������ʾ���
		int z_index(int _z_index)
		{
			return __z_index;
		}
	private:
		div* content[1];
		int content_length;
		int __z_index;
	};

	class application
	{
	public:
		int mouse_x;//���x����
		int mouse_y;//���y����
	public:
		application(){};
		~application(){};
		div* content[1];
		int content_length;
		void run(UINT message, WPARAM wParam, LPARAM lParam);
		div* find();
		void Draw();
	};
}