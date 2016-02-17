
#include "font_alpha.h"
#include "../g_logic/g_main.h"
#include <vector>
using namespace font_alpha;
using namespace std;

class FontFactory
{
private:
	const static int len=1000;
public:
	font_a **fontlist;
public:
	FontFactory()
	{
		fontlist=new font_a*[len];//1000个字
		memset(fontlist,0,sizeof(font_a*)*1000);
	};
	font_a* GetFont(UINT code)
	{
		int i = 0;
		//查找
		for (; i < len; i++)
		{
			auto item=fontlist[i];
			if(item==NULL)
				break;
			if(item->code==code)
				return item;
		}
		
		//造字
		if(i>=len)
		{
			//error 超过字体容量限制
			return NULL;
		}
		auto f=new font_a(code);
		fontlist[i]=f;
		return f;
	}
};

FontFactory* fontfactory;//字体工厂


HDC hdc;
GLYPHMETRICS glpm;
MAT2 mmat2={0,1,0,0,0,0,0,1};
TEXTMETRIC text;

//int bit[8]={1,2,4,8,16,32,64,128};
int bit[8]={128,64,32,16,8,4,2,1};
//初始化
void font_alpha::InitFont(HWND hwnd)
{
	hdc =GetDC(hwnd);
	HFONT font;
	font=CreateFontW(16,0,0,0,1,false,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"新宋体");
	font=(HFONT)SelectObject(hdc,font);
	GetTextMetricsW(hdc,&text);
	fontfactory=new FontFactory();
}

font_a::font_a(UINT code,int size)
{
	this->code=code;
	//从api获取字体
	DWORD nLen;
	GLYPHMETRICS glpm;
	nLen=GetGlyphOutlineA(hdc,code,GGO_BITMAP,&glpm,0,NULL,&mmat2);
	if(nLen<=0)
		return;
	BYTE* pBuf=new BYTE[nLen];
	auto i_pBuf=pBuf;
	GetGlyphOutlineA(hdc,code,GGO_BITMAP,&glpm,nLen,pBuf,&mmat2);
	//转换成8888颜色
	//上边距和左边距
	auto top=text.tmAscent-glpm.gmptGlyphOrigin.y;
	auto left=glpm.gmptGlyphOrigin.x;
	this->size=size;
	//大小
	height=text.tmHeight;
	width=glpm.gmCellIncX;
	//创建空间
	auto count=width*height;
	buff=new DWORD[count];
	memset(buff,0,count*4);
	auto i_buff=buff;
	i_buff+=top*width;//移动下来
	//pBuf 4字节对齐而且要取位
	UINT ret;
	int byte_align=(ret = glpm.gmBlackBoxY % 16) > 0 ? glpm.gmBlackBoxY / 16 + 1 : glpm.gmBlackBoxY / 16;
	byte_align=byte_align*4;
	//复制像素
	for (UINT box_y = 0; box_y < glpm.gmBlackBoxY; box_y++)
	{

		for(int box_x=0; box_x < glpm.gmCellIncX; box_x++)
		{
			auto item=*(i_pBuf+(box_x>>3));
			if((item&bit[box_x%8])>0)
			{
				*(i_buff+left+box_x)=0xffffff;
			}
		}
		i_pBuf+=byte_align;
		i_buff+=width;
	}
	delete []pBuf;
};


//显示文本
void font_alpha::Draw_Text(char * str,RECT* rect,DWORD colour,int align)
{
	//空判断
	if(str==NULL) return;

	BYTE item;
	UINT item_code=0;
	RECT div={0};
	vector<font_a *> vec;
	//查字
	div.bottom=text.tmHeight;
	while ((item=*str)!='\0')
	{
		str++;
		if(item>=128)
		{
			item_code=((UINT)item)<<8;
			item_code+=*str;
			str++;
			item_code+=256;//中文需要加上256?
		}
		else
		{
			item_code=item;
		}
		auto f=fontfactory->GetFont(item_code);
		if(f==NULL) continue;
		vec.push_back(f);
		div.right+=f->width;
	}
	//将多个字放到一起
	DWORD *buff=new DWORD[div.bottom*div.right];
	memset(buff,0,div.bottom*div.right*sizeof(DWORD));
	int left=0;
	for (int i = 0,length=vec.size(); i < length; i++)
	{
		auto item_f=vec[i];
		auto item_f_buff=item_f->buff;
		auto i_buff=buff;
		for (int f_y = 0; f_y < item_f->height; f_y++)
		{
			for (int f_x = 0; f_x < item_f->width; f_x++)
			{
				auto pix=*(item_f_buff+f_x);
				if(pix==0) continue;
				*(i_buff+f_x+left)=colour;
			}
			i_buff+=div.right;
			item_f_buff+=item_f->width;
		}
		left+=item_f->width;
	}
	//定位
	if(align==align_left)
	{
		div.left=rect->left;
		div.top=rect->top;
		div.bottom+=div.top;
		div.right+=div.left;
	}
	else
	{
		int width_inc=rect->right-rect->left-div.right;
		if(width_inc>0)
		{
			div.left=rect->left+(width_inc>>1);
			div.top=rect->top;
			div.bottom+=div.top;
			div.right+=div.left;
		}
		else
		{
			div.left=rect->left;
			div.top=rect->top;
			div.bottom+=div.top;
			div.right+=div.left;
		}
	}
	BltBmp(&div,buff);
	delete[] buff;
}

