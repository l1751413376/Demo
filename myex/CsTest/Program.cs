using System;
using System.ComponentModel;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.Win32;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace CsTest
{
    static class Program
    {
        static void Main(string[] args)
        {
            var file = File.Open(@"D:\MyConfiguration\ly09614.TCENT\Desktop\Unity3D5.0\破解补丁\1222.map", FileMode.Open);
            MapInfo map = new MapInfo();
            map.Flag = file.ReadDword();

        }


    }
    public static class FileEx
    {
        public static uint ReadDword(this FileStream stream)
        {
            byte[] b = new byte[4];
            stream.Read(b, 0, 4);
            return BitConverter.ToUInt32(b, 0);
        }
    }

    public class MapInfo
    {
        public int Flag;
        public uint Width;
        public uint Height;
        public uint MaskFlag;
        /// <summary>
        /// mask数量
        /// </summary>
        public int MaskNum;
        public List<MaskInfo> MaskInfos;
        /// <summary>
        /// 图像
        /// </summary>
        public byte[] MapImg;

    }

    public class MaskInfo
    {
        public int startX;
        public int startY;
        public int width;
        public int height;
        public int mask_size;
        public byte[] img;
    }
}