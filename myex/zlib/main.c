#include"zlib.h"
#include<Windows.h>
void main() 
{
	Byte  source[] = {1,1,1,3,3,4,5};
	Byte destbuff[100];
	memset(destbuff,0,sizeof(destbuff));
	uLong destlen=100;
	compress2(destbuff,&destlen, source, sizeof(source), Z_NO_COMPRESSION);
}