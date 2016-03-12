#include "DllExport.h"
#include "Png.h"
#include "mapformat.h"
#include "WasFile.h"

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
};

int __stdcall GetBitMap2(void **Pointer,int filename)
{
	//Դͼ��
	tagfile map;
	map.load_file("D:/Demo/myex/Beta/Resources/1207.map.tga");

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
	delete pngIDATbuff;

	*Pointer = pngbuff;
	return buff_len;
};

//int __stdcall GetBitMap2(void **Pointer,int filename)

/* in char* filename,out int** ptr,out int* directionCount,out int* frameCount
���� in->���� out->���
filename �ļ�����ptr WasImgָ��,directionCount ���鷽�������frame_Count ֡����
*/
void __stdcall GetWasFileInfo(const char* filename,int** ptr,int* directionCount,int* frameCount)
{
	WasImg *wasImg=new WasImg();
	wasImg->load_file(filename);
	*ptr=(int*)wasImg;
	*directionCount=wasImg->spriteCount;
	*frameCount=wasImg->frameCount;
}

void __stdcall GetWasFrame(int width,int height,int* wasPtr,int directionIndex,int frameIndex,int** dataptr,int* dataptrLen)
{
	WasImg *wasImg=(WasImg*)wasPtr;
	auto frame=&wasImg->frames[directionIndex][frameIndex];
	auto len=width*height*4;
	auto buff=new BYTE[len];
	//�������
	*dataptr=(int*)buff;
	*dataptrLen=len;
	//���ݴ���
	memset(buff,0,len);
	auto offx=(width+wasImg->width - wasImg->centerX - frame->offX)/2;
	auto offy=(height+wasImg->height - wasImg->centerY - frame->offY)/2;

	//buff+=offy*4;
	for(int h=0;h<frame->height;h++)
	{
		//buff+= offx<<2;
		for(int w=0;w<frame->width;w++)
		{
			auto pixle=frame->pixels[h][w];
			auto pixleb=((byte*)&pixle);
			buff[0]=pixleb[2];
			buff[1]=pixleb[1];
			buff[2]=pixleb[0];
			buff[3]=pixleb[3];
			buff+=4;
		}
		//buff+= (width-frame->width)*4;
	}


}
void main()
{
	int imgWidth=39;
	int imgHeight=93;

	auto filename="D:/Demo/myex/Alpha/project/bin/0AF85B1A.was";
	int *intptr=0;
	int directionCount=0;
	int frameCount=0;
	GetWasFileInfo(filename,&intptr,&directionCount,&frameCount);
	int *dataptr=0;
	int datalen=0;
	GetWasFrame(imgWidth,imgHeight,intptr,0,0,&dataptr,&datalen);

	//pngͼ��
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg����
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;

	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, (byte*)dataptr, imgWidth, imgHeight);
	CreatePNGByFormatData(imgWidth, imgHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	auto pngfilename = "D:/Demo/myex/Beta/img/testcrc.png";
	auto file = fopen(pngfilename, "wb+");
	fwrite(pngbuff, buff_len, 1, file);
	fclose(file);
}