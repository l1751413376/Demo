using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace Beta.PictureProcess
{
    //
    public class WasFile
    {
        /// <summary>
        /// 帧数量
        /// </summary>
        public int FrameCount;
        /// <summary>
        /// 方向计数
        /// </summary>
        public int DirectionCount;
        /// <summary>
        /// 整个Action宽度
        /// </summary>
        public int Width;
        /// <summary>
        /// 整个Action高度
        /// </summary>
        public int Height;
        /// <summary>
        /// 中心X
        /// </summary>
        public int CentryX;
        /// <summary>
        /// 中心Y
        /// </summary>
        public int CentryY;
        /// <summary>
        /// 帧集合
        /// </summary>
        public BitmapImage[,] Frames;
        /// <summary>
        /// 文件路径
        /// </summary>
        public string filePath;

        public void LoadFile(String filePath)
        {
            IntPtr wasptr;
            CppAPI.GetWasFileInfo(filePath, out wasptr, out DirectionCount, out FrameCount, out Width, out Height, out CentryX, out CentryY);
            IntPtr dataptr;
            int datalen = 0;
            Frames = new BitmapImage[DirectionCount, FrameCount];
            for (int x = 0; x < DirectionCount;x++)
            {
                for (int y = 0; y < FrameCount; y++)
                {
                    CppAPI.GetWasFrame(Width, Height, wasptr, x, y, out dataptr, out datalen);
                    var item = new BitmapImage();
                    item.FromIntPtr(dataptr, datalen);
                    Frames[x, y] = item;
                    CppAPI.Delete(dataptr);
                }
            }
            CppAPI.Delete(wasptr);
        }

        /// <summary>
        /// 1-上，2-右上...8-左上
        /// </summary>
        public BitmapImage GetImg(int directon,int count) 
        {
            if (count < FrameCount && directon < DirectionCount)
            {
                return Frames[directon, count];
            }
            else 
            {
                return null;
            }

        }

    }
}
