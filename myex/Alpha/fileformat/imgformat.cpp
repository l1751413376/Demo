#include "imgformat.h"
#include "JPEG.H"

void BmpFile::load(char *filename)
{
	open(filename);
	read(&fhead,sizeof(bmpFileHeader));
	read(&ihead,sizeof(bmpInformationHeader));

	if(fhead.bfType != 'MB')//文件标示错误
		return;

	auto tcolor=color = new DWORD[ihead.biWidth*ihead.biHeight];
	tcolor+=ihead.biWidth*ihead.biHeight-1;

	if(ihead.biBitCount=24)
	{
		int cp= (4 - ihead.biWidth % 4 +ihead.biWidth)*3;
		int p=ihead.biWidth*3;
		for (DWORD i = 0; i < ihead.biHeight; i++)
		{
			pfmoving+=p;
			for (DWORD k = 0; k < ihead.biWidth; k++)
			{
				memcpy(tcolor--, pfmoving-=3, 3);
			}
			pfmoving+=cp;
		}

	}
	close();
}

#pragma comment (lib,"project/lib/jpeglib.lib")

