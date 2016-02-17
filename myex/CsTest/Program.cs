using System;
using System.ComponentModel;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.Win32;

namespace CsTest
{
    static class Program
    {
        static Regex PicReg = new Regex(@"_\d+x\d+_", RegexOptions.ECMAScript);
        static void Main(string[] args)
        {
            //CRC32=X32+X26+X23+X22+X16+X12+X11+X10+X8+X7+X5+X4+X2+X1+1
            //10000100110000010001110110110111
            
            //uint CRC32 = 2227248567;
            //uint CRC
        }
    }

    public class A
    {
        public int B;
        public A(int a)
        {
            B = a;
        }
    }

}