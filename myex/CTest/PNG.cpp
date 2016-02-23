#include "CRC.h"
#include <Windows.h>
#include <iostream>
#include <io.h>
#pragma warning( disable : 4996 )//���Էǰ�ȫ����
namespace PNG {}

using namespace PNG;
#define out //�������

#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static BYTE png_Title[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A }; //.PNG....
const static BYTE png_CompressionAlgorithm[3] = { 0x78,0x01,0x00 };
const static BYTE png_ImgDataEnd[5] = { 0x01,0x00,0x00,0xFF,0xFF };

DWORD ChgDWORD(DWORD value);
inline WORD ChgWORD(WORD value);
//�������ݲ���ͬʱ�ƶ�ָ��
inline void Xmemmove(BYTE* &buff, void * value, int size)
{
	memmove(buff, value, size);
	buff += size;
}
#pragma pack (push)
#pragma pack (1)
class Chunk
{
public:
	DWORD ChunkLen;//ChunkLen ���� ChunkLen��Tag��CRC�ĳ���
	DWORD Tag;//4byte
	DWORD CRC;
};



class IHDR : public Chunk
{
public:
	DWORD Width;//��
	DWORD Height;//��
	BYTE  BitDepth;//λ�� ���ɫͼ��8��16
	BYTE  ColorType;//��ɫ���� 6������ͨ�����ݵ����ɫͼ��8��16
	BYTE  CompressionMethod;//ѹ������(LZ77�����㷨) �̶�0
	BYTE  FilterMethod;//�˲������� �̶�0
	BYTE  InterlaceMethod;//����ɨ�跽����0���Ǹ���ɨ�� 1�� Adam7(��Adam M.Costello������7�����ɨ�跽��)
public:
	void WriteMemoryAndCRC(BYTE* &buff)
	{
		DWORD tempDWORD = 0;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(ChunkLen)), sizeof(ChunkLen));
		auto buff_begin = buff;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Tag)), sizeof(Tag));
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Width)), sizeof(Width));
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Height)), sizeof(Height));
		Xmemmove(buff, &BitDepth, sizeof(BitDepth));
		Xmemmove(buff, &ColorType, sizeof(ColorType));
		Xmemmove(buff, &CompressionMethod, sizeof(CompressionMethod));
		Xmemmove(buff, &FilterMethod, sizeof(FilterMethod));
		Xmemmove(buff, &InterlaceMethod, sizeof(InterlaceMethod));
		//CRC
		CRC = crc32(buff_begin, buff- buff_begin);
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	}
};
class IDAT :public Chunk
{
public:
	BYTE CompressionAlgorithm[3];//��ѹ�� 1byte
	WORD Len;//ͼ������(�ֽ���+1) 2byte
	WORD nLen;//len�Ĳ���0xFFFF-len 2byte
	BYTE b1;//ռλ�� 1byte
	DWORD *ImgData;//ͼ������
	BYTE ImgDataEnd[5];//β�� 5byte
	DWORD Adler32;//���� 4byte
public:
	void WriteMemoryAndCRC(BYTE* &buff)
	{
		DWORD tempDWORD = 0;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(ChunkLen)), sizeof(ChunkLen));
		auto buff_begin = buff;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Tag)), sizeof(Tag));
		Xmemmove(buff, CompressionAlgorithm, sizeof(CompressionAlgorithm));
		Xmemmove(buff, &Len, sizeof(Len));
		Xmemmove(buff, &nLen, sizeof(nLen));
		Xmemmove(buff, &b1, sizeof(b1));
		//ͼ������
		Xmemmove(buff, ImgData, Len-1);

		Xmemmove(buff, ImgDataEnd, sizeof(ImgDataEnd));
		Xmemmove(buff, &Adler32, sizeof(Adler32));
		//CRC
		CRC = crc32(buff_begin, buff - buff_begin);
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	};
};
class IEND:public Chunk
{
public:
	void WriteMemoryAndCRC(BYTE* &buff) 
	{
		DWORD tempDWORD = 0;

		Xmemmove(buff, &(tempDWORD = ChgDWORD(ChunkLen)), sizeof(ChunkLen));
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Tag)), sizeof(Tag));
		CRC = png_IEND_CRC;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	}
};
DWORD ChgDWORD(DWORD value)
{
	DWORD ret = 0;
	auto pret = (BYTE*)&ret;
	auto pvalue = (BYTE*)&value;
	*pret++ = pvalue[3];
	*pret++ = pvalue[2];
	*pret++ = pvalue[1];
	*pret++ = pvalue[0];
	return ret;

}
WORD ChgWORD(WORD value)
{
	__asm
	{
		mov ax , value
		xchg al, ah
		mov value, ax
	}
	return value;
}
#pragma pack (pop)
void CreatePNG(DWORD Witdh, DWORD Height, DWORD *ImgData, void *& out PNGBuff, int & PNGBuff_Len)
{
	//���㻺������С
	int buff_size = sizeof(png_Title)+sizeof(IHDR)+sizeof(IDAT)+sizeof(IEND);
	PNGBuff_Len = buff_size;
	//����ͼƬ���ݴ�С��-4��IDATA����һ��ͼ��ָ��
	buff_size += Witdh * Height * 4 - 4;
	//����������
	BYTE *buff = new BYTE[buff_size];
	PNGBuff = buff;

	//title
	Xmemmove(buff, (void *)png_Title, sizeof(png_Title));
	//head info
	IHDR head;
	head.ChunkLen = sizeof(IHDR)-sizeof(Chunk);
	head.Tag = png_IHDR;
	head.Width = Witdh;
	head.Height = Height;
	head.BitDepth = 8;
	head.ColorType = 6;
	head.CompressionMethod = 0;
	head.FilterMethod = 0;
	head.InterlaceMethod = 0;
	head.WriteMemoryAndCRC(buff);
	//���ݶ�1
	IDAT data1;
	data1.Tag = png_IDAT;
	memcpy(data1.CompressionAlgorithm, png_CompressionAlgorithm, sizeof(png_CompressionAlgorithm));
	data1.Len = Witdh*Height * 4 + 1;
	data1.nLen = 0xFFFF - data1.Len;
	data1.ImgData = ImgData;
	memcpy(data1.ImgDataEnd, png_ImgDataEnd, sizeof(png_ImgDataEnd));
	data1.ChunkLen = sizeof(IDAT) - sizeof(Chunk)+ data1.Len - 1;
	data1.Adler32 = 0;
	data1.b1 = 0;
	data1.WriteMemoryAndCRC(buff);

	IEND end;
	end.Tag = png_IEND;
	//IEND����Ϊ0
	end.ChunkLen = 0;
	end.WriteMemoryAndCRC(buff);
}
void main()
{
	DWORD ImgData[] = { 0xffffffff };
	void * buff = 0;
	int buff_len = 0;
	CreatePNG(1, 1, ImgData, buff, buff_len);
	auto filename = "D:/Demo/myex/Beta/img/testcrc.png";
	auto file = fopen(filename, "wb+");
	fwrite(buff, buff_len, 1, file);
	fclose(file);
	auto ret = 0;
}

