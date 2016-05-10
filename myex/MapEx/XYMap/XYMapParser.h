//----------------------------------------------------------------------------------------------------
// File  : XYMapParser.h
// Title : CXYMapParser ��������ϵ�е�ͼ��ȡ��(ֻ֧���λúʹ�2)
// Date  : 2013-4-1
// Desc  : 
//	[���ڱ�����]
//	������������λ���ߣ�һ����CSDN��freegodly,�ṩ��map�ļ���ʽ˵���Ͷ�ȡ��ʽ,
//  ��һ��������������,�ṩ��jpg���ݵĽ�ѹ,����λ������ǧ�︽��,�ṩ��mask���ݵĽ�ѹ�㷨,
//
// Copyright(c) WeiWuYuan
//----------------------------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <windows.h>



/*----------------------------------------------------------------------------------------------
��������2���λ����εĵ�ͼ�ļ���ʽ��

���ļ���ʼ����ʼ����Ϊ��
	[�ļ�ͷ]
	��4�ֽ�flag,ֵΪ1296126040,��"XPAM"
	��4�ֽڵ�ͼ���
	��4�ֽڵ�ͼ�߶�

	[��ͼ��ƫ����Ϣ]
	��4*block_num�ֽڣ�ÿ4���ֽڱ���1����ͼ���ƫ�Ƶ�ַ������block_num = ��ͼ�������*��ͼ����������������������ɵ�ͼ�ߴ�������

	[mask head����]
	��4�ֽ�δ֪����
	��4�ֽ�mask_num�����ڵ�ͼ������

	[mask���ݵ�ƫ����Ϣ]
	��4*mask_num�ֽڣ�ÿ4���ֽڱ���1��mask���ݵ�ƫ�Ƶ�ַ


��Ԫ���ݣ�

[��ͼ������]
�����ݵ�ͼ��ƫ����Ϣ����λ����ͼ�����ݵ�λ�ã������Դ�λ��Ϊ��ʼ
��4�ֽ�δ֪����
��4*n�ֽ����ݣ�nΪ��һ��4�ֽڵ�ֵ

��4�ֽ�jpg��ǣ�ֵΪ��0x4A504547����'GEPJ'
��4�ֽ�jpg���ݴ�С
��n�ֽ�jpg���ݣ�nΪ��һ��4�ֽڵ�ֵ

��4�ֽ�CELL���(��ͼ����)��ֵΪ��0x43454C4C����'LLEC'
��4�ֽ�CELL���ݴ�С
��n�ֽ�CELL���ݣ�nΪ��һ��4�ֽڵ�ֵ

��4�ֽ�BRIG���(��������)��ֵΪ��0x42524947����'GIRB'
��4�ֽ�BRIG���ݴ�С
��n�ֽ�BRIG���ݣ�nΪ��һ��4�ֽڵ�ֵ

��4�ֽڽ�����Ԫ

[mask����]
������maskƫ����Ϣ����λ��mask���ݵ�λ�ã������Դ�λ��Ϊ��ʼ
��4�ֽ�maskλ�ڵ�ͼ��x����
��4�ֽ�maskλ�ڵ�ͼ��y����
��4�ֽ�maskͼ��Ŀ��
��4�ֽ�maskͼ��ĸ߶�
��4�ֽ�mask���ݵĴ�С
��n�ֽ�mask���ݣ�nΪ��һ��4�ֽڵ�ֵ
/*----------------------------------------------------------------------------------------------*/



class CXYMapParser
{
public:
	CXYMapParser();
	~CXYMapParser();

public:
	struct SXYMap_Header
	{
		int				flag;		// 1296126040,��"XPAM"
		unsigned int	map_width;
		unsigned int	map_height;
	};

	struct SMaskHead
	{
		unsigned int	unknown;
		int				mask_num;
	};

	struct SMaskInfo
	{
		int		startX;
		int		startY;
		int		width;
		int		height;
		int		mask_size;	// mask���ݴ�С
	};

	struct SBlockInfo
	{
		unsigned int	UnitN;	// δ֪���ݴ�С
		unsigned int	flag;
		unsigned int	size;
	};

public:
	bool	Open(const char* szMapFile);
	void	Close();

private:
	void	ReadBlock(int block_index, int block_offset);
	void	ReadJPEG(int block_index, int size);
	void	ReadMask(int offset, int mask_index);
	BYTE*	JpgHandler(BYTE* Buffer, int inSize, UINT* outSize);
	int		DecompressMask(void* in , void* out);
	void	ByteSwap(WORD& value);
	void	WriteMapPixel(int block_index, WORD* pBmp16, int width, int height, int pitch);

private:
	void	ErrorDialog(const char* szFormat, ...);

public:
	enum
	{
		MAP_BLOCK_WIDTH		= 320,			// ��ͼ���ָ����N��320��240�Ŀ�
		MAP_BLOCK_HEIGHT	= 240,
	};

private:
	FILE*			m_pFile;
	SXYMap_Header	m_FileHeader;
	int				m_MapBlockRowNum;		// ��ͼ�������
	int				m_MapBlockColNum;		// ��ͼ�������

	WORD*			m_pMapBmp;				// ARGB1555��ʽ����
	int				m_MapBmpWidth;
	int				m_MapBmpHeight;
	char			m_szSrcMapFileName[256];
};