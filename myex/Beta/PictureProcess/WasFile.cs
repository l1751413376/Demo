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
    public class WasAction
    {
        /// <summary>
        /// 帧数量
        /// </summary>
        public int Count;
        /// <summary>
        /// 整个Action宽度
        /// </summary>
        public int Width;
        /// <summary>
        /// 整个Action高度
        /// </summary>
        public int Height;
        /// <summary>
        /// 帧集合
        /// </summary>
        public BitmapImage[][] Frames;

        

    }
}
