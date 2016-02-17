
#include <stdio.h>
#include <string.h>
#include <assert.h>
using namespace std;

class String
{
public:
	String(const char *str = NULL); // 普通构造函数
	String(const String &other); // 拷贝构造函数
	~ String(void); // 析构函数
	String & operator =(const String &other)
	{
		if(this==&other)
			return *this;
		delete m_data;
		m_data=new char[strlen(other.m_data)+1];
		strcpy(m_data,other.m_data);
		return *this;
	} // 赋值函数
private:
	char *m_data; // 用于保存字符串
};

String::String(const char *str)
{
	if(str==NULL)
	{
		m_data=new char[1];
		m_data[0]=0;
	}
	else
	{
		m_data=new char[strlen(str)+1];
		strcpy(m_data,str);
	}
}

String::String(const String &other)
{
	m_data=new char[strlen(other.m_data)+1];
	strcpy(m_data,other.m_data);
}
String::~ String(void)
{
	delete m_data;
}
char *_strcpy(char *a1,char *a2)
{
	char *orgin=a1;
	while((*a1++=*a2++)!=0);
	return orgin;
}

char* s = "0123456789";

void reverse2(char* p){
	if(*(p+1)==0)return;
	char t;
	//o最后一个字符
	for(char* o=p+strlen(p)-1,t=*o;o!=p;o--)
		*o=*(o-1);
	*p=t;
	reverse2(p+1);
}

int _strlen( const char *str ) //输入参数const　以下是引用片段：
{
	assert(str != NULL ); //断言字符串地址非0
	int len=0; //注，一定要初始化。
	while( (*str++) != '\0' )
	{
		len++;
	}
	return len;
}
void main()
{
	int j,k,l,m;

	int i=(j=4,k=8,l=16,m=32); printf("%", i);
	char a1[100];
	char *a2="123";
}



