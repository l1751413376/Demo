#include "WasImage.h" 
#include <assert.h>



const int TYPE_ALPHA = 0x00;// ǰ2λ

const int TYPE_ALPHA_PIXEL = 0x20;// ǰ3λ 0010 0000

const int TYPE_ALPHA_REPEAT = 0x00;// ǰ3λ

const int TYPE_FLAG = 0xC0;// 2����ǰ2λ 1100 0000

const int TYPE_PIXELS = 0x40;// ����ǰ2λ 0100 0000

const int TYPE_REPEAT = 0x80;// 1000 0000

const int TYPE_SKIP = 0xC0; // 1100 0000

const int WAS_IMAGE_HEADER_SIZE = 12;

//const int WAS_FILE_TAG = "SP";
//const int WAS_FILE_TAG_INT = 'PS';

#define WAS_FILE_TAG			'PS'	// 'SP'



CWASImage::CWASImage()
{
	m_pWasFileDataMoving = NULL;
	m_pWasFileData = NULL;
	width = 0;
	height = 0;
	centerX = 0;
	centerY = 0;
	delayLine = NULL;
	frameCount = 0;
	frames = NULL;
	imageHeaderSize = 0;
	ZeroMemory(name, sizeof(name));
	ZeroMemory(palette, sizeof(palette));
	//ZeroMemory(path, sizeof(path));
	spriteCount = 0;
}

CWASImage::~CWASImage()
{
	Release();
}


void CWASImage::Release()
{
	if (delayLine != NULL)
	{
		delete[] delayLine;
		delayLine = NULL;
	}

	if (frames != NULL)
	{
		for (int i=0; i<spriteCount; i++)
		{
			delete[] frames[i];
		}
		delete[] frames;
		frames = NULL;
	}

	m_pWasFileDataMoving = NULL;
	m_pWasFileData = NULL;
}


