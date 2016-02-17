#include "../g_logic/g_main.h"
#include <windowsx.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>

#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <ddraw.h>
#include <atlimage.h>
#include <thread>


/*全局变量*/
//dx
LPDIRECTDRAW7 lpdd = 0;
//主屏
LPDIRECTDRAWSURFACE7 lpddsprimary = 0;
//离屏面1
LPDIRECTDRAWSURFACE7 lpddsback1 = 0;
//裁剪器
LPDIRECTDRAWCLIPPER lpDDClipper = 0;
//窗口句柄
extern HWND Hwnd;
DDSURFACEDESC2 ddsd_1;


//清除离屏的东西
void Clear_BackSurface()
{
	RECT rect = { 0 };
	rect.bottom = 600;
	rect.right = 800;

	HRESULT ret;
	ret = lpddsback1->Lock(&rect, &ddsd_1, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0);
	if (FAILED(ret))
	{
		//error
		return;
	}
	int linewidth = ddsd_1.lPitch / 4;
	DWORD*video_buffer = (DWORD*)ddsd_1.lpSurface;

	for (int y = 0; y < rect.bottom; y++){
		for (int x = 0; x < rect.right; x++)
		{
			auto temp_p = video_buffer + x;
			*temp_p = 0;

		}
		video_buffer += linewidth;
	}

	lpddsback1->Unlock(0);

	if (FAILED(ret))
	{
		//error
		return;
	}
}
/*将图像画到离屏上去
像素为矩形
*/
void BltBmp(RECT * lrect, DWORD**piexl)
{
	int width = lrect->right - lrect->left;
	int height = lrect->bottom - lrect->top;
	int off_x = lrect->left;
	int off_y = lrect->top;
	lrect->left = 0;
	lrect->top = 0;
	HRESULT ret;
	ret = lpddsback1->Lock(0, &ddsd_1, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0);
	if (FAILED(ret))
	{
		//error
		return;
	}
	int linewidth = ddsd_1.lPitch / 4;
	DWORD*video_buffer = (DWORD*)ddsd_1.lpSurface;
	//向下偏移
	video_buffer += linewidth*off_y;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			auto pi = piexl[y][x];
			if (pi == 0) continue;
			auto temp_p = video_buffer + x + off_x;
			*temp_p = pi;

		}
		video_buffer += linewidth;
	}

	lpddsback1->Unlock(0);

	if (FAILED(ret))
	{
		//error
		return;
	}
}

/*将图像画到离屏上去
像素为直行
*/
void BltBmp(RECT * lrect, DWORD* piexl)
{
	int width = lrect->right - lrect->left;
	int height = lrect->bottom - lrect->top;
	int off_x = lrect->left;
	int off_y = lrect->top;

	HRESULT ret;
	ret = lpddsback1->Lock(0, &ddsd_1, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0);
	if (FAILED(ret))
	{
		//error
		return;
	}
	int linewidth = ddsd_1.lPitch / 4;
	DWORD*video_buffer = (DWORD*)ddsd_1.lpSurface;
	//向下偏移
	video_buffer += linewidth*off_y;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			auto pi = *piexl;
			auto temp_p = video_buffer + x + off_x;
			if (pi > 0)
			{
				*temp_p = pi;
			}
			piexl++;

		}
		video_buffer += linewidth;
	}

	lpddsback1->Unlock(0);

	if (FAILED(ret))
	{
		//error
		return;
	}
}


