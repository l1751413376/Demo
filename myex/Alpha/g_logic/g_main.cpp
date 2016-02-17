#include "g_main.h"
#include "../fileformat/wasformat.h"
#include "../fileformat/mapformat.h"
#include "g_object.h"
#include "../ui_logic/ui_logic.h"
#include "../fileformat/imgformat.h"
#include "../dal/OLEDBDAL.h"
#include "../fileformat/wdfformat.h"
#include "../graphics/font_alpha.h"

using namespace g_objectspcae;
using namespace ui;
using namespace font_alpha;
typedef character_action ca;
//Global Variables

//-----------------extern Variables
extern application _application;
extern HWND Hwnd;
//-----------------end extern Variables
RECT rect;
WasImg fight_map;
tagfile tag_map;

//248,205
POINT stand_postion[12] = {
	{ 560, 400 }//1号位
	, { 510, 350 }//2号位
	, { 0, 0 }
	, { 240, 240 }//对面1号位
	, { 190, 290 }//对面2号位 
	, { 140, 340 }};//对面3号位 
POINT attack_postion[12] = {
	{ 410, 350 }//1号位对应的
	,{ 360, 300 }//2号位对应的
	,{ 310, 250 }//3号位对应的
	, { 390, 290 }//对面1号位对应的
	, { 360, 300 }//对面2号位对应的
	, { 310, 350 }//对面3号位对应的
};

character lead_role;
character monster1;
WdfFile wdffile;
//------------------end Global Variables

//function
//初始化
int Init(void*parms)
{
	InitFont(Hwnd);
	/*InitOLEDBDAL("D:\\Demo\\myex\\Alpha\\data\\sys.mdb");
	wdffile.load_file("");*/
	Init_dx();
	/*tag_map.load_file("bin\\1207.map.tga");
	fight_map.load_file("bin\\708C11A0.was");

	lead_role.load_access(1,&wdffile);
	monster1.load_access(2,&wdffile);*/
	return 0;
}

//结束
int ShutDown(void*parms)
{
	Release_dx();
	return 0;
}

int Main2(void*parms)
{
	Clear_BackSurface();
	////背景
	auto bmp_map = tag_map.bmp32;
	bmp_map += tag_map.header.ImageWidth * 2100;
	DWORD *temp = new DWORD[600 * 800];
	DWORD *_temp = temp;
	for (WORD i = 0; i < 600; i++)
	{
		for (int k = 0; k < 800; k++)
		{
			*_temp++ = *(bmp_map + k + 100);
		}
		bmp_map += tag_map.header.ImageWidth;
	}
	RECT rect_map = { 0 };
	rect_map.bottom = 600;
	rect_map.right = 800;
	BltBmp(&rect_map, temp);
	free(temp);


	DrawSurface();
	return 0;
}

int Main(void*parms)
{
	Clear_BackSurface();
	////背景
	//auto frame = fight_map.frames[0] + 0;
	//RECT rect_stand = { 0 };
	//rect_stand.left = 0;
	//rect_stand.top = 0;
	//rect_stand.bottom = frame->height + rect_stand.top;
	//rect_stand.right = frame->width + rect_stand.left;

	//RECT r = { 0, 0, 800, 600 };
	//BltBmp(&rect_stand, frame->pixels, &r);

	//for (int i = 0, length = 1; i < length; i++)
	//{
	//	auto item = &characters[i];
	//	if (item == 0) continue;
	//	item->run();
	//	RECT rect_stand = { 0 };
	//	auto piexls = item->getgenius(rect_stand);
	//	BltBmp(&rect_stand, piexls);
	//}
	
	RECT rect={0};
	rect.left=100;
	rect.top=100;
	
	Draw_Text("123456/1234",&rect,0x00ffff);
	RECT rect2={0};
	rect2.left=100;
	rect2.top=116;
	rect2.right=200;
	Draw_Text("国家Aj矮",&rect2,0x00ff21,1);
	//rect_stand.bottom=bmp.ihead.biHeight;
	//rect_stand.right=bmp.ihead.biWidth;
	//BltBmp(&rect_stand, bmp.color);
	DrawSurface();
	return 0;
}


int KeyMes(WPARAM code, BYTE isDown)
{
	auto current = &lead_role;
	//在场景中
	if (current->flag == 0)
	{
		if (code == 38 && isDown)//上
		{
			current->dir = genius_dir::up;
			current->action = character_action::move;
		}
		if (code == 39 && isDown)//右
		{
			current->dir = genius_dir::right;
			current->action = character_action::move;
		}
		if (code == 40 && isDown)//下
		{
			current->dir = genius_dir::down;
			current->action = character_action::move;
		}
		if (code == 37 && isDown)//左
		{
			current->dir = genius_dir::left;
			current->action = character_action::move;
		}

		if (code == 38 && !isDown && current->dir == genius_dir::up)//上
		{
			current->action = character_action::stand;
		}
		if (code == 39 && !isDown && current->dir == genius_dir::right)//右
		{
			current->action = character_action::stand;
		}
		if (code == 40 && !isDown && current->dir == genius_dir::down)//下
		{
			current->action = character_action::stand;
		}
		if (code == 37 && !isDown && current->dir == genius_dir::left)//左
		{
			current->action = character_action::stand;
		}
	}
	else
	{
		if (code == 38 && isDown)//上
		{
			//current->action_attack(character1);
		}
	}
	return 0;
};
//------------------------end function