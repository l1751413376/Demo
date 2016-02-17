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
		控件坐标范围
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
		//颜色
		DWORD *color;
		//获取和设置宽度
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
		//获取和设置高度
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
		//获取和设置显示层次
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
		int mouse_x;//鼠标x坐标
		int mouse_y;//鼠标y坐标
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