#pragma once
#include <Windows.h>
/*��ʽ������*/
void PNGFormatData_R8G8B8A8(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight);

/*��ʽ������*/
void PNGFormatData_ARGB1555(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight);
/*����Png�������Ѿ���ʽ�����ݣ�
ImgData=>(������A)(ÿ�п�ʼ�и�0)(��һ��0RGBA RGBA RGBA....�ڶ���0RGBA RGBA RGBA....)(A=0͸��A=0xFF��͸��)
PNGBuff=>�����ڲ�����
*/
void CreatePNGByFormatData(DWORD Witdh, DWORD Height, BYTE *ImgBuff, int ImgBuffLen, void *& PNGBuff, int & PNGBuffLen);
