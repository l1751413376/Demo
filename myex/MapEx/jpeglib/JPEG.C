/*
*	Cloud Wu's JPEG decoder
*
*			2000/6/19 第 2 版
*
*		允许非赢利性质的自由使用, 但如果使用此代码的全部或部分
*		请署上 Cloud Wu (云风)
*
*		商业使用请向作者直接联系
*
*		www.codingnow.com
*		cloudwu@263.net
*	
*		JPEG 解码主函数 (需要风魂)
*/


#include <string.h>
#include <stdlib.h>
//#include "windsoul.h"
#include "jpegint.h"

short jpeg_ybuf[256];
short jpeg_cbbuf[64];
short jpeg_crbuf[64];
short jpeg_DC[3];

int init_jpeg(void)
{
	//	is555=0;
	//	isMMX=1;
	jpeg_setcolormode(1,1);
	return 0;
}

void free_table()
{
	int i;
	for (i=0;i<8;i++)
		if (jpeg_htable[i].htb) free(jpeg_htable[i].htb);
	for (i=0;i<4;i++)
		if (jpeg_qtable[i]) free(jpeg_qtable[i]);
}

void init_table()
{
	memset(jpeg_htable,0,8*sizeof(JPEG_HUFFMANTABLE));
	memset(jpeg_qtable,0,4*sizeof(short *));
}

BMP *Unpak_jpg(unsigned char *inbuf,unsigned int insize)
{
	BMP *tmp;
	BYTE type;
	void *jpeg_buf,*jpeg_buf_begin;
	int HDU,VDU,HY,VY;
	int i,j,jpeg_mode;
	jpeg_buf_begin=jpeg_buf=inbuf;

	if (READ_WORD(jpeg_buf)!=0xd8ff) 
	{
		// 不是 JPEG 文件
		return NULL;
	}

	init_table();

	// 读文件头
	do {
		while(READ_BYTE(jpeg_buf)!=0xff);
		while((type=READ_BYTE(jpeg_buf))==0xff);
		switch (type) {
		case 0x01: break;
		case 0xc0: 
			if (!(jpeg_buf=(BYTE *)read_SOF0(jpeg_buf))) 
				return NULL;
			break;
		case 0xc4: 
			if (!(jpeg_buf=(BYTE *)read_DHT(jpeg_buf)))
				return NULL;
			break;
		case 0xda: 
			if (!(jpeg_buf=(BYTE *)read_SOS(jpeg_buf))) 
				return NULL;
			break;
		case 0xdb: 
			if (!(jpeg_buf=(BYTE *)read_DQT(jpeg_buf))) 
				return NULL;
			break;
		case 0xd9:
			return NULL;
		case 0xdd: // DRI 支持 (jpeg_DRI=MCU 间隔数)
			jpeg_buf=(BYTE *)read_DRI(jpeg_buf);
			break;
			// EOI 标记, 异常结束图象
		default: jpeg_buf=(BYTE *)skip_SEG(jpeg_buf);
			break;
		}
		if (((unsigned)jpeg_buf-(unsigned)jpeg_buf_begin)>=insize) {
			// 解文件头出错
			return NULL;
		}
	} while(type!=0xda); // 扫描行开始

	// 初始化

	HY=jpeg_head.component[0].h;
	VY=jpeg_head.component[0].v;
	if (HY!=VY || HY>2) {
		// 不支持的 MCU (只支持 YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU 三种模式)
		return NULL;
	}

	if (jpeg_head.components==1) jpeg_mode=0;
	else {
		int HCb,VCb,HCr,VCr;
		HCb=jpeg_head.component[1].h;
		VCb=jpeg_head.component[1].v;
		HCr=jpeg_head.component[2].h;
		VCr=jpeg_head.component[2].v;
		if ((HCb|VCb|HCr|VCr)!=1) {
			// 不支持的 MCU (只支持 YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU 三种模式)
			return NULL;
		}
		if (HY==2) jpeg_mode=1;
		else jpeg_mode=2;
	}

	HDU=(jpeg_head.width+HY*8-1)&(-1<<(HY+2));
	VDU=(jpeg_head.height+VY*8-1)&(-1<<(VY+2));
	tmp=create_bitmap(HDU,VDU);

	tmp->w=jpeg_head.width,tmp->h=jpeg_head.height;
	//	tmp=screen;

	// 解码
	jpeg_DC[0]=jpeg_DC[1]=jpeg_DC[2]=0;
	jpeg_preprocess((BYTE *)jpeg_buf);
	jpeg_bit=0;
	jpeg_stream=(BYTE *)jpeg_buf;

	if (jpeg_DRI>0) {
		int dri=0;
		switch (jpeg_mode) {
		case 0: // 灰度 Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					if (dri==jpeg_DRI) {
						dri=0;
						jpeg_stream+=(jpeg_bit+7)/8;
						jpeg_bit=0;
						while (*jpeg_stream!=0xff) ++jpeg_stream;
						++jpeg_stream;
						jpeg_DC[0]=0;
					}
					jpeg_decode_DU(jpeg_ybuf,0);
					Y2RGB(&tmp->line[i][j],tmp->pitch);			
					++dri;
				}
				break;
		case 1: // YYYYCbCr Jpeg
			for (i=0;i<VDU;i+=16)
				for (j=0;j<HDU;j+=16) {
					if (dri==jpeg_DRI) {
						dri=0;
						jpeg_stream+=(jpeg_bit+7)/8;
						jpeg_bit=0;
						while (*jpeg_stream!=0xff) ++jpeg_stream;
						++jpeg_stream;
						jpeg_DC[0]=jpeg_DC[1]=jpeg_DC[2]=0;
					}
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_ybuf+64,0);
					jpeg_decode_DU(jpeg_ybuf+128,0);
					jpeg_decode_DU(jpeg_ybuf+192,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr411(&tmp->line[i][j],tmp->pitch);			
					++dri;
				}
				break;
		case 2: // YCbCr Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					if (dri==jpeg_DRI) {
						dri=0;
						jpeg_stream+=(jpeg_bit+7)/8;
						jpeg_bit=0;
						while (*jpeg_stream!=0xff) ++jpeg_stream;
						++jpeg_stream;
						jpeg_DC[0]=jpeg_DC[1]=jpeg_DC[2]=0;
					}
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr111(&tmp->line[i][j],tmp->pitch);			
					++dri;
				}
				break;
		}
	}
	else {

		switch (jpeg_mode) {
		case 0: // 灰度 Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					jpeg_decode_DU(jpeg_ybuf,0);
					Y2RGB(&tmp->line[i][j],tmp->pitch);			
				}
				break;
		case 1: // YYYYCbCr Jpeg
			for (i=0;i<VDU;i+=16)
				for (j=0;j<HDU;j+=16) {
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_ybuf+64,0);
					jpeg_decode_DU(jpeg_ybuf+128,0);
					jpeg_decode_DU(jpeg_ybuf+192,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr411(&tmp->line[i][j],tmp->pitch);			
				}
				break;
		case 2: // YCbCr Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr111(&tmp->line[i][j],tmp->pitch);			
				}
				break;
		}
	}
	free_table();
	return tmp;
}

