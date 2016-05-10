#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>

#include "XYMapParser.h"



void UnpackXYMap(const char* szMapDir)
{
	char szPath[1024] = { 0 };
	sprintf(szPath, "%s\\*.*", szMapDir);

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(szPath, &findData);	// 定位到该目录的第一个文件(“.”即根目录)
	if (hFind == INVALID_HANDLE_VALUE) return;

	// 查找每个文件
	while (true)
	{
		// 绕过"."和".."
		if (strcmp(findData.cFileName, ".")!=0 && strcmp(findData.cFileName, "..")!=0 && !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char szCurMapFile[256];
			sprintf(szCurMapFile, "%s\\%s", szMapDir, findData.cFileName);

			const char* szFormat = strrchr(findData.cFileName, '.');
			if (strcmp(szFormat, ".map")==0 || strcmp(szFormat, ".MAP")==0)
			{
				printf("正在解压: %s", szCurMapFile);

				CXYMapParser mapParser;
				mapParser.Open(szCurMapFile);

				printf("  √\n");
			}
		}

		// 查找下一个文件
		if (!FindNextFileA(hFind, &findData))
		{
			if (GetLastError() != ERROR_NO_MORE_FILES) return;
			break;
		}
	}

	FindClose(hFind);
}


int main()
{
	// 内存泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	char szBuffer[256];

	printf("请输入要解压的map目录: ");

	char szMapDir[256];
	gets(szBuffer);
	sscanf(szBuffer, "%s", szMapDir);

	UnpackXYMap(szMapDir);

	printf("\n生成完毕.按任意键继续.");
	getchar();

	return 0;
}