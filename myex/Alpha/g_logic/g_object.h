#pragma once

#include <Windows.h>
#include "../fileformat/wasformat.h"
#include "../fileformat/wdfformat.h"
namespace g_objectspcae
{
	enum genius_dir
	{
		up = 6,//上
		right_up = 3,//右上
		right = 7,//右
		right_down = 0,//右下
		down = 4,//下
		left_down = 1,//左下
		left = 5,//左
		left_up = 2,//左上
	};
	enum character_action
	{
		none = 0,
		stand = 0,
		move=1,
		fstand=2,
		fmoveto=3,
		fattack=4,
		fgoback=5,
		freed=6,
		fdefense=7,//防御
		fhit=8,//被打
	};
	enum camp
	{
		r=0,//左边
		l=5,//右边
	};
	//属性
	struct Property
	{
		int level;
		int exp;
		int hp;
		int mp;
		//int tp;
		int atk;
		int def;
		int mag;
		int spd;
	};

};



//精灵
class genius
{
public:
	genius()
	{
		dytime=100;
		tickcount_before=0;
	};
	~genius()
	{

	};

public:
	int x, y;//坐标
	g_objectspcae::genius_dir dir;//方向
	int index;
	DWORD dytime;
	DWORD tickcount_before;
	WasImg* picture;//was图片

	DWORD** getpicture(int ac,RECT * lrect)
	{
		auto frame = picture->frames[dir] + ac % picture->frameCount;
		lrect->left = x + picture->width - picture->centerX - frame->offX;
		lrect->top = y + picture->height - picture->centerY - frame->offY;
		lrect->bottom = frame->height + lrect->top;
		lrect->right = frame->width + lrect->left;
		return frame->pixels;
	};
	void run()
	{
		//帧数变化
		DWORD current = GetTickCount();
		if (current - tickcount_before > dytime)
		{
			index++;
			tickcount_before = current;
		}
	}
};
//人物对象
class character
{
public:
	character()
	{
		dytime=100;
	};
	~character()
	{

	};
	//在场景中的位置
	POINT sence_point;
	//当前位置
	int x, y;
	genius* _genius[8];
	int fightpostion;//战斗顺序
	g_objectspcae::camp camp;//阵营
	int flag;//0普通 1战斗
	character**targets;//目标
	g_objectspcae::character_action action;//当前动作类型
	POINT target_point;//当前目标坐标
	POINT source_point;//源坐标
	float pxs;//人物移动x增量
	float pys;//人物移动y增量
	g_objectspcae::genius_dir dir;//方向
	int index;//当前帧索引
	DWORD dytime;//每帧延时ms
	DWORD tickcount_before;
	int fp;
	g_objectspcae::Property property;
	char *name;
public :
	//relative position
	POINT getrel_pos();
	POINT get_pos();
	DWORD** getgenius(RECT& rect);
	void run();
	void action_attack(character**_target);//普通攻击
	void moveto();//移动过去
	void attack();//hit
	void goback();//回来
	void stand();//站立
	int intoFight();
	void freed(character**_targets);//
	void fdefense();//
	void fhit(int hurt=0);//
	void load_access(int arg,WdfFile* wdffile);
	//设置改组动作一共所需时间
	void set_acttime(DWORD time,int action=0);
};


