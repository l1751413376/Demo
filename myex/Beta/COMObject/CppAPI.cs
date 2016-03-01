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
        public extern static int GetBitMap(out IntPtr Pointer);//与dll中一致 
    }
}