BMP *load_jpg(char *filename)
{
	BMP *tmp;
	PACKFILE *f;
	BYTE type;
	BYTE *jpeg_buf,*jpeg_buf_begin;
	int HDU,VDU,HY,VY;
	int i,j,jpeg_mode;

	if ((f=pack_fopen(filename))==NULL) {
		//       DEBUGINFO(1,"无法打开JPEG文件 %s",filename);
		return NULL;
	}

	jpeg_buf_begin=jpeg_buf=(BYTE *)malloc(f->len);
	pack_fread(jpeg_buf,f->len,f);
	pack_fclose(f);

	// SOI，Start of Image，图像开始
	if (READ_WORD(jpeg_buf)!=0xd8ff) {
		// 不是 JPEG 文件
		free(jpeg_buf);
		return NULL;
	}

	init_table();

	// 读文件头

	do {
		//去掉空数据 非0xff
		while(READ_BYTE(jpeg_buf)!=0xff);
		//去掉空数据 0xff 并且读取非0xff的数据
		while((type=READ_BYTE(jpeg_buf))==0xff);

		switch (type) {
		case 0x01: break;
			/*l       SOF0，Start of Frame，帧图像开始

			u  标记代码                   2字节     固定值0xFFC0

			u  包含9个具体字段：
			① 数据长度           2字节     ①~⑥六个字段的总长度
			即不包括标记代码，但包括本字段
			② 精度                 1字节     每个数据样本的位数
			通常是8位，一般软件都不支持 12位和16位
			③ 图像高度           2字节     图像高度（单位：像素），如果不支持 DNL 就必须 >0
			④ 图像宽度           2字节     图像宽度（单位：像素），如果不支持 DNL 就必须 >0
			⑤ 颜色分量数        1字节     只有3个数值可选
			1：灰度图；3：YCrCb或YIQ；4：CMYK
			而JFIF中使用YCrCb，故这里颜色分量数恒为3
			⑥颜色分量信息      颜色分量数×3字节（通常为9字节）

			a)         颜色分量ID                 1字节    

			b)        水平/垂直采样因子      1字节            高4位：水平采样因子
			低4位：垂直采样因子
			（曾经看到某资料把这两者调转了）

			c)        量化表                         1字节            当前分量使用的量化表的ID

			本标记段中，字段⑥应该重复出现，有多少个颜色分量（字段⑤），就出现多少次（一般为3次）。*/
		case 0xc0: if (!(jpeg_buf=(BYTE *)read_SOF0(jpeg_buf))) {
			free(jpeg_buf_begin);
			return NULL;
				   }
				   break;
				   /*l       DHT，Difine Huffman Table，定义哈夫曼表

				   u  标记代码                                 2字节            固定值0xFFC4

				   u  包含2个具体字段：
				   ①数据长度                             2字节            字段①和多个字段②的总长度
				   即不包括标记代码，但包括本字段
				   ② 哈夫曼表              数据长度-2字节

				   a)       表ID和表类型            1字节            高4位：类型，只有两个值可选
				   0：DC直流；1：AC交流
				   低4位：哈夫曼表ID，
				   注意，DC表和AC表分开编码

				   b)      不同位数的码字数量    16字节

				   c)      编码内容       16个不同位数的码字数量之和（字节）

				   本标记段中，字段②可以重复出现（一般4次），也可以致出现1次。例如，Adobe Photoshop 生成的JPEG图片文件中只有1个DHT标记段，里边包含了4个哈夫曼表；而Macromedia Fireworks生成的JPEG图片文件则有4个DHT标记段，每个DHT标记段只有一个哈夫曼表。*/
		case 0xc4: if (!(jpeg_buf=(BYTE *)read_DHT(jpeg_buf))) {
			free(jpeg_buf_begin);
			return NULL;
				   }
				   break;
				   /*
				   l       SOS，Start of Scan，扫描开始 12字节
				   u  标记代码                          2字节     固定值0xFFDA
				   u  包含2个具体字段：
				   ①数据长度                      2字节     ①~④两个字段的总长度
				   即不包括标记代码，但包括本字段
				   ②颜色分量数                 1字节     应该和SOF中的字段⑤的值相同，即：
				   1：灰度图是；3： YCrCb或YIQ；4：CMYK。
				   而JFIF中使用YCrCb，故这里颜色分量数恒为3
				   ③颜色分量信息
				   a) 颜色分量ID           1字节
				   b) 直流/交流系数表号 1字节     高4位：直流分量使用的哈夫曼树编号
				   ④ 压缩图像数据
				   a)谱选择开始                     1字节     固定值0x00
				   b)谱选择结束                     1字节     固定值0x3F
				   c)谱选择                            1字节     在基本JPEG中总为00
				   本标记段中，字段③应该重复出现，有多少个颜色分量（字段②），就出现多少次（一般为3次）。本段结束后，紧接着就是真正的图像信息了。图像信息直至遇到一个标记代码就自动结束，一般就是以EOI标记表示结束。*/
		case 0xda: if (!(jpeg_buf=(BYTE *)read_SOS(jpeg_buf))) {
			free(jpeg_buf_begin);
			return NULL;
				   }
				   break;
				   /*
				   l       DQT，Define Quantization Table，定义量化表
				   u  标记代码                          2字节            固定值0xFFDB
				   u  包含9个具体字段：
				   ① 数据长度                  2字节            字段①和多个字段②的总长度                                                         即不包括标记代码，但包括本字段
				   ② 量化表        数据长度-2字节
				   a)         精度及量化表ID   1字节            高4位：精度，只有两个可选值
				   0：8位；1：16位
				   低4位：量化表ID，取值范围为0～3
				   b)        表项       (64×(精度+1))字节              例如8位精度的量化表
				   本标记段中，字段②可以重复出现，表示多个量化表，但最多只能出现4次。*/
		case 0xdb: if (!(jpeg_buf=(BYTE *)read_DQT(jpeg_buf))) {
			free(jpeg_buf_begin);
			return NULL;
				   }
				   break;
				   /*  l  EOI，End of Image，图像结束 2字节

				   u  标记代码                   2字节     固定值0xFFD9*/
		case 0xd9: free(jpeg_buf_begin);
			return NULL;
			/*
			l       DRI，Define Restart Interval，定义差分编码累计复位的间隔
			u  标记代码                                 2字节     固定值0xFFDD
			u  包含2个具体字段：
			①数据长度                             2字节     固定值0x0004，①~②两个字段的总长度
			即不包括标记代码，但包括本字段
			②MCU块的单元中的重新开始间隔
			2字节     设其值为n，则表示每n个MCU块就有一个
			RSTn标记。第一个标记是RST0，第二个是
			RST1等，RST7后再从RST0重复。
			如果没有本标记段，或间隔值为0时，就表示不存在重开始间隔和标记RST*/
		case 0xdd: // DRI 支持 (jpeg_DRI=MCU 间隔数)
			jpeg_buf=(BYTE *)read_DRI(jpeg_buf);
			break;
			// EOI 标记, 异常结束图象
		default: jpeg_buf=(BYTE *)skip_SEG(jpeg_buf);
			break;
		}
		if (((unsigned)jpeg_buf-(unsigned)jpeg_buf_begin)>=f->len) {
			// 解文件头出错
			free(jpeg_buf_begin);
			return NULL;
		}
	} while(type!=0xda); // 扫描行开始

	// 初始化

	HY=jpeg_head.component[0].h;
	VY=jpeg_head.component[0].v;
	if (HY!=VY || HY>2) {
		// 不支持的 MCU (只支持 YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU 三种模式)
		free(jpeg_buf_begin);
		return NULL;
	}

	if (jpeg_head.components==1) jpeg_mode=0;
	else {
		int HCb,VCb,HCr,VCr;
		HCb=jpeg_head.component[1].h;
		VCb=jpeg_head.component[1].v;
		HCr=jpeg_head.component[2].h;
		VCr=jpeg_head.component[2].v;
		if ((HCb|VCb|HCr|VCr)!=1) {
			// 不支持的 MCU (只支持 YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU 三种模式)
			free(jpeg_buf_begin);
			return NULL;
		}
		if (HY==2) jpeg_mode=1;
		else jpeg_mode=2;
	}

	HDU=(jpeg_head.width+HY*8-1)&(-1<<(HY+2));
	VDU=(jpeg_head.height+VY*8-1)&(-1<<(VY+2));
	tmp=create_bitmap(HDU,VDU);

	tmp->w=jpeg_head.width,tmp->h=jpeg_head.height;
	//	tmp=screen;

	// 解码

	jpeg_DC[0]=jpeg_DC[1]=jpeg_DC[2]=0;
	jpeg_preprocess((BYTE *)jpeg_buf);
	jpeg_bit=0;
	jpeg_stream=(BYTE *)jpeg_buf;

	if (jpeg_DRI>0) {
		int dri=0;
		switch (jpeg_mode) {
		case 0: // 灰度 Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					if (dri==jpeg_DRI) {
						dri=0;
						jpeg_stream+=(jpeg_bit+7)/8;
						jpeg_bit=0;
						while (*jpeg_stream!=0xff) ++jpeg_stream;
						++jpeg_stream;
						jpeg_DC[0]=0;
					}
					jpeg_decode_DU(jpeg_ybuf,0);
					Y2RGB(&tmp->line[i][j],tmp->pitch);			
					++dri;
				}
				break;
		case 1: // YYYYCbCr Jpeg
			for (i=0;i<VDU;i+=16)
				for (j=0;j<HDU;j+=16) {
					if (dri==jpeg_DRI) {
						dri=0;
						jpeg_stream+=(jpeg_bit+7)/8;
						jpeg_bit=0;
						while (*jpeg_stream!=0xff) ++jpeg_stream;
						++jpeg_stream;
						jpeg_DC[0]=jpeg_DC[1]=jpeg_DC[2]=0;
					}
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_ybuf+64,0);
					jpeg_decode_DU(jpeg_ybuf+128,0);
					jpeg_decode_DU(jpeg_ybuf+192,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr411(&tmp->line[i][j],tmp->pitch);			
					++dri;
				}
				break;
		case 2: // YCbCr Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					if (dri==jpeg_DRI) {
						dri=0;
						jpeg_stream+=(jpeg_bit+7)/8;
						jpeg_bit=0;
						while (*jpeg_stream!=0xff) ++jpeg_stream;
						++jpeg_stream;
						jpeg_DC[0]=jpeg_DC[1]=jpeg_DC[2]=0;
					}
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr111(&tmp->line[i][j],tmp->pitch);			
					++dri;
				}
				break;
		}
	}
	else {

		switch (jpeg_mode) {
		case 0: // 灰度 Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					jpeg_decode_DU(jpeg_ybuf,0);
					Y2RGB(&tmp->line[i][j],tmp->pitch);			
				}
				break;
		case 1: // YYYYCbCr Jpeg
			for (i=0;i<VDU;i+=16)
				for (j=0;j<HDU;j+=16) {
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_ybuf+64,0);
					jpeg_decode_DU(jpeg_ybuf+128,0);
					jpeg_decode_DU(jpeg_ybuf+192,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr411(&tmp->line[i][j],tmp->pitch);			
				}
				break;
		case 2: // YCbCr Jpeg
			for (i=0;i<VDU;i+=8)
				for (j=0;j<HDU;j+=8) {
					jpeg_decode_DU(jpeg_ybuf,0);
					jpeg_decode_DU(jpeg_cbbuf,1);
					jpeg_decode_DU(jpeg_crbuf,2);
					YCbCr111(&tmp->line[i][j],tmp->pitch);			
				}
				break;
		}
	}

	free_table();
	free(jpeg_buf_begin);
	return tmp;
}
