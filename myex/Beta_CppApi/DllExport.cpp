#include "DllExport.h"
#include "Png.h"

int g_count = 0;

int __stdcall add(int x, int y)
{
	return g_count++;
}

int __stdcall GetBitMap(void **Pointer)
{
	//Դͼ��

	int imgWidth = 512;
	int imgHeight = 512;
	BYTE *ImgData = new BYTE[imgWidth*imgHeight * 4];
	for (size_t i = 0, len = imgHeight*imgWidth; i < len; i++)
	{
		auto item = ImgData + int(i << 2);
		item[0] = 0xff;//R
		item[1] = 0x00;//G
		item[2] = 0x00;//B
		item[3] = 0xff;//A
	}
	{
		auto item = ImgData + imgHeight*imgWidth * 4 - 4;
		item[0] = 0x00;//R
		item[1] = 0xff;//G
		item[2] = 0x00;//B
		item[3] = 0xff;//A
	}
	//pngͼ��
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg����
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//��������
	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, ImgData, imgWidth, imgHeight);
	CreatePNGByFormatData(imgWidth, imgHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//��Դ�ͷ�
	delete pngIDATbuff;

	*Pointer=pngbuff;
	return buff_len;
}

