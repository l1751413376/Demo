#include <iostream>
#include <queue>
using namespace std;

struct knight{
	int x,y,step;
	int g,h,f;
	bool operator < (const knight & k) const{      //重载比较运算符
		return f > k.f;
	}
}k;
bool visited[8][8];  //已访问标记(关闭列表)
bool map[8][8];
int x1,y3,x2,y2,ans;                               //起点(x1,y1),终点(x2,y2),最少移动次数ans
int dirs[8][2]={{0,1},{1,1},{1,0},{-1,-1},{-1,0},{0,-1},{-1,1},{1,-1}};//8个移动方向
priority_queue<knight> que;                        //最小优先级队列(开启列表)

bool in(const knight & a){                         //判断knight是否在棋盘内
	if(a.x<0 || a.y<0 || a.x>=8 || a.y>=8)
		return false;
	return true;
}
int Heuristic(const knight &a){                    //manhattan估价函数
	return (abs(a.x-x2)+abs(a.y-y2))*10;
}
void Astar(){                                      //A*算法
	knight t,s;
	while(!que.empty()){
		t=que.top(),que.pop(),visited[t.x][t.y]=true;
		cout<<t.x<<t.y<<endl;
		if(t.x==x2 && t.y==y2){
			ans=t.step;
			break;
		}
		for(int i=0;i<8;i++){
			s.x=t.x+dirs[i][0],s.y=t.y+dirs[i][1];
			if(in(s) && !visited[s.x][s.y]&&!map[s.x][s.y]){
				visited[s.x][s.y]=1;
				s.g = t.g + 10;                 //23表示根号5乘以10再取其ceil
				s.h = Heuristic(s);
				s.f = s.g + s.h;
				s.step = t.step + 1;
				que.push(s);
			}
		}
	}
}
int main(){
	map[3][0]=1;
	map[3][1]=1;
	map[3][3]=1;
	map[3][4]=1;
	map[3][5]=1;
	map[3][6]=1;
	map[3][7]=1;

	map[5][0]=1;
	map[5][1]=1;
	map[5][2]=1;
	map[5][3]=1;
	map[5][4]=1;
	map[5][5]=1;
	map[5][6]=1;

	x1=0,y3=7,x2=7,y2=6;
	memset(visited,false,sizeof(visited));
	k.x=x1,k.y=y3,k.g=k.step=0,k.h=Heuristic(k),k.f=k.g+k.h;
	while(!que.empty()) que.pop();
	que.push(k);
	Astar();
	printf("To get from %d knight moves.\n",ans);

	return 0;
}