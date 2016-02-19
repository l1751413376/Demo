using System;
using System.ComponentModel;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.Win32;
using System.IO;
using System.Text;

namespace CsTest
{
    static class Program
    {
        static Regex PicReg = new Regex(@"_\d+x\d+_", RegexOptions.ECMAScript);
        static void Main(string[] args)
        {
            string filename = @"D:\Demo\myex\Beta\img\2x1red.png";
            string desfilename = filename + "0xfile";
            FileStream fs = new FileStream(filename, FileMode.OpenOrCreate);
            byte[] buff = new byte[fs.Length];
            fs.Read(buff,0,(int)fs.Length);
            fs.Close();
            StringBuilder sb = new StringBuilder();
            foreach (var item in buff)
            {
                sb.AppendFormat("0x{0,2:X2},",item);
            }
            var outbuff = new UTF8Encoding().GetBytes(sb.ToString());
            if (File.Exists(desfilename))
            {
                File.Delete(desfilename);
            }
            fs = new FileStream(desfilename, FileMode.OpenOrCreate);
            fs.Write(outbuff, 0, outbuff.Length);
            fs.Close();
        }
    }

    

}