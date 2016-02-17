#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>

#include "XYMapParser.h"



void UnpackXYMap(const char* szMapDir)
{
	char szPath[1024] = { 0 };
	sprintf(szPath, "%s\\*.*", szMapDir);

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(szPath, &findData);	// ��λ����Ŀ¼�ĵ�һ���ļ�(��.������Ŀ¼)
	if (hFind == INVALID_HANDLE_VALUE) return;

	// ����ÿ���ļ�
	while (true)
	{
		// �ƹ�"."��".."
		if (strcmp(findData.cFileName, ".")!=0 && strcmp(findData.cFileName, "..")!=0 && !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char szCurMapFile[256];
			sprintf(szCurMapFile, "%s\\%s", szMapDir, findData.cFileName);

			const char* szFormat = strrchr(findData.cFileName, '.');
			if (strcmp(szFormat, ".map")==0 || strcmp(szFormat, ".MAP")==0)
			{
				printf("���ڽ�ѹ: %s", szCurMapFile);

				CXYMapParser mapParser;
				mapParser.Open(szCurMapFile);

				printf("  ��\n");
			}
		}

		// ������һ���ļ�
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
	// �ڴ�й©���
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	char szBuffer[256];

	printf("������Ҫ��ѹ��mapĿ¼: ");

	char szMapDir[256];
	gets(szBuffer);
	sscanf(szBuffer, "%s", szMapDir);

	UnpackXYMap(szMapDir);

	printf("\n�������.�����������.");
	getchar();

	return 0;
}