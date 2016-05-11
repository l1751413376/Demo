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

//mask解码
int DecompressMask(void* in , void* out)
{
	register BYTE *op;
	register  BYTE *ip;
	register unsigned t;
	register  BYTE *m_pos;

	op = (BYTE *) out;
	ip = (BYTE *) in;

	if (*ip > 17) {
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (1) {
		t = *ip++;
		if (t >= 16) goto match;
		if (t == 0) {
			while (*ip == 0) {
				t += 255;
				ip++;
			}
			t += 15 + *ip++;
		}

		* (unsigned *) op = * ( unsigned *) ip;
		op += 4; ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do {
					* (unsigned * ) op = * ( unsigned * ) ip;
					op += 4; ip += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *ip++; while (--t > 0);
			}
			else do *op++ = *ip++; while (--t > 0);
		}

first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;

		m_pos = op - 0x0801;
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;

		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;

		goto match_done;

		while (1)
		{
match:
			if (t >= 64)
			{

				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;

				goto copy_match;

			}
			else if (t >= 32)
			{
				t &= 31;
				if (t == 0)	{
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 31 + *ip++;
				}

				m_pos = op - 1;
				m_pos -= (* ( unsigned short * ) ip) >> 2;
				ip += 2;
			}
			else if (t >= 16) {
				m_pos = op;
				m_pos -= (t & 8) << 11;
				t &= 7;
				if (t == 0) {
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 7 + *ip++;
				}
				m_pos -= (* ( unsigned short *) ip) >> 2;
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
			}
			else {
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
				*op++ = *m_pos++; *op++ = *m_pos;
				goto match_done;
			}

			if (t >= 6 && (op - m_pos) >= 4) {
				* (unsigned *) op = * ( unsigned *) m_pos;
				op += 4; m_pos += 4; t -= 2;
				do {
					* (unsigned *) op = * ( unsigned *) m_pos;
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else {
copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

match_done:

			t = ip[-2] & 3;
			if (t == 0)	break;

match_next:
			do *op++ = *ip++; while (--t > 0);
			t = *ip++;
		}
	}

eof_found:
	//   if (ip != ip_end) return -1;
	return (op - (BYTE*)out);
}

// 2字节高低位调换
void ByteSwap(WORD& value)
{
	WORD tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}
// 地图的JPEG数据处理器
BYTE* JpgHandler(BYTE* Buffer, int inSize, UINT* outSize)
{
	// JPEG数据处理原理
	// 1、复制D8到D9的数据到缓冲区中
	// 2、删除第3、4个字节 FFA0
	// 3、修改FFDA的长度00 09 为 00 0C
	// 4、在FFDA数据的最后添加00 3F 00
	// 5、替换FFDA到FF D9之间的FF数据为FF 00

	UINT TempNum = 0;						// 临时变量，表示已读取的长度
	BYTE* outBuffer;
	BYTE* TempBuffer = new BYTE[inSize*2];		// 临时变量，表示处理后的数据
	memset(TempBuffer,0,inSize*2);
	outBuffer = TempBuffer;					// 已处理数据的开始地址
	WORD TempTimes = 0;					// 临时变量，表示循环的次数
	UINT Temp = 0;

	// 当已读取数据的长度小于总长度时继续
	while(TempNum < inSize && *Buffer++==0xFF)
	{
		*TempBuffer++=0xFF;
		TempNum++;
		switch(*Buffer)
		{
		case 0xD8:
			*TempBuffer++=0xD8;
			*Buffer++;
			TempNum++;
			break;
		case 0xA0:
			*Buffer++;
			TempBuffer--;
			TempNum++;
			break;
		case 0xC0:
			*TempBuffer++=0xC0;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序


			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}

			break;
		case 0xC4:
			*TempBuffer++=0xC4;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDB:
			*TempBuffer++=0xDB;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDA:
			*TempBuffer++=0xDA;
			*TempBuffer++=0x00;
			*TempBuffer++=0x0C;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序
			*Buffer++;
			TempNum++;
			*Buffer++;

			for(int i=2;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			*TempBuffer++=0x00;
			*TempBuffer++=0x3F;
			*TempBuffer++=0x00;
			Temp += 1; // 这里应该是+3的，因为前面的0xFFA0没有-2，所以这里只+1。

			// 循环处理0xFFDA到0xFFD9之间所有的0xFF替换为0xFF00
			for (;TempNum < inSize-2;)
			{
				if (*Buffer==0xFF)
				{
					*TempBuffer++=0xFF;
					*TempBuffer++=0x00;
					*Buffer++;
					TempNum++;	
					Temp ++;
				}
				else
				{
					*TempBuffer++=*Buffer++;
					TempNum++;
				}
			}
			// 直接在这里写上了0xFFD9结束Jpeg图片.
			Temp--; // 这里多了一个字节，所以减去。
			*TempBuffer--;
			*TempBuffer--=0xD9;
			break;
		case 0xD9:
			// 算法问题，这里不会被执行，但结果一样。
			*TempBuffer++=0xD9;
			TempNum++;
			break;
		default:
			break;
		}
	}
	Temp+=inSize;
	*outSize=Temp;
	return outBuffer;
}

