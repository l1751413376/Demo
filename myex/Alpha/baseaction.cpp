#include "g_logic/action.h"
using namespace action;

int baseattackevent(character* _source,character* _target,int _targetlength)
{
	auto s_p=&_source->property;
	auto t_p=&_target->property;
	int hurt = 0;//s_p->att*(100 - 10000 / (t_p->def + 100)) / 100;
	t_p->hp-=hurt;
	if(t_p->hp<0)
	{
		t_p->hp=0;
	}
	return hurt;
}