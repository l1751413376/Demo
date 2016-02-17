#include <windows.h> // 包含 windows.h 头文件
#include <winuser.h>
#include <winbase.h>
#include <stdio.h>
#include <locale.h>
#include "../g_logic/g_main.h"
#include "../ui_logic/ui_logic.h"

//程序入口处
using namespace std;
using namespace ui;
int fs=0;//已经运行的帧数
const int delaytime = 1000 / fps;
HWND Hwnd;//主窗口句柄
application _application;//ui

// 窗口函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 

//控制台
void OpenConsole();
void CloseConsole();
FILE* fpDebugOut = NULL; //输出流
FILE* fpDebugIn  = NULL; //输入流

//WinMain函数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wndclass; // 定义窗口类结构变量
	HWND hwnd; // 定义窗口句柄
	MSG msg; // 定义消息结构变量

	//计算客户区为800*600的窗口实际大小（边框）
	auto style =  WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU |WS_MINIMIZEBOX;
	RECT _rect={0};
	_rect.bottom = 600;
	_rect.right = 800;
	AdjustWindowRect(&_rect, style,0);

	wndclass.style = CS_HREDRAW | CS_VREDRAW ; // 改变窗口大小则重画
	wndclass.lpfnWndProc = WndProc; // 窗口函数为 WndProc
	wndclass.cbClsExtra = 0; // 窗口类无扩展
	wndclass.cbWndExtra = 0; // 窗口实例无扩展
	wndclass.hInstance = hInstance; // 注册窗口类实例句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 用箭头光标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景为白色
	wndclass.lpszMenuName = NULL; // 窗口默认无菜单
	wndclass.lpszClassName = L"window 窗口创建 "; // 窗口类名为 window 窗口创建
	/* 注册窗口类 */
	if (!RegisterClass(&wndclass)) return FALSE;
	/* 创建窗口 */
	hwnd = CreateWindowW(L"window 窗口创建 ", // 窗口类名 window 窗口创建
		L"window 窗口创建 ", // 窗口名 window 窗口创建
		style, // 重叠式窗口
		CW_USEDEFAULT, CW_USEDEFAULT, // 左上角屏幕坐标默认值
		_rect.right-_rect.left, _rect.bottom-_rect.top, // 窗口宽度和高度默认值
		NULL, // 此窗口无父窗口
		NULL, // 此窗口无主菜单
		hInstance, // 创建此窗口的实例句柄
		NULL); // 此窗口无创建参数
	/* 显示并更新窗口 */

	Hwnd = hwnd;

	ShowWindow(hwnd, nCmdShow); // 显示窗口
	UpdateWindow(hwnd); // 更新窗口的客户区
	//打开控制台
	OpenConsole();
	//初始化
	Init();
	//消息循环
	while (TRUE) {
		auto currtime = GetTickCount();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT){ break; }
			TranslateMessage(&msg); // 键盘消息转换
			DispatchMessage(&msg); // 派送消息给窗口函数
		}
		Main();
		//定时
		fs++;
		auto nexttime = GetTickCount();
		auto spendtime = nexttime - currtime;
		if (spendtime < delaytime)
		{
			Sleep(delaytime - spendtime);
		}
	}
	ShutDown();
	//关闭控制台
	CloseConsole();
	return msg.wParam; // 返回退出值
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT: // 重画窗口客户区消息处理
		HDC hdc; // 定义设备描述表句柄
		PAINTSTRUCT ps; // 定义绘图信息结构变量
		hdc = BeginPaint(hwnd, &ps); // 获取要重画的窗口的设备描述表句柄
		TextOutW(hdc, 10, 20, L" 哈哈， Windows 编程创建的窗口 !", 20); // 输出文本
		EndPaint(hwnd, &ps); // 结束要重画的窗口
		return 0;
	case WM_DESTROY: // 撤销窗口消息处理
		PostQuitMessage(0); // 产生退出程序消息 WM_QUIT
		return 0;
		//按键消息
	case WM_KEYDOWN://按下
		return KeyMes(wParam, true);
	case WM_KEYUP://弹起
		return KeyMes(wParam, false);
	case WM_LBUTTONDOWN:
		_application.run(message, wParam, lParam);
		break;
	case WM_QUIT:

		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
	// 其他转默认窗口函数
}

void OpenConsole()
{
	if( !AllocConsole() ) MessageBox(NULL, L"控制台生成失败。", NULL, 0); 
	SetConsoleTitle(L"Debug Window"); 
	freopen_s(&fpDebugOut, "CONOUT$","w", stdout); 
	freopen_s(&fpDebugIn, "CONIN$", "r", stdin); 
	setlocale(LC_ALL, "chs");
};

void CloseConsole()
{
	fclose(fpDebugOut); 
	fclose(fpDebugIn); 
	FreeConsole();
};