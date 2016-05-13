#pragma once
#include <Windows.h>
/*格式化函数*/
void PNGFormatData_R8G8B8A8(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight);

/*格式化函数*/
void PNGFormatData_ARGB1555(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight);
/*创建Png函数（已经格式化数据）
ImgData=>(红绿蓝A)(每行开始有个0)(第一行0RGBA RGBA RGBA....第二行0RGBA RGBA RGBA....)(A=0透明A=0xFF不透明)
PNGBuff=>函数内部创建
*/
void CreatePNGByFormatData(DWORD Witdh, DWORD Height, BYTE *ImgBuff, int ImgBuffLen, void *& PNGBuff, int & PNGBuffLen);
