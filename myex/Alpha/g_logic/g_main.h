#pragma once


#include <windows.h> // 包含 windows.h 头文件
#include "../app.h"
#define as 10;//频率
//每秒刷新的频率
#define fps 30

//主要的3个逻辑
int Main(void*parms = NULL);
int Init(void*parms = NULL);
int ShutDown(void*parms = NULL);
//按键事件处理
int KeyMes(WPARAM code, BYTE isDown);


//dx逻辑
int Init_dx(void*parms= NULL);
int Release_dx(void*parms= NULL);
//将画好的离屏放到主屏上去
int DrawSurface();
void BltBmp(RECT * lrect, DWORD* piexl);
void BltBmp(RECT * lrect, DWORD**piexl);
void BltBmp(RECT * lrect, DWORD** piexl, RECT * lrecttarget);
//清除离屏的东西
void Clear_BackSurface();
