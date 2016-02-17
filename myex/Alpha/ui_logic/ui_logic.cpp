#include "ui_logic.h"
using namespace ui;
void application::run(UINT message, WPARAM wParam, LPARAM lParam)
{
	mouse_x=LOWORD(lParam);
	mouse_y=HIWORD(lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:
		auto target=find();
		if(target!=NULL&&target->onclick!=NULL)
		{
			target->onclick(0);
		}
		break;
	}
}

ui::div* application::find()
{
	for(int i=0;i<content_length;i++)
	{
		auto item=content[i];
		if(item->range.left<=mouse_x
			&&item->range.right>=mouse_x
			&&item->range.top<=mouse_y
			&&item->range.bottom>=mouse_y)
		{
			return item;
		}
	}
	return NULL;
}

void application::Draw()
{
	for(int i=0;i<content_length;i++)
	{
		auto item=content[i];
		BltBmp(&item->range,item->color);
	}
}