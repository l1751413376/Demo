#include <Windows.h>
#include <vector>
#include "g_logic\g_object.h"

using namespace std;
//fposition
//248,205


//charter
typedef int(*Formula)(character* c);
//skill
struct Skill 
{
	char *Name;//名字
	char *Descr;//描述
	Formula Formula;//计算公式
	genius* Genius; //图片
};
//场景
struct Scenes
{
	char *Name;//名字
	char *Document;//文档

};
vector<Skill*> SkillList;

void InitSkillList() 
{
	auto shugong = new Skill();

	shugong->Name = "shugong";
	shugong->Formula = [](character* c) 
	{
		int mag_attack = c->property.mag*1.2 + 15;
		auto result= c->targets[0]->property.mag-mag_attack;
		if (result >= 0)result = 0;
		return result;
	};
	
	SkillList.push_back(shugong);
}

