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
	//源图像

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
	//png图像
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg数据
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//函数调用
	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, ImgData, imgWidth, imgHeight);
	CreatePNGByFormatData(imgWidth, imgHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//资源释放
	delete pngIDATbuff;

	*Pointer=pngbuff;
	return buff_len;
};

int __stdcall GetBitMap2(void **Pointer,int filename)
{
	//源图像
	tagfile map;
	map.load_file("D:/Demo/myex/Beta/Resources/1207.map.tga");

	int imgWidth = map.header.ImageWidth;
	int imgHeight = map.header.ImageHeight;
	BYTE *ImgData = (BYTE*)map.bmp32;

	//png图像
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg数据
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//函数调用
	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, ImgData, imgWidth, imgHeight);
	CreatePNGByFormatData(imgWidth, imgHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//资源释放
	delete pngIDATbuff;

	*Pointer = pngbuff;
	return buff_len;
};

//int __stdcall GetBitMap2(void **Pointer,int filename)

/* in char* filename,out int** ptr,out int* directionCount,out int* frameCount
参数 in->输入 out->输出
filename 文件名，ptr WasImg指针,directionCount 精灵方向计数，frame_Count 帧计数
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
	//输出参数
	*dataptr=(int*)buff;
	*dataptrLen=len;
	//数据处理
	memset(buff,0,len);
	auto offxl=(width-frame->width)/2;
	auto offxr=width-frame->width-offxl;
	auto offy=(height-frame->height)/2;
	buff+=width*offy*4;
	for(int h=0;h<frame->height;h++)
	{
		buff += offxl*4;
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
		buff += offxr*4;
	}

	//png图像
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg数据
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;

	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, (byte*)*dataptr, width, height);
	CreatePNGByFormatData(width, height, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	*dataptr=(int*)pngbuff;
	*dataptrLen=buff_len;
}
void main()
{
	int imgWidth=150;
	int imgHeight=150;

	auto filename="D:/Demo/myex/Alpha/project/bin/0AF85B1A.was";
	int *intptr=0;
	int directionCount=0;
	int frameCount=0;
	GetWasFileInfo(filename,&intptr,&directionCount,&frameCount);
	int *dataptr=0;
	int datalen=0;

	for(int i=0;i<frameCount;i++)
	{

		GetWasFrame(imgWidth,imgHeight,intptr,0,i,&dataptr,&datalen);
		char pngfilename[] = "D:/Demo/myex/Beta/img/testcrc0.png";
		pngfilename[29]=(char)i+48;
		auto file = fopen(pngfilename, "wb+");
		fwrite(dataptr, datalen, 1, file);
		fclose(file);
	}
}