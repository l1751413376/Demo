#include <Windows.h>
namespace PNG {}

using namespace PNG;


#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static UINT64 png_Title = 0x89504E470D0A1A0A; //.PNG....

struct IHDR
{
	DWORD Tag= png_IHDR;
	DWORD Width;//��
	DWORD Height;//��
	BYTE  BitDepth;//λ�� ���ɫͼ��8��16
	BYTE  ColorType;//��ɫ���� 6������ͨ�����ݵ����ɫͼ��8��16
	BYTE  CompressionMethod;//ѹ������(LZ77�����㷨) �̶�0
	BYTE  FilterMethod;//�˲������� �̶�0
	BYTE  InterlaceMethod;//����ɨ�跽����0���Ǹ���ɨ�� 1�� Adam7(��Adam M.Costello������7�����ɨ�跽��)
	DWORD CRC;
};


#pragma pack (1)
struct IDAT
{
	DWORD Tag = png_IHDR;
	BYTE CompressionAlgorithm[3] = { 0x78,0x01,0x00 };//��ѹ��
	WORD Len;//ͼ������(�ֽ���+1)
	WORD nLen;//len�Ĳ���0xFFFF-len
	BYTE b1;//ռλ��
	DWORD ImgData[1];//ͼ������
	BYTE ImgDataEnd[5] = {0x01,0x00,0x00,0xFF,0xFF};//β�� 
	DWORD Adler32;//����
	DWORD CRC;//������
};
#pragma pack (pop)

struct IEND 
{
	DWORD Tag = png_IEND;
	DWORD CRC = png_IEND_CRC;
};