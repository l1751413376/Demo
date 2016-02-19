
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#pragma warning( disable : 4996 )//忽略非安全函数
using namespace std;
#define OFFSET_CODING_LENGTH    (10)
#define MAX_WND_SIZE            1024

#define OFFSET_MASK_CODE        (MAX_WND_SIZE-1)

const ULONG     m=3;


UCHAR   __buffer1__[0x200000];
UCHAR   __buffer2__[0x200000];



void
Write1ToBitStream(
   PUCHAR  pBuffer,
   ULONG   ulBitOffset
   )
{
   ULONG   ulByteBoundary;
   ULONG   ulOffsetInByte;

   ulByteBoundary = ulBitOffset>>3 ;
   ulOffsetInByte = ulBitOffset&7;

   *(pBuffer+ulByteBoundary) |= (1<<ulOffsetInByte);
}

void
Write0ToBitStream(
   PUCHAR  pBuffer,
   ULONG   ulBitOffset
   )
{
   ULONG   ulByteBoundary;
   ULONG   ulOffsetInByte;

   ulByteBoundary = ulBitOffset>>3 ;
   ulOffsetInByte = ulBitOffset&7;

   *(pBuffer+ulByteBoundary) &= (~(1<<ulOffsetInByte));
}

ULONG
ReadBitFromBitStream(
   PUCHAR  pBuffer,
   ULONG   ulBitOffset
   )
{
   ULONG   ulByteBoundary;
   ULONG   ulOffsetInByte;

   ulByteBoundary = ulBitOffset>>3 ;
   ulOffsetInByte = ulBitOffset&7;

   return ((*(PULONG)(pBuffer+ulByteBoundary))>>ulOffsetInByte)&1 ;
}


ULONG WINAPI
WriteGolombCode(
   ULONG   x,
   PUCHAR  pBuffer,
   ULONG   ulBitOffset
   )
{
   ULONG           q, r;
   int             i;

   q = (x-1)>>m;
   r = x-(q<<m)-1;

   for(i=0; (ULONG)i<q; i++, ulBitOffset++)
   {
       Write1ToBitStream(pBuffer, ulBitOffset);
   }
   Write0ToBitStream(pBuffer, ulBitOffset);
   ulBitOffset++;

   for(i=0; i<m; i++, ulBitOffset++)
   {
       if( (r>>i)&1 )
       {
           Write1ToBitStream(pBuffer, ulBitOffset);
       }
       else
       {
           Write0ToBitStream(pBuffer, ulBitOffset);
       }
   }

   return m+q+1;
}


ULONG
ReadGolombCode(
   PULONG  pulCodingLength,
   PUCHAR  pBuffer,
   ULONG   ulBitOffset
   )
{
   ULONG   q, r;
   ULONG   bit;
   int i;

   for(q=0; ;q++)
   {
       bit = (ULONG)ReadBitFromBitStream(pBuffer, ulBitOffset);
       ulBitOffset++;
       if( !bit )
       {
           break;
       }
   }


   for(i=0, r=0; (ULONG)i<m; i++, ulBitOffset++)
   {
       bit = (ULONG)ReadBitFromBitStream(pBuffer, ulBitOffset);
       bit <<= i;
       r |= bit;
   }

   *pulCodingLength = m + q + 1;

   return r+(q<<m)+1;
}


ULONG
CompareStrings(
   PUCHAR  string1,
   PUCHAR  string2,
   ULONG   length
   )
{
   ULONG       i;
   PUCHAR      p1, p2;

   p1 = string1;
   p2 = string2;

   for(i=0; i<length; i++)
   {
       if( *p1==*p2 )
       {
           p1++;
           p2++;
       }
       else
       {
           break;
       }
   }

   return p1-string1;
}


void WINAPI
FindLongestSubstring(
   PUCHAR  pSourceString,
   PUCHAR  pString,
   ULONG   ulSourceStringLength,
   PULONG  pulSubstringOffset,
   PULONG  pulSubstringLength
   )
{
   PUCHAR  pSrc;

   ULONG   offset, length;

   ULONG   ulMaxLength;


   *pulSubstringOffset = offset = 0;
   *pulSubstringLength = 0;

   if( NULL==pSourceString || NULL==pString )
   {
       return;
   }

   ulMaxLength = ulSourceStringLength;
   pSrc = pSourceString;

   while( ulMaxLength>0 )
   {
       length = CompareStrings(pSrc, pString, ulMaxLength);

       if( length>*pulSubstringLength )
       {
           *pulSubstringLength = length;
           *pulSubstringOffset = offset;
       }

       pSrc++;
       offset++;
       ulMaxLength--;
   }
}





