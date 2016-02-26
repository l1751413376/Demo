#pragma once
namespace CRC {
	typedef unsigned int uint;
	static uint   CRC32[256];
	static char   init = 0;
	typedef unsigned char uchar;
	//初始化表
	static void init_table()
	{
		int   i, j;
		uint   crc;
		for (i = 0;i < 256;i++)
		{
			crc = i;
			for (j = 0;j < 8;j++)
			{
				if (crc & 1)
				{
					crc = (crc >> 1) ^ 0xEDB88320;
				}
				else
				{
					crc = crc >> 1;
				}
			}
			CRC32[i] = crc;
		}
	}

	//crc32实现函数
	uint crc32(uchar *buf, int len)
	{
		uint ret = 0xFFFFFFFF;
		int   i;
		if (!init)
		{
			init_table();
			init = 1;
		}
		for (i = 0; i < len;i++)
		{
			ret = CRC32[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
		}
		ret = ~ret;
		return ret;
	}
}