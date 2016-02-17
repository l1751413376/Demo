
#ifndef _JPEG_
#define _JPEG_

// 一个色点的类型 (暂时不能修改)
typedef unsigned short int PIXEL;

// 游戏库中使用的不压缩的位图结构
typedef struct {
	int w,h,pitch;      //位图宽和高以及每行实际字节数
	int cl,ct,cr,cb;    //位图剪裁矩形的左上角顶点坐标
	//剪裁矩形的宽和高
	PIXEL *line[1];     //创建位图时动态分配大小
} BMP;
typedef BMP* lpBMP;

__declspec(dllexport) BMP *load_jpg(char *filename);
__declspec(dllexport) BMP *Unpak_jpg(unsigned char *inbuf,unsigned int insize);

// 释放非压缩的位图
#define destroy_bitmap(a) free(a)

__declspec(dllexport) int init_jpeg(void);
#define free_jpeg NULL
#define active_jpeg NULL

#endif