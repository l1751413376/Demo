#include "WasImage.h" 
#include <assert.h>



const int TYPE_ALPHA = 0x00;// 前2位

const int TYPE_ALPHA_PIXEL = 0x20;// 前3位 0010 0000

const int TYPE_ALPHA_REPEAT = 0x00;// 前3位

const int TYPE_FLAG = 0xC0;// 2进制前2位 1100 0000

const int TYPE_PIXELS = 0x40;// 以下前2位 0100 0000

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
* 文件标志 2字节， SP 53 50 / SP<br>
* 文件头大小 2字节， 不包含前四个字节 0C 00 / 12<br>
* 动画方向数 2字节 01 00 / 1<br>
* 每方向的帧数 2字节 01 00 / 1<br>
* 动画的宽度 2字节 80 02 / 640<br>
* 动画的高度 2字节 29 00 / 41<br>
* 动画的中心点 X 2字节， 有符号 00 00 / 0<br>
* 动画的中心点 Y 2字节, 有符号 00 00 / 0<br>
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
		OutputDebugString("不是有效的was文件\n");
		return FALSE;
	}

	imageHeaderSize = readUnsignedShort();
	spriteCount = readUnsignedShort();
	frameCount = readUnsignedShort();
	width = readUnsignedShort();
	height = readUnsignedShort();
	centerX = readUnsignedShort();
	centerY = readUnsignedShort();

	// 读取帧延时信息
	int nDelayLen = imageHeaderSize - WAS_IMAGE_HEADER_SIZE;
	if (nDelayLen < 0) 
	{
		assert("帧延时信息错误！");
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

	// 读取调色板
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
			// 帧偏移列表
			if (delayLine != NULL && n < nDelayLen) 
			{
				frames[i][n].delay = delayLine[n];
			}
			frames[i][n].addrOffset = readInt();
		}
	}

	for (int i = 0; i < spriteCount; i++) 
	{
		// 帧信息
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
				// 行像素数据偏移
				pFrame->lineOffsets[l] = readInt();
			}

			// 解析帧数据
			Parse(pFrame);

			// 生成静态图片
			pFrame->MakeImage(pGraphics);
		}
	}


	return TRUE;
}




/**
* 将图片一行RLE编码格式的数据解码,解码后的数据放到pixels中<br>
* 格式:低16位为[565]rgb颜色值,16-20位为alpha值(最大为0x1F);
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



// 接下来是图片段数据信息，格式如下：
// 段数据=类型(8比特)+数据
//
// 类型的格式如下：
// 类型有4种，用2比特表示：
//
// 00：表示alpha象素，剩下的6比特也是0时，数据段结束。--00000000
// 如果第3个比特是1时，剩下的5比特(0~31)为alpha层。并且以后的字节是象素引索。--001*****
// 否则，剩下的5比特(0~31)是alpha象素重复次数.并且以后的2字节是alpha,象素引索。
// 01：表示象素组，剩下的6比特(0~63)为数据段的长度。
// 10：表示重复象素 n 次，n 为剩下的6比特(0~63)表示。
// 11：表示跳过象素 n 个，n 为剩下的6比特(0~63)表示。