BOOL CWASImage::LoadWAS(CGraphics* pGraphics, const char* strFileName)
{
	HANDLE file=CreateFile(strFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( file == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	strcpy(name, strFileName);

	DWORD dwFileSize = GetFileSize(file, NULL);

	BYTE *pFileData = new BYTE[dwFileSize];

	DWORD dwLen;
	ReadFile(file,(void*)pFileData,sizeof(BYTE)*dwFileSize,&dwLen,NULL);

	assert(dwFileSize == dwLen);

	BOOL br = this->LoadWAS(pGraphics, pFileData, dwFileSize);

	delete[] pFileData;

	CloseHandle(file);

	return br;
}


/**
* �ļ���־ 2�ֽڣ� SP 53 50 / SP<br>
* �ļ�ͷ��С 2�ֽڣ� ������ǰ�ĸ��ֽ� 0C 00 / 12<br>
* ���������� 2�ֽ� 01 00 / 1<br>
* ÿ�����֡�� 2�ֽ� 01 00 / 1<br>
* �����Ŀ�� 2�ֽ� 80 02 / 640<br>
* �����ĸ߶� 2�ֽ� 29 00 / 41<br>
* ���������ĵ� X 2�ֽڣ� �з��� 00 00 / 0<br>
* ���������ĵ� Y 2�ֽ�, �з��� 00 00 / 0<br>
* 
* @param filename
*/
BOOL CWASImage::LoadWAS(CGraphics* pGraphics, const void* pData, UINT nDatLen)
{
	Release();

	m_pWasFileData = (const BYTE*)pData;
	m_pWasFileDataMoving = m_pWasFileData;

	UINT16 TAG = readUnsignedShort();
	if (TAG != WAS_FILE_TAG)
	{
		OutputDebugString("������Ч��was�ļ�\n");
		return FALSE;
	}

	imageHeaderSize = readUnsignedShort();
	spriteCount = readUnsignedShort();
	frameCount = readUnsignedShort();
	width = readUnsignedShort();
	height = readUnsignedShort();
	centerX = readUnsignedShort();
	centerY = readUnsignedShort();

	// ��ȡ֡��ʱ��Ϣ
	int nDelayLen = imageHeaderSize - WAS_IMAGE_HEADER_SIZE;
	if (nDelayLen < 0) 
	{
		assert("֡��ʱ��Ϣ����");
	} 
	else if (nDelayLen > 0) 
	{
		delayLine = new BYTE[nDelayLen];
		for (int i = 0; i < nDelayLen; i++) 
		{
			delayLine[i] = readByte();
		}
	} 
	else
	{
		delayLine = NULL;
	}

	// ��ȡ��ɫ��
	seek(imageHeaderSize + 4);
	for (int i = 0; i < 256; i++) 
	{
		palette[i] = readUnsignedShort();
	}

	frames = new CWASFrame*[spriteCount];
	seek(imageHeaderSize + 4 + 512);
	for (int i = 0; i < spriteCount; i++) 
	{
		frames[i] = new CWASFrame[frameCount];
		for (int n = 0; n < frameCount; n++) 
		{
			// ֡ƫ���б�
			if (delayLine != NULL && n < nDelayLen) 
			{
				frames[i][n].delay = delayLine[n];
			}
			frames[i][n].addrOffset = readInt();
		}
	}

	for (int i = 0; i < spriteCount; i++) 
	{
		// ֡��Ϣ
		for (int n = 0; n < frameCount; n++)
		{
			CWASFrame* pFrame = &frames[i][n];
			int offset = pFrame->addrOffset;
			if (offset == 0)
				continue;// blank frame
			seek(offset + imageHeaderSize + 4);
			pFrame->offX = readInt();
			pFrame->offY = readInt();
			pFrame->width = readInt();
			pFrame->height = readInt();
			pFrame->lineOffsets = new int[pFrame->height];
			ZeroMemory(pFrame->lineOffsets, pFrame->height*4);
			for (int l = 0; l < pFrame->height; l++) 
			{
				// ����������ƫ��
				pFrame->lineOffsets[l] = readInt();
			}

			// ����֡����
			Parse(pFrame);

			// ���ɾ�̬ͼƬ
			pFrame->MakeImage(pGraphics);
		}
	}


	return TRUE;
}




/**
* ��ͼƬһ��RLE�����ʽ�����ݽ���,���������ݷŵ�pixels��<br>
* ��ʽ:��16λΪ[565]rgb��ɫֵ,16-20λΪalphaֵ(���Ϊ0x1F);
* 
* @throws IOException
*/

void CWASImage::Parse(CWASFrame* pFrame)
{
	int frameWidth = pFrame->width;
	int frameHeight = pFrame->height;
	pFrame->pixels = new DWORD*[frameHeight];
	for (int i=0; i<frameHeight; i++)
	{
		pFrame->pixels[i] = new DWORD[frameWidth];
		ZeroMemory(pFrame->pixels[i], frameWidth*4);
	}
	DWORD** pixels = pFrame->pixels;
	int x = 0;
	int y = 0;
	BYTE typeFlag = 0;
	BYTE count = 0;
	DWORD color = 0;
	DWORD alpha = 0;

	for (y = 0; y < frameHeight; y++)
	{
		x = 0;
		seek(pFrame->lineOffsets[y] + pFrame->addrOffset + imageHeaderSize + 4);
		while (x < frameWidth) 
		{
			typeFlag = readByte();
			switch ((typeFlag & TYPE_FLAG)) 
			{
			case TYPE_ALPHA:
				{
					if ((typeFlag & TYPE_ALPHA_PIXEL) > 0) 
					{
						color = palette[readByte()];
						alpha = typeFlag & 0x1F;
						pixels[y][x] = color + (alpha << 16);
						x++;
					} 
					else if (typeFlag != 0) 
					{
						// ???
						count = typeFlag & 0x1F;// count
						alpha = readByte();// alpha
						alpha = alpha & 0x1F;
						color = palette[readByte()];
						for (int i = 0; i < count; i++) 
						{
							pixels[y][x] = color + (alpha << 16);
							x++;
						}
					} 
					else 
					{
						// block end
						if (x > frameWidth) 
						{
							char szError[256];
							sprintf(szError, "block end error: [%d][%d/%d]\n", y, x, frameWidth);
							OutputDebugString(szError);
							continue;
						}
						else if (x == 0)
						{
							// System.err.println("x==0");
						} 
						else 
						{
							x = frameWidth;// set the x value to break the 'while' sentences
						}
					}
				}
				break;

				case TYPE_PIXELS:
					{
						count = typeFlag & 0x3F;
						for (int i = 0; i < count; i++) 
						{
							color = palette[readByte()];
							alpha = 0x1F;
							pixels[y][x] = color + (alpha << 16);
							x++;
						}
					}
					break;

				case TYPE_REPEAT:
					{
						count = typeFlag & 0x3F;
						color = palette[readByte()];
						alpha = 0x1F;
						for (int i = 0; i < count; i++) 
						{
							pixels[y][x] = color + (alpha << 16);
							x++;
						}
					}
					break;

				case TYPE_SKIP:
					{
						count = typeFlag & 0x3F;
						x += count;
					}
					break;
			}
		}

		if (x > frameWidth)
		{
			// System.err.println("block end error: [" + y + "][" + x + "/" + frameWidth + "]");
			char szError[256];
			sprintf(szError, "block end error: [%d][%d/%d]", y,x,frameWidth);
			OutputDebugString(szError);
		}
	}
}



// ��������ͼƬ��������Ϣ����ʽ���£�
// ������=����(8����)+����
//
// ���͵ĸ�ʽ���£�
// ������4�֣���2���ر�ʾ��
//
// 00����ʾalpha���أ�ʣ�µ�6����Ҳ��0ʱ�����ݶν�����--00000000
// �����3��������1ʱ��ʣ�µ�5����(0~31)Ϊalpha�㡣�����Ժ���ֽ�������������--001*****
// ����ʣ�µ�5����(0~31)��alpha�����ظ�����.�����Ժ��2�ֽ���alpha,����������
// 01����ʾ�����飬ʣ�µ�6����(0~63)Ϊ���ݶεĳ��ȡ�
// 10����ʾ�ظ����� n �Σ�n Ϊʣ�µ�6����(0~63)��ʾ��
// 11����ʾ�������� n ����n Ϊʣ�µ�6����(0~63)��ʾ��





