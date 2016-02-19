#include "CRC.h"
#include <Windows.h>
namespace PNG {}

using namespace PNG;


#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static UINT64 png_Title = 0x89504E470D0A1A0A; //.PNG....
const static BYTE   png_CompressionAlgorithm[3] = { 0x78,0x01,0x00 };
const static BYTE   png_ImgDataEnd[5]= { 0x01,0x00,0x00,0xFF,0xFF };

#pragma pack (1)
struct IHDR
{
	DWORD Tag;
	DWORD Width;//��
	DWORD Height;//��
	BYTE  BitDepth;//λ�� ���ɫͼ��8��16
	BYTE  ColorType;//��ɫ���� 6������ͨ�����ݵ����ɫͼ��8��16
	BYTE  CompressionMethod;//ѹ������(LZ77�����㷨) �̶�0
	BYTE  FilterMethod;//�˲������� �̶�0
	BYTE  InterlaceMethod;//����ɨ�跽����0���Ǹ���ɨ�� 1�� Adam7(��Adam M.Costello������7�����ɨ�跽��)
	DWORD CRC;
};
struct IDAT
{
	DWORD Tag;
	BYTE CompressionAlgorithm[3];//��ѹ��
	WORD Len;//ͼ������(�ֽ���+1)
	WORD nLen;//len�Ĳ���0xFFFF-len
	BYTE b1;//ռλ��
	DWORD *ImgData;//ͼ������
	BYTE ImgDataEnd[5];//β�� 
	DWORD Adler32;//����
	DWORD CRC;//������
};
struct IEND 
{
	DWORD Tag;
	DWORD CRC;
};
#define write_byte(buff,src,size)  memcpy(buff,src,size) buff + = size
#pragma pack (pop)
void CreatePNG(DWORD Witdh, DWORD Height,DWORD *ImgData)
{
	
	//���㻺������С
	int buff_size = sizeof(png_Title)+4+sizeof(IHDR)+4+sizeof(IDAT)+4+sizeof(IEND);
	//����ͼƬ���ݴ�С
	buff_size += Witdh * Height * 4;
	//����������
	BYTE *buff = new BYTE[buff_size];
	
	//head info
	IHDR head = { 0 };
	head.Tag = png_IHDR;
	head.Width = Witdh;
	head.Height = Height;
	head.BitDepth = 8;
	head.ColorType = 6;
	head.CRC = crc32((uchar*)&head,sizeof(IHDR)-sizeof(DWORD));
	write_byte(buff, &head, sizeof(IHDR));
	//���ݶ�1
	IDAT data1 = { 0 };
	data1.Tag = png_IDAT;
	memcpy(data1.CompressionAlgorithm, png_CompressionAlgorithm, sizeof(png_CompressionAlgorithm));
	data1.Len = Witdh*Height * 4 + 1;
	data1.nLen = 0xFFFF - data1.Len;
	data1.ImgData = ImgData;
	memcpy(data1.ImgDataEnd, png_ImgDataEnd, sizeof(png_ImgDataEnd));

	IEND end = { png_IEND ,png_IEND_CRC };
}