void
WriteBits(
   PUCHAR  pDataBuffer,
   ULONG   ulOffsetToWrite,
   ULONG   ulBits,
   ULONG   ulBitLength
   )
{
   ULONG   ulDwordsOffset;
   ULONG   ulBitsOffset, ulBitsRemained;

   ulDwordsOffset = ulOffsetToWrite>>5;
   ulBitsOffset = ulOffsetToWrite&31;
   ulBitsRemained = 32 - ulBitsOffset;

   if( 0==ulBitsOffset )
   {
       *((PULONG)pDataBuffer+ulDwordsOffset) = ulBits;
   }
   else if( ulBitsRemained>=ulBitLength )
   {
       *((PULONG)pDataBuffer+ulDwordsOffset) |= (ulBits<<ulBitsOffset);
   }
   else
   {
       *((PULONG)pDataBuffer+ulDwordsOffset) |= (ulBits<<ulBitsOffset);
       *((PULONG)pDataBuffer+ulDwordsOffset+1) = ulBits>>ulBitsRemained;
   }
}

void
ReadBits(
   PUCHAR  pDataBuffer,
   ULONG   ulOffsetToRead,
   PULONG  pulBits
   )
{
   ULONG   ulDwordsOffset;
   ULONG   ulBitsOffset, ulBitsLength;

   ulDwordsOffset = ulOffsetToRead>>5;
   ulBitsOffset = ulOffsetToRead&31;
   ulBitsLength = 32 - ulBitsOffset;


   *pulBits = *((PULONG)pDataBuffer+ulDwordsOffset);
   if( 0!=ulBitsOffset )
   {
       (*pulBits) >>= ulBitsOffset;
       (*pulBits) |= (*((PULONG)pDataBuffer+ulDwordsOffset+1))<<ulBitsLength;
   }
}

void
lz77compress(
   PUCHAR  pDataBuffer,
   ULONG   ulDataLength,
   PUCHAR  pOutputBuffer,
   PULONG  pulNumberOfBits
   )
{
   LONG        iSlideWindowPtr;
   ULONG       ulBytesCoded;
   ULONG       ulMaxlength;
   PUCHAR      pSlideWindowPtr;
   PUCHAR      pUnprocessedDataPtr;

   ULONG   offset;
   ULONG   length;
   ULONG   ulCodingLength;

   ULONG   ulBitOffset;
   UCHAR   cc;

   int     i;

   iSlideWindowPtr = -MAX_WND_SIZE;
   pSlideWindowPtr = NULL;
   ulBitOffset = 0;
   ulBytesCoded = 0;

   while( ulBytesCoded<ulDataLength )
   {
       if( iSlideWindowPtr>=0 )
       {
           pSlideWindowPtr = pDataBuffer+iSlideWindowPtr;
           ulMaxlength = MAX_WND_SIZE;

       }
       else if( iSlideWindowPtr>=-MAX_WND_SIZE )
       {
           pSlideWindowPtr = pDataBuffer;
           ulMaxlength = MAX_WND_SIZE + iSlideWindowPtr;
       }
       else
       {
           pSlideWindowPtr = NULL;
           ulMaxlength = 0;
       }

       pUnprocessedDataPtr = pDataBuffer + ulBytesCoded;
       if( ulMaxlength>ulDataLength-ulBytesCoded )
       {
           ulMaxlength = ulDataLength-ulBytesCoded;
       }

       FindLongestSubstring(
           pSlideWindowPtr,
           pUnprocessedDataPtr,
           ulMaxlength,
           &offset,
           &length
           );

       assert( length<=MAX_WND_SIZE );
       assert( offset<MAX_WND_SIZE );

       if(length>1)
       {

           Write1ToBitStream(pOutputBuffer, ulBitOffset);
           ulBitOffset++;

           for(i=0; i<OFFSET_CODING_LENGTH; i++, ulBitOffset++)
           {
               if( (offset>>i)&1 )
               {
                   Write1ToBitStream(pOutputBuffer, ulBitOffset);
               }
               else
               {
                   Write0ToBitStream(pOutputBuffer, ulBitOffset);
               }
           }

           ulCodingLength = WriteGolombCode(length, pOutputBuffer, ulBitOffset);

           ulBitOffset += ulCodingLength;
           iSlideWindowPtr += length;
           ulBytesCoded += length;

       }
       else
       {
           Write0ToBitStream(pOutputBuffer, ulBitOffset);
           ulBitOffset++;

           cc = (*pUnprocessedDataPtr);
           for(i=0; i<8; i++, ulBitOffset++)
           {
               if( (cc>>i)&1 )
               {
                   Write1ToBitStream(pOutputBuffer, ulBitOffset);
               }
               else
               {
                   Write0ToBitStream(pOutputBuffer, ulBitOffset);
               }
           }

           iSlideWindowPtr++;
           ulBytesCoded++;
       }

   }

   if( ulBytesCoded!=ulDataLength )
   {
       assert(ulBytesCoded==ulDataLength);
   }

   *pulNumberOfBits = ulBitOffset;

}


