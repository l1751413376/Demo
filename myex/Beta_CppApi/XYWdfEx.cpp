
#include "XYWdfEx.h"
#include <vector>

using namespace std;



vector<WdfFile*> WdfLibrary;


WdfFile* LoadWdf(char *filePath)
{
	for (auto i = WdfLibrary.begin(); i != WdfLibrary.end(); i++)
	{
		if (stricmp((*i)->FilePath, filePath) == 0)
		{
			return *i;//如果已经被加载
		}
	}
	//没有加载过
	__file* file = new __file();
	file->open(filePath);

	DWORD Flag = file->read_dword(); // 包裹的标签
	DWORD Number = file->read_dword(); // 包裹中的文件数量
	DWORD Offset = file->read_dword(); // 包裹中的文件列表偏移位置

	WdfFile* wdfFile = new WdfFile();
	wdfFile->WdfItems = new WdfItem[Number];
	wdfFile->WdfItemssNumber = Number;
	wdfFile->FileHandle = file;
	strcpy(wdfFile->FilePath, filePath);
	file->seek(Offset, SEEK_SET);
	file->read(wdfFile->WdfItems, sizeof(WdfItem)*Number);
	
	WdfLibrary.push_back(wdfFile);
	return wdfFile;
}

WdfItem* GetWasSizeByOffset(WdfFile* wdfFile, DWORD hash)
{
	for (DWORD i = 0; i < wdfFile->WdfItemssNumber; i++)
	{
		auto item = &wdfFile->WdfItems[i];
		if (item->Hash == hash)
		{
			return item;
		}
	}
	return 0;
}

