#include <stdio.h>
#include <Windows.h>


/*.dll动态链接使用动态链接库，允许可执行模块（.dll文件或.exe文件）仅包含在运行时定位DLL函数的可执行代码所需的信息。
  .lib静态链接使用静态链接库，链接器从静态链接库LIB获取所有被引用函数，并将库同代码一起放到可执行文件中。*/
/*使用lib需注意两个文件：
•.h头文件，包含lib中说明输出的类或符号原型或数据结构。应用程序调用lib时，需要将该文件包含入应用程序的源文件中。
•.LIB文件。
使用dll需注意三个文件：
•.h头文件，包含dll中说明输出的类或符号原型或数据结构的.h文件。应用程序调用dll时，需要将该文件包含入应用程序的源文件中。
•.LIB文件，是dll在编译、链接成功之后生成的文件，作用是当其他应用程序调用dll时，需要将该文件引入应用程序，否则产生错误（如果不想用lib文件或者没有lib文件，可以用WIN32 API函数LoadLibrary、GetProcAddress装载）。
•dll文件，真正的可执行文件，开发成功后的应用程序在发布时，只需要有.exe文件和.dll文件，并不需要.lib文件和.h头文件。*/

/*dll静态链接使用 需要头文件，lib文件，dll文件*/

#pragma comment(lib,"dll_or_lib.lib")  

int Max(int a,int b);  
int Min(int a,int b);  


void main()  
{
int a;  
a=Min(8,10);
printf("比较的结果为%d ",a);  
} 
/*lib使用*/


/*dll动态链接使用 需要头文件，lib文件，dll文件*/
/*typedef int(*pMax)(int a,int b);
typedef int(*pMin)(int a,int b);

void main(void)
{
	HINSTANCE hDLL;
	pMax Max;
	hDLL=LoadLibrary(L"dll或者lib.dll");//加载动态链接库MyDll.dll文件；
	Max=(pMax)GetProcAddress(hDLL,"Max");
	auto a=Max(5,8);
	printf("比较的结果为%d ",a);
	FreeLibrary(hDLL);//卸载MyDll.dll文件；
}*/