void lz77decompress(
   PUCHAR  pDataBuffer,
   ULONG   ulNumberOfBits,
   PUCHAR  pOutputBuffer,
   PULONG  pulNumberOfBytes
   )
{
   LONG        iSlideWindowPtr;
   PUCHAR      pSlideWindowPtr;

   ULONG   length, offset;
   ULONG   bit;
   UCHAR   cc;
   int     i;

   ULONG   ulBytesDecoded;
   ULONG   ulBitOffset;

   ULONG   ulCodingLength;
   PUCHAR  pWrite;

   iSlideWindowPtr = -MAX_WND_SIZE;
   pWrite = (PUCHAR)pOutputBuffer;
   ulBitOffset = 0;
   ulBytesDecoded = 0;


   while( ulBitOffset<ulNumberOfBits )
   {
       bit = ReadBitFromBitStream(pDataBuffer, ulBitOffset);
       ulBitOffset++;

       if( bit )
       {
           if( iSlideWindowPtr>=0 )
           {
               pSlideWindowPtr = pOutputBuffer + iSlideWindowPtr;

           }
           else if( iSlideWindowPtr>=-MAX_WND_SIZE )
           {
               pSlideWindowPtr = pOutputBuffer;
           }
           else
           {
               pSlideWindowPtr = NULL;
           }


           for(i=0, offset=0; i<OFFSET_CODING_LENGTH; i++, ulBitOffset++)
           {
               bit = ReadBitFromBitStream(pDataBuffer, ulBitOffset);
               offset |= (bit<<i);
           }

           length= ReadGolombCode(&ulCodingLength, pDataBuffer, ulBitOffset);

           assert(offset<MAX_WND_SIZE);

           if( length>MAX_WND_SIZE )
           {
               assert(length<=MAX_WND_SIZE);
           }
           ulBitOffset += ulCodingLength;

           RtlMoveMemory(pWrite, pSlideWindowPtr+offset, length);
           pWrite+=length;
           iSlideWindowPtr+=length;
           ulBytesDecoded+=length;
       }
       else
       {
           for(i=0, cc=0; i<8 ; i++, ulBitOffset++)
           {
               bit = ReadBitFromBitStream(pDataBuffer, ulBitOffset);
               cc |= ((UCHAR)bit<<i);
           }

           *pWrite++ = cc;
           iSlideWindowPtr++;
           ulBytesDecoded++;
       }

   }

   *pulNumberOfBytes = ulBytesDecoded;
}

extern "C"
void WINAPI
LZ77Compress(
   PUCHAR  __pDataBuffer,
   ULONG   __ulDataLength,
   PUCHAR  __pOutputBuffer,
   PULONG  __pulNumberOfBits
   );

extern "C"
void WINAPI
LZ77Decompress(
   PUCHAR  __pDataBuffer,
   ULONG   __ulNumberOfBits,
   PUCHAR  __pOutputBuffer,
   PULONG  __pulNumberOfBytes
   );

int
main2(
      )
{
   FILE    *fp=NULL;
   FILE    *fp1;
   ULONG   fsize;
   ULONG   ulNumberOfBits;
   ULONG   ulFileCompressedSize;
   ULONG   ulFileDecompressedSize;
   SYSTEMTIME      t1, t2;

 
   fp = fopen("1.txt", "rb");
   if( !fp )
   {
       return -1;
   }

   fseek(fp, 0, SEEK_END);
   fsize = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   fread(__buffer1__, 1, fsize, fp);


   GetSystemTime(&t1);
   lz77compress(__buffer1__, fsize, __buffer2__, &ulNumberOfBits);
  
   GetSystemTime(&t2);
   ulFileCompressedSize = ((ulNumberOfBits+7)>>3);

   fp1=fopen("2.txt", "wb+");
   if( !fp1 )
   {
       goto l1;
   }
   fwrite(__buffer2__, 1, ulFileCompressedSize, fp1);
   fclose(fp1);

   //RtlZeroMemory(__buffer1__, sizeof(__buffer1__));

   lz77decompress(__buffer1__, ulNumberOfBits, __buffer2__, &ulFileDecompressedSize);
   
   fp1=fopen("3.txt", "wb+");
   if( !fp1 )
   {
       goto l1;
   }
   fwrite(__buffer2__, 1, ulFileDecompressedSize, fp1);
   fclose(fp1);

l1:
   if( fp )
   {
       fclose(fp);
   }

   ULONG   milliS;

   milliS = ((t2.wHour - t1.wHour)*3600 + (t2.wMinute-t1.wMinute)*60 + (t2.wSecond-t1.wSecond)) * 1000 + (t2.wMilliseconds-t1.wMilliseconds);
  // printf("Totally %ld milliseconds elapsed!\n\n", milliS);
cout<<"压缩算法的实现"<<endl;
cout<<endl;
cout<<endl;
cout<<"成功完成压缩与解压缩！"<<endl;
cout<<endl;
cout<<"具体情况请参看生成的2.txt,3.txt文件"<<endl;
cout<<endl;
cout<<"1.txt及为要压缩的源文件"<<endl;
cout<<endl;
cout<<"2.txt及为压缩后所得文件"<<endl;
cout<<endl;
cout<<"3.txt及为解压后所得文件"<<endl;

   printf("Press any key to exit!\n");
   getch();

   return 0;
}
