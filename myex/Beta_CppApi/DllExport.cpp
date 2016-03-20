#include "DllExport.h"
#include "Png.h"
#include "mapformat.h"
#include "WasFile.h"

int g_count = 0;

int __stdcall add(int x, int y)
{
	return g_count++;
}

void __stdcall GetBitMap(char * filename,void **Pointer,int * len)
{
	//源图像
	tagfile map;
	map.load_file(filename);

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
	*Pointer = pngbuff;
	*len=buff_len;
};

//int __stdcall GetBitMap2(void **Pointer,int filename)

/* in char* filename,out int** ptr,out int* directionCount,out int* frameCount,out int* centerX,out int* centerY ,out int* width,out int *height,out int *centerX,out int *centerY
参数 in->输入 out->输出
filename 文件名，ptr WasImg指针,directionCount 精灵方向计数，frame_Count 帧计数，图片宽高 width height ,图片中心坐标 centerX,centerY
*/
void __stdcall GetWasFileInfo(const char* filename,int** ptr,int* directionCount,int* frameCount,int* width,int *height,int *centerX,int *centerY)
{
	WasImg *wasImg=new WasImg();
	wasImg->load_file(filename);
	*ptr=(int*)wasImg;
	*directionCount=wasImg->spriteCount;
	*frameCount=wasImg->frameCount;
	*centerX=wasImg->centerX;
	*centerY=wasImg->centerY;
	*width=wasImg->width;
	*height=wasImg->height;
}

void __stdcall GetWasFrame(int width,int height,int* wasPtr,int directionIndex,int frameIndex,int** dataptr,int* dataptrLen)
{
	WasImg *wasImg=(WasImg*)wasPtr;
	auto frame=&wasImg->frames[directionIndex][frameIndex];
	auto len=width*height*4;
	auto buff=new BYTE[len];
	//输出参数
	auto orginbuff=buff;
	*dataptrLen=len;
	//数据处理
	memset(buff,0,len);
	auto offxl=wasImg->centerX - frame->offX;
	auto offy=wasImg->centerY - frame->offY;

	auto offxr=width-frame->width-offxl;

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

	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, orginbuff, width, height);
	CreatePNGByFormatData(width, height, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);
	
	*dataptr=(int*)pngbuff;
	*dataptrLen=buff_len;
	//资源释放
	delete[] orginbuff;
}
void main()
{
	char * filename="D:/Demo/myex/Beta/Resources/1207.map.tga";
	int *Pointer=0;
	int len=0;
	GetBitMap(filename,(void**)&Pointer,&len);
}