//将画好的离屏放到主屏上去
int DrawSurface()
{
	RECT rect = { 0 };
	//BltMap(info);
	GetClientRect(Hwnd, &rect);
	ClientToScreen(Hwnd, (LPPOINT)&rect);
	ClientToScreen(Hwnd, (LPPOINT)&rect + 1);

	HRESULT ret;
	ret = lpddsprimary->Blt(&rect, lpddsback1, 0, DDBLT_WAIT, 0);

	if (FAILED(ret))
	{
		return 0;
	}
	return 1;
}
//将图像渲染到离屏上面并支持放大缩小
void BltBmp(RECT * lrect, DWORD** piexl,RECT * lrecttarget)
{
	auto width = lrect->right - lrect->left;
	auto height = lrect->bottom - lrect->top;
	HRESULT ret = 0;
	DDSURFACEDESC2 _ddsd = {0};
	LPDIRECTDRAWSURFACE7 _lpdds = 0;
	_ddsd.dwSize = sizeof(DDSURFACEDESC2);
	_ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
	_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	_ddsd.dwWidth = width;
	_ddsd.dwHeight = height;
	//创建离屏
	ret = lpdd->CreateSurface(&_ddsd, &_lpdds, 0);
	if (FAILED(ret))
	{
		return;
	}

	ret = _lpdds->Lock(lrect, &_ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0);
	if (FAILED(ret))
	{
		//error
		return;
	}
	int linewidth = _ddsd.lPitch / 4;
	DWORD*video_buffer = (DWORD*)_ddsd.lpSurface;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			auto pi = piexl[y][x];
			if (pi == 0) continue;
			auto temp_p = video_buffer + x;
			*temp_p = pi;

		}
		video_buffer += linewidth;
	}

	_lpdds->Unlock(0);

	if (FAILED(ret))
	{
		//error
		return;
	}
	if(lrecttarget!=NULL)
	{
		ret = lpddsback1->Blt(lrecttarget, _lpdds, 0, DDBLT_WAIT, 0);
	}
	else
	{
		ret = lpddsback1->Blt(lrect, _lpdds, 0, DDBLT_WAIT, 0);
	}
	if (FAILED(ret))
	{
		return;
	}
	if (_lpdds != 0)
	{
		_lpdds->Release();
		_lpdds = 0;
	}
}
//初始化dx
int Init_dx(void*parms)
{
	HRESULT ret = 0;
	//创建dx
	ret = DirectDrawCreateEx(NULL, (void**)&lpdd, IID_IDirectDraw7, NULL);
	if (FAILED(ret))
	{
		//error
		return 0;
	}
	//设置协作方式
	ret = lpdd->SetCooperativeLevel(Hwnd, DDSCL_NORMAL);

	if (FAILED(ret))
	{
		//error
		return 0;
	}

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	//主屏
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;


	//创建主屏
	ret = lpdd->CreateSurface(&ddsd, &lpddsprimary, 0);

	if (FAILED(ret))
	{
		//error
		return 0;
	}

	//离屏
	memset(&ddsd_1, 0, sizeof(DDSURFACEDESC2));
	ddsd_1.dwSize = sizeof(DDSURFACEDESC2);
	ddsd_1.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
	ddsd_1.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd_1.dwWidth = 800;
	ddsd_1.dwHeight = 600;
	//创建离屏
	ret = lpdd->CreateSurface(&ddsd_1, &lpddsback1, 0);
	auto t = ddsd_1.lpSurface;
	if (FAILED(ret))
	{
		//error
		return 0;
	}

	//设置裁剪器
	//将离屏blt到主屏
	ret = lpdd->CreateClipper(0, &lpDDClipper, NULL);

	if (FAILED(ret))
	{
		//error
		return 0;
	}

	ret = lpDDClipper->SetHWnd(0, Hwnd);

	if (FAILED(ret))
	{
		//error
		return 0;
	}

	ret = lpddsprimary->SetClipper(lpDDClipper);

	if (FAILED(ret))
	{
		//error
		return 0;
	}
	//设置裁剪器end

	return 1;

}

//结束
int Release_dx(void*parms)
{
	if (lpdd != 0)
	{
		lpdd->Release();
		lpdd = 0;
	}
	if (lpDDClipper != 0)
	{
		lpDDClipper->Release();
		lpDDClipper = 0;
	}

	return 0;
}
