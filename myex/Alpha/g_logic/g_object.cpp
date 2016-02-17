#include "g_object.h"
#include "action.h"
#include <locale.h>
#include "../dal/OLEDBDAL.h"

extern POINT stand_postion[12];
extern POINT attack_postion[12];


using namespace g_objectspcae;
typedef character_action ca;
//移动过去
void character::moveto()
{
	fp = 0;
	index = 0;
	dytime = 100;
	source_point.x = x;
	source_point.y = y;
	action = ca::fmoveto;
	auto gens = _genius[action];
	auto frameCount = gens->picture->frameCount;
	auto target = targets[0];
	target_point = target->getrel_pos();
	float p = 3 * frameCount;
	pxs = (target_point.x - x) / p;
	pys = (target_point.y - y) / p;

};
POINT character::getrel_pos()
{
	return attack_postion[fightpostion + camp];
};

POINT character::get_pos()
{
	return stand_postion[fightpostion + camp];
};
void character::attack()
{
	//auto hurt=action::baseattackevent(this,this->targets[0]);
	//this->targets[0]->fhit(hurt);
	index = 0;
	action = ca::fattack;
};
//回来
void character::goback()
{
	dir =genius_dir::right_down;
	fp = 0;
	source_point.x = x;
	source_point.y = y;
	index = 0;
	action = ca::fgoback;
	auto gens = _genius[action];
	auto frameCount = gens->picture->frameCount;
	target_point = get_pos();
	float p = 3 * frameCount;
	pxs = (target_point.x - x) / p;
	pys = (target_point.y - y) / p+2;//不知道为什么坐标有问题
};
//站立
void character::stand()
{
	dir =genius_dir::left_up;
	index = 0;
	action = ca::fstand;
	dytime = 100;
};


void character::action_attack(character**_targets)
{
	targets = _targets;
	moveto();
};

void character::run()
{
	fp++;
	//帧数变化
	DWORD current = GetTickCount();
	if (current - tickcount_before > dytime)
	{
		index++;
		tickcount_before = current;
	}
	//位置变化
	switch (action)
	{
	case ca::stand:
		break;
	case ca::move://移动
		if (x > 0 && dir == genius_dir::left)
			x -= 5;
		if (y < 500 && dir == genius_dir::down)
			y += 5;
		if (x < 700 && dir == genius_dir::right)
			x += 5;
		if (y > 0 && dir == genius_dir::up)
			y -= 5;
		break;
	case ca::fstand:
		break;
	case ca::fmoveto:
		//if (index+1 >= _genius[ca::fmoveto]->picture->frameCount)//最后一帧
		//{
		x = target_point.x;
		y = target_point.y;
		attack();
		/*}
		else
		{
		x = source_point.x+pxs*fp;
		y = source_point.y+pys*fp;
		}*/
		//printf_s("x=%d y=%d\n",x,y);
		break;
	case ca::fattack:
		if (index >= _genius[ca::fattack]->picture->frameCount)//最后一帧
		{
			goback();
		}
		break;
	case ca::fgoback:
		//if (index+1 >= _genius[ca::fgoback]->picture->frameCount)//最后一帧
		//{
		x = target_point.x;
		y = target_point.y;
		stand();
		/*}
		else
		{
		x = source_point.x+pxs*fp;
		y = source_point.y+pys*fp;
		}*/
		//printf_s("x=%d y=%d\n",x,y);
		break;
	}
}

DWORD** character::getgenius(RECT& rect)
{
	auto _g = _genius[action];
	_g->x = x;
	_g->y = y;
	_g->dir = dir;
	return _g->getpicture(index, &rect);
};

int character::intoFight()
{
	sence_point.x = x;
	sence_point.y = y;
	camp = camp::r;
	fightpostion = 1;
	flag = 1;
	action = ca::fstand;
	auto p = get_pos();
	x = p.x;
	y = p.y;
	dir = genius_dir::left_up;
	return 0;
}
//
void character::freed(character**_targets)
{
	targets = _targets;
	index = 0;
	action = ca::freed;
};
//
void character::fdefense()
{
	index = 0;
	action = ca::fdefense;
};
//
void character::fhit(int hurt)
{
	index = 0;
	action = ca::fhit;
};
void character::load_access(int arg,WdfFile* wdffile)
{
	for (int i = 0; i < 9; i++)
	{
		auto ret=Execute("select WFFileOffset where WFCharacterId=%d and WFActionId=%d and WFTypeId=0",arg,i);
		if(ret->adoEOF) continue;
		auto set=(DWORD)ret->GetCollect("WFFileOffset");
		if(set==0) continue;
		_genius[i]=new genius();
		_genius[i]->picture=new WasImg();
		auto buffer=wdffile->get_data(set);
		_genius[i]->picture->load_data(buffer);
		if(buffer!=NULL) delete buffer;//释放临时资源
	}
}