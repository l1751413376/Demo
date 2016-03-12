#include "WasFile.h"



const int TYPE_ALPHA = 0x00;// 前2位
const int TYPE_ALPHA_PIXEL = 0x20;// 前3位 0010 0000
const int TYPE_ALPHA_REPEAT = 0x00;// 前3位
const int TYPE_FLAG = 0xC0;// 2进制前2位 1100 0000
const int TYPE_PIXELS = 0x40;// 以下前2位 0100 0000
const int TYPE_REPEAT = 0x80;// 1000 0000
const int TYPE_SKIP = 0xC0; // 1100 0000
const int WAS_IMAGE_HEADER_SIZE = 12;
#define WAS_FILE_TAG 'PS'// 'SP'文件前缀



//颜色转换20位->32位
DWORD colour4565to8888(DWORD c)
{
	WORD wColor = 0;
	DWORD ret_c = 0;
	BYTE Alpha = 0;
	BYTE b = 0;
	BYTE g = 0;
	BYTE r = 0;
	BYTE a = 0;

	wColor = (WORD)(c & 0x00FFFF);
	Alpha = (BYTE)((c & 0xFF0000) >> 16);

	b = ((wColor & 0x001F) << 3);
	g = ((wColor & 0x07E0) >> 3);
	r = ((wColor & 0xF800) >> 8);
	// a = 值是0-31,  要转为0-255
	a = Alpha << 3;
	BYTE iArray[4];
	iArray[0] = b; // blue
	iArray[1] = g; // green
	iArray[2] = r; // red
	iArray[3] = a; // alpha
	memcpy(&ret_c, iArray, 4);
	return ret_c;
}

void WasImg::load_file(const char* strFileName)
{
	open((char *)strFileName);
	load_data(pfmoving);
	close();
}

void WasImg::load_data(const BYTE* pData)
{
	Release();

	UINT16 TAG = read_word();
	if (TAG != WAS_FILE_TAG)
	{
		OutputDebugString(L"不是有效的was文件\n");
		return;
	}

	imageHeaderSize = read_word();
	spriteCount = read_word();
	frameCount = read_word();
	width = read_word();
	height = read_word();
	centerX = read_word();
	centerY = read_word();

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
			delayLine[i] = read_byte();
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
		palette[i] = read_word();
	}

	frames = new WasFrame*[spriteCount];
	seek(imageHeaderSize + 4 + 512);
	for (int i = 0; i < spriteCount; i++)
	{
		frames[i] = new WasFrame[frameCount];
		for (int n = 0; n < frameCount; n++)
		{
			// 帧偏移列表
			if (delayLine != NULL && n < nDelayLen)
			{
				frames[i][n].delay = delayLine[n];
			}
			frames[i][n].addrOffset = read_dword();
		}
	}

	for (int i = 0; i < spriteCount; i++)
	{
		// 帧信息
		for (int n = 0; n < frameCount; n++)
		{
			WasFrame* pFrame = &frames[i][n];
			int offset = pFrame->addrOffset;
			if (offset == 0)
				continue;// blank frame
			seek(offset + imageHeaderSize + 4);
			pFrame->offX = read_dword();
			pFrame->offY = read_dword();
			pFrame->width = read_dword();
			pFrame->height = read_dword();
			pFrame->lineOffsets = new int[pFrame->height];
			ZeroMemory(pFrame->lineOffsets, pFrame->height * 4);
			for (int l = 0; l < pFrame->height; l++)
			{
				// 行像素数据偏移
				pFrame->lineOffsets[l] = read_dword();
			}

			// 解析帧数据
			Parse(pFrame);
		}
	}
}


/**
* 将图片一行RLE编码格式的数据解码,解码后的数据放到pixels中<br>
* 格式:低16位为[565]rgb颜色值,16-20位为alpha值(最大为0x1F);
*
* @throws IOException
*/

void WasImg::Parse(WasFrame* pFrame)
{
	int frameWidth = pFrame->width;
	int frameHeight = pFrame->height;
	pFrame->pixels = new DWORD*[frameHeight];
	for (int i = 0; i < frameHeight; i++)
	{
		pFrame->pixels[i] = new DWORD[frameWidth];
		ZeroMemory(pFrame->pixels[i], frameWidth * 4);
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
			typeFlag = read_byte();
			switch ((typeFlag & TYPE_FLAG))
			{
			case TYPE_ALPHA:
				if ((typeFlag & TYPE_ALPHA_PIXEL) > 0)
				{
					color = palette[read_byte()];
					alpha = typeFlag & 0x1F;
					pixels[y][x] = color + (alpha << 16);
					pixels[y][x] = colour4565to8888(pixels[y][x]);
					x++;
				}
				else if (typeFlag != 0)
				{
					count = typeFlag & 0x1F;// count
					alpha = read_byte();// alpha
					alpha = alpha & 0x1F;
					color = palette[read_byte()];
					for (int i = 0; i < count; i++)
					{
						pixels[y][x] = color + (alpha << 16);
						pixels[y][x] = colour4565to8888(pixels[y][x]);
						x++;
					}
				}
				else
				{
					// block end
					if (x > frameWidth)
					{
						char szError[256];
						sprintf_s(szError, "block end error: [%d][%d/%d]\n", y, x, frameWidth);
						OutputDebugStringA(szError);
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

				break;

			case TYPE_PIXELS:
				count = typeFlag & 0x3F;
				for (int i = 0; i < count; i++)
				{
					color = palette[read_byte()];
					alpha = 0x1F;
					pixels[y][x] = color + (alpha << 16);
					pixels[y][x] = colour4565to8888(pixels[y][x]);
					x++;
				}

				break;

			case TYPE_REPEAT:
				count = typeFlag & 0x3F;
				color = palette[read_byte()];
				alpha = 0x1F;
				for (int i = 0; i < count; i++)
				{
					pixels[y][x] = color + (alpha << 16);
					pixels[y][x] = colour4565to8888(pixels[y][x]);
					x++;
				}

				break;

			case TYPE_SKIP:
				count = typeFlag & 0x3F;
				x += count;
				break;
			}
		}

		if (x > frameWidth)
		{
			char szError[256];
			sprintf_s(szError, "block end error: [%d][%d/%d]", y, x, frameWidth);
			OutputDebugStringA(szError);
		}
	}
}