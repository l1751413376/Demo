#include "DllExport.h"
#include "Png.h"
#include "mapformat.h"
#include "WasFile.h"
#include "XYWdfEx.h"
#include "XYMapEx.h"

int g_count = 0;

int __stdcall add(int x, int y)
{
	return g_count++;
}

void __stdcall GetBitMap(char * filename, void **Pointer, int * len)
{
	/*
	//Դͼ��
	tagfile map;
	map.load_file(filename);

	int imgWidth = map.header.ImageWidth;
	int imgHeight = map.header.ImageHeight;
	BYTE *ImgData = (BYTE*)map.bmp32;

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
	*Pointer = pngbuff;
	*len = buff_len;
	*/
};

//int __stdcall GetBitMap2(void **Pointer,int filename)

/* in char* filename,out int** ptr,out int* directionCount,out int* frameCount,out int* centerX,out int* centerY ,out int* width,out int *height,out int *centerX,out int *centerY
���� in->���� out->���
filename �ļ�����ptr WasImgָ��,directionCount ���鷽�������frame_Count ֡������ͼƬ��� width height ,ͼƬ�������� centerX,centerY
*/
void __stdcall GetWasFileInfo(const char* filename, int** ptr, int* directionCount, int* frameCount, int* width, int *height, int *centerX, int *centerY)
{
	WasImg *wasImg = new WasImg();
	wasImg->load_file(filename);
	*ptr = (int*)wasImg;
	*directionCount = wasImg->spriteCount;
	*frameCount = wasImg->frameCount;
	*centerX = wasImg->centerX;
	*centerY = wasImg->centerY;
	*width = wasImg->width;
	*height = wasImg->height;
}
/* in char* filename,out int** ptr,out int* directionCount,out int* frameCount,out int* centerX,out int* centerY ,out int* width,out int *height,out int *centerX,out int *centerY
���� in->���� out->���
filename �ļ�����ptr WasImgָ��,directionCount ���鷽�������frame_Count ֡������ͼƬ��� width height ,ͼƬ�������� centerX,centerY,hash��ֵ
*/
void __stdcall GetWasFileInfoFromWdfFile(char* filename, int** ptr, int* directionCount, int* frameCount, int* width, int *height, int *centerX, int *centerY, DWORD hash)
{
	auto wdfFile = LoadWdf(filename);
	auto wdfItem = GetWasSizeByOffset(wdfFile, hash);

	byte* data = new  byte[wdfItem->Size];
	wdfFile->FileHandle->seek(wdfItem->Offset, SEEK_SET);
	wdfFile->FileHandle->read(data, wdfItem->Size);

	WasImg *wasImg = new WasImg();
	wasImg->pfhead = wasImg->pfmoving = data;
	wasImg->load_data();
	*ptr = (int*)wasImg;
	*directionCount = wasImg->spriteCount;
	*frameCount = wasImg->frameCount;
	*centerX = wasImg->centerX;
	*centerY = wasImg->centerY;
	*width = wasImg->width;
	*height = wasImg->height;
	delete data;

}

void __stdcall GetWasFrame(int width, int height, int* wasPtr, int directionIndex, int frameIndex, int** dataptr, int* dataptrLen)
{
	WasImg *wasImg = (WasImg*)wasPtr;
	auto frame = &wasImg->frames[directionIndex][frameIndex];
	auto len = width*height * 4;
	auto buff = new BYTE[len];
	//�������
	auto orginbuff = buff;
	*dataptrLen = len;
	//���ݴ���
	memset(buff, 0, len);
	auto offxl = wasImg->centerX - frame->offX;
	auto offy = wasImg->centerY - frame->offY;

	auto offxr = width - frame->width - offxl;

	buff += width*offy * 4;
	for (int h = 0; h < frame->height; h++)
	{
		buff += offxl * 4;
		for (int w = 0; w < frame->width; w++)
		{
			auto pixle = frame->pixels[h][w];
			auto pixleb = ((byte*)&pixle);
			buff[0] = pixleb[2];
			buff[1] = pixleb[1];
			buff[2] = pixleb[0];
			buff[3] = pixleb[3];
			buff += 4;
		}
		buff += offxr * 4;
	}

	//pngͼ��
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg����
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;

	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, orginbuff, width, height);
	CreatePNGByFormatData(width, height, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	*dataptr = (int*)pngbuff;
	*dataptrLen = buff_len;
	//��Դ�ͷ�
	delete[] orginbuff;
}

void __stdcall GetXYMap(const char * FileName, byte **mapbuff,int* mapbuffsize)
{
	MapInfo * mapinfo = new MapInfo();
	LoadMap(FileName, *mapinfo);
	*mapbuff = (byte*)mapinfo;
	*mapbuffsize = sizeof(MapInfo);
}

void main()
{
	auto file = "D:\\BaiduYunDownload\\�λ�����\\scene\\1002.map";
	byte* mapbuff;
	int mapbuffsize;
	GetXYMap(file,&mapbuff,&mapbuffsize);
}


