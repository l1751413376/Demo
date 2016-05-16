using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using DWORD = System.UInt32;

namespace Beta.Controls
{
    public class XYMap
    {
        public Byte[] FileName = new byte[256];
        public DWORD Flag;
        public DWORD Width;
        public DWORD Height;
        public DWORD MaskFlag;
        public DWORD m_MapBmpWidth;
        public DWORD m_MapBmpHeight;
        /// <summary>
        /// mask数量
        /// </summary>
        public DWORD MaskNum;
        public IntPtr MaskInfos;
        public int MaskInfoSize;
        /// <summary>
        /// 图像
        /// </summary>
        public DWORD MapImgSize;
        public IntPtr MapImg;

        //追加属性
        public List<MaskInfo> MaskInfoList = new List<MaskInfo>();
        public BitmapImage MapImg2 = new BitmapImage();

        public void Load(string filename)
        {
            IntPtr mapInfo;
            int mapInfosize;
            CppAPI.GetXYMap(filename, out mapInfo, out mapInfosize);
            var mapstream = mapInfo.ToStream(mapInfosize);
            mapstream.Read(FileName, 0, FileName.Length);
            Flag = mapstream.ReadDword();
            Width = mapstream.ReadDword();
            Height = mapstream.ReadDword();
            MaskFlag = mapstream.ReadDword();
            m_MapBmpWidth = mapstream.ReadDword();
            m_MapBmpHeight = mapstream.ReadDword();
            MaskNum = mapstream.ReadDword();
            MaskInfos = new IntPtr(mapstream.ReadDword());
            MaskInfoSize = (int)mapstream.ReadDword();
            MapImgSize = mapstream.ReadDword();
            MapImg = new IntPtr(mapstream.ReadDword());
            //地图大图操作
            MapImg2.FromIntPtr(MapImg, (int)MapImgSize);


            //mask操作
            var maskstream = MaskInfos.ToStream((int)(MaskInfoSize* MaskNum));
            for (var i = 0; i < MaskNum; i++)
            {
                var item = new MaskInfo();
                item.startX=(int) maskstream.ReadDword();
                item.startY = (int)maskstream.ReadDword();
                item.width = (int)maskstream.ReadDword();
                item.height = (int)maskstream.ReadDword();
                item.mask_size = (int)maskstream.ReadDword();
                item.img = new IntPtr(maskstream.ReadDword());
                item.imgsize = (int)maskstream.ReadDword();
                item.img2.FromIntPtr(item.img, item.imgsize);

                MaskInfoList.Add(item);
            }
        }
    }

    public class MaskInfo
    {
        public int startX;
        public int startY;
        public int width;
        public int height;
        public int mask_size;
        public IntPtr img;
        public int imgsize;
        //追加
        public BitmapImage img2 = new BitmapImage();
    }
}
