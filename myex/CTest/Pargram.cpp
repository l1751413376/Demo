#include "zlib.h"
#pragma comment (lib,"lib/zlib.lib")
#include <iostream>
#include <io.h>
void main()
{
	auto filename="D:/MyConfiguration/ly09614.TCENT/Desktop/Save04.rvdata2";
	auto file = fopen(filename, "rb+");

	auto buff=new Byte[200000];
	fread(buff,200000,1,file);
	
	auto dest=new Byte[200000];
	uLongf size=200000;
	uncompress(dest,&size,buff,200000);


}