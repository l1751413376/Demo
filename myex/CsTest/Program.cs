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
            map.Flag = (int)file.ReadDword();
            map.Width = file.ReadDword();
            map.Height = file.ReadDword();
            int m_MapBlockRowNum = (int)Math.Ceiling(map.Height / (double)240);
            int m_MapBlockColNum = (int)Math.Ceiling(map.Width / (double)320);

            //----------------------------------
            // 读偏移信息
            //----------------------------------
            int block_num = m_MapBlockRowNum * m_MapBlockColNum;
            uint[] pBlockOffset = new uint[block_num];
            for (var i = 0; i < pBlockOffset.Length; i++)
            {
                pBlockOffset[i] = file.ReadDword();
            }

            //块
            SBlockInfo[] blocks = new SBlockInfo[block_num];

            for (int i = 0; i < block_num; i++)
            {
                var item = blocks[i]=new SBlockInfo();
                file.Seek(pBlockOffset[i], SeekOrigin.Begin);
                uint usize = file.ReadDword();
                file.Seek(usize, SeekOrigin.Current);
                var flag = file.ReadDword();

                //var flag2 = file.ReadDword();
            }


            //mask
            map.MaskFlag = file.ReadDword();
            map.MaskNum = (int)file.ReadDword();

            uint[] pMaskDataOffset = new uint[map.MaskNum];

            for (var i = 0; i < map.MaskNum; i++)
            {
                pMaskDataOffset[i] = file.ReadDword();
            }
            map.MaskInfos = new MaskInfo[map.MaskNum];
            for (var i = 0; i < map.MaskNum; i++)
            {
                file.Seek(pMaskDataOffset[i], SeekOrigin.Begin);
                var item = map.MaskInfos[i] = new MaskInfo();
                item.startX = (int)file.ReadDword();
                item.startY = (int)file.ReadDword();
                item.width = (int)file.ReadDword();
                item.height = (int)file.ReadDword();
                item.mask_size = (int)file.ReadDword();

                byte[] pMaskDataOrgin = new byte[item.mask_size];
                file.Read(pMaskDataOrgin, 0, item.mask_size);
                //解压mask
                int align_width = (item.width / 4 + ((item.width % 4 != 0) ? 1 : 0)) * 4;	// 以4对齐的宽度
                byte[] pMaskDataDec = new byte[align_width * item.height / 4];


                
            }

            
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
        public MaskInfo[] MaskInfos;
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

    public class SBlockInfo
    {
        public uint UnitN;	// 未知数据大小
        public uint flag;
        public uint size;
    };
}