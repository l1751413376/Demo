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
	char *Name;//����
	char *Descr;//����
	Formula Formula;//���㹫ʽ
	genius* Genius; //ͼƬ
};
//����
struct Scenes
{
	char *Name;//����
	char *Document;//�ĵ�

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

