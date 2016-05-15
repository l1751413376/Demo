using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Beta
{
    public static class CppAPI
    {
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static int add(int x, int y);
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static int GetBitMap([MarshalAs(UnmanagedType.LPStr)]string filename,out IntPtr Pointer,out int len);
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static void GetWasFileInfo([MarshalAs(UnmanagedType.LPStr)]string filename, out IntPtr ptr, out int directionCount, out int frameCount, out int width, out int height, out int centerX, out int centerY);
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static void GetWasFrame(int width, int height, IntPtr wasPtr, int directionIndex, int frameIndex, out IntPtr dataptr, out int dataptrLen);
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static void Delete(IntPtr ptr );
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static void GetWasFileInfoFromWdfFile([MarshalAs(UnmanagedType.LPStr)]string filename, out IntPtr ptr, out int directionCount, out int frameCount, out int width, out int height, out int centerX, out int centerY, uint hash);
        [DllImport(@"D:\Demo\myex\Debug\Beta_CppApi.dll")]
        public extern static void GetXYMap([MarshalAs(UnmanagedType.LPStr)]string FileName, out IntPtr mapbuff, out int mapbuffsize);
    }


    /*
     * C# C++ 字符串传递
     * 
     * #include "stdafx.h"
 2#include "TestDll.h"
 3#include <stdio.h>
 4#include <string.h>
 5#include <tchar.h>
 6
 7
 8 staticchar* _hello ="Hello,World!!";
 9 static TCHAR * _helloW = TEXT("Hello,World!!");
10
11 void __stdcall PrintString(char* hello)
12{
13    printf("%s\n",hello);
14}
15
16 void __stdcall PrintStringW(TCHAR * hello)
17{
18    _tprintf(TEXT("%s\n"),hello);
19}
20
21
22 char*  __stdcall GetStringReturn()
23{
24return _hello;
25}
26
27TCHAR * __stdcall GetStringReturnW()
28{
29return _helloW;
30}
31
32
33 void __stdcall GetStringParam(char* outHello,int len)
34{  //output "aaaaaaaa"
35 for(int i=0; i< len -1 ;i++) outHello[i] ='a';
36    outHello[len -1] ='\0';
37}
38
39 void __stdcall GetStringParamW(TCHAR * outHello,int len)
40{ //output "aaaaaaaa" unicode version.
41 for(int i=0; i< len -1 ;i++) outHello[i] = TEXT('a');
42    outHello[len -1] = TEXT('\0');
43}
 
 
 
下面看C#如何调用。
 
第一：字符串指针作为输入参数，可以使用byte[] 和MarshalAs来解决。（注意四个P-INVOKE，两个ANSI版本,和两个UNICODE版本），推荐使用MarshalAs方法简单明了。
 
 1        [DllImport("TestDll", EntryPoint ="PrintString")]
 2publicstaticexternvoid PrintStringByBytes(byte[] hello);
 3
 4        [DllImport("TestDll", EntryPoint ="PrintString")]
 5publicstaticexternvoid PrintStringByMarshal([MarshalAs(UnmanagedType.LPStr)]string hello);
 6
 7        [DllImport("TestDll", EntryPoint ="PrintStringW")]
 8publicstaticexternvoid PrintStringByBytesW(byte[] hello);
 9
10        [DllImport("TestDll", EntryPoint ="PrintStringW")]
11publicstaticexternvoid PrintStringByMarshalW([MarshalAs(UnmanagedType.LPWStr)]string hello);
12
13
14publicvoid Run()
15        {
16            PrintStringByBytes(Encoding.ASCII.GetBytes("use byte[]"));
17            PrintStringByMarshal("use MarshalAs");
18            PrintStringByBytesW(Encoding.Unicode.GetBytes("use byte[]"));
19            PrintStringByMarshalW("use MarshalAs");
20}
 
 
 
第二：字符串指针作为返回值，和上面一样也有两种声明方法，同样也包含两个版本。注意：Marshal.PtrToStringAnsi()函数的使用，把字符串指针转变为C#的string.推荐使用MarshalAs方法简单明了。
 
 1        [DllImport("TestDll", EntryPoint ="GetStringReturn")]
 2publicstaticextern IntPtr GetStringReturnByBytes();
 3
 4        [DllImport("TestDll", EntryPoint ="GetStringReturn")]
 5        [return:MarshalAs(UnmanagedType.LPStr)]
 6publicstaticexternstring GetStringReturnByMarshal();
 7
 8        [DllImport("TestDll", EntryPoint ="GetStringReturnW")]
 9publicstaticextern IntPtr GetStringReturnByBytesW();
10
11        [DllImport("TestDll", EntryPoint ="GetStringReturnW")]
12        [return: MarshalAs(UnmanagedType.LPWStr)]
13publicstaticexternstring GetStringReturnByMarshalW();
14
15
16publicvoid Run()
17        {          //Marshal.PtrToStringAuto(GetStringReturnByBytes()); 自动判断类型不错。
18             Console.WriteLine(Marshal.PtrToStringAnsi(GetStringReturnByBytes()));
19            Console.WriteLine(GetStringReturnByMarshal());
20            Console.WriteLine(Marshal.PtrToStringUni(GetStringReturnByBytesW()));
21            Console.WriteLine(GetStringReturnByMarshalW());
22}
 
 
 
第三：字符串指针作为输入输出参数时，因为要求有固定的容量，所以这里使用的是StringBuilder，大家仔细看了，当然也有byte[]版本。这个看大家喜欢那个版本就是用那个.
 
 1        [DllImport("TestDll", EntryPoint ="GetStringParam")]
 2publicstaticexternvoid GetStringParamByBytes(byte[] outHello, int len);
 3
 4        [DllImport("TestDll", EntryPoint ="GetStringParam")]
 5publicstaticexternvoid GetStringParamByMarshal([Out, MarshalAs(UnmanagedType.LPStr)]StringBuilder outHello, int len);
 6
 7        [DllImport("TestDll", EntryPoint ="GetStringParamW")]
 8publicstaticexternvoid GetStringParamByBytesW(byte[] outHello, int len);
 9
10        [DllImport("TestDll", EntryPoint ="GetStringParamW")]
11publicstaticexternvoid GetStringParamByMarshalW([Out, MarshalAs(UnmanagedType.LPWStr)]StringBuilder outHello, int len);
12
13
14publicbyte[] _outHello =newbyte[10];
15publicbyte[] _outHelloW =newbyte[20];
16public StringBuilder _builder =new StringBuilder(10); //很重要设定string的容量。
17 
18publicvoid Run()
19        {
20//
21             GetStringParamByBytes(_outHello, _outHello.Length);
22            GetStringParamByMarshal(_builder, _builder.Capacity);
23            GetStringParamByBytesW(_outHelloW, _outHelloW.Length /2);
24            GetStringParamByMarshalW(_builder, _builder.Capacity);
25
26//
27             Console.WriteLine(Encoding.ASCII.GetString(_outHello));
28            Console.WriteLine(_builder.ToString());
29            Console.WriteLine(Encoding.Unicode.GetString(_outHelloW));
30            Console.WriteLine(_builder.ToString());
31}*/
}
