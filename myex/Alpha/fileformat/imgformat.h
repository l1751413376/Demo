#pragma once

#include <stdio.h>
#include <Windows.h>
#include "../app.h"

//------------------------bmp -----------
/*bmp 文件要点(24位无压缩)
1.head信息
2.图片信息 宽度为4像素（每像素3字节）对齐(每行4个像素(实际0)每行5个像素(实际8)每行6个像素(实际8)每行7个像素(实际8))
3.上下颠倒 (！左右不颠倒)
*/
#pragma pack (push)
#pragma pack (2)
 //位图文件头
struct bmpFileHeader
{
	WORD bfType;       // 2 bytes 文件类型 BM
	DWORD bfSize;	   // 4 bytes 文件大小
	WORD bfReserved1;  // 2 bytes 未知
	WORD bfReserved2;  // 2 bytes 未知
	DWORD bfOffBits;   // 4 bytes BMP图像数据的地址
};

//位图信息头
struct bmpInformationHeader
{
	DWORD  biSize;   //4 bytes 本结构的大小
	DWORD  biWidth;   //4 bytes BMP图像的宽度，单位像素
	DWORD  biHeight;  //4 bytes 总为0
	WORD  biPlanes;  //2 bytes 总为0
	WORD biBitCount; //2 bytes 图像的色深，即一个像素用多少位表示，常见有1、4、8、16、24和32，分别对应单色、16色、256色、16位高彩色、24位真彩色和32位增强型真彩色
	DWORD biCompression; //4 bytes 压缩方式，0表示不压缩，1表示RLE8压缩，2表示RLE4压缩，3表示每个像素值由指定的掩码决定
	DWORD biSizeImage;   //4 bytes BMP图像数据大小，必须是4的倍数，图像数据大小不是4的倍数时用0填充补足
	DWORD biXPelsPerMeter;//4 bytes 水平分辨率，单位像素/m 
	DWORD biYPelsPerMeter;//4 bytes 垂直分辨率，单位像素/m
	DWORD biclrUsed;     //4 bytes BMP图像使用的颜色，0表示使用全部颜色，对于256色位图来说，此值为100h=256
	DWORD biClrImportant;//4 bytes 重要的颜色数，此值为0时所有颜色都重要，对于使用调色板的BMP图像来说，当显卡不能够显示所有颜色时，此值将辅助驱动程序显示颜色
};
#pragma pack (pop)
//--------bmp图片处理-----------
class BmpFile : __bfile
{
public:
	BmpFile(){};
	~BmpFile()
	{
		if(bmprgbquad!=NULL) delete bmprgbquad;
		if(color!=NULL) delete color;
	};
	void load(char *filename);
	bmpFileHeader fhead;
	bmpInformationHeader ihead;
	DWORD* bmprgbquad;
	DWORD* color;
private:
};
//------------------------end bmp-----------