using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Beta.Controls
{
    /// <summary>
    /// QXMap.xaml 的交互逻辑
    /// </summary>
    public partial class QXMap :UserControl
    {
        /// <summary>
        /// 窗口对于地图的偏移量x 
        /// </summary>
        public int CenterX { get { return GV.WindowOffsetX; } set { GV.WindowOffsetX = value; } }
        /// <summary>
        /// 窗口对于地图的偏移量Y
        /// </summary>
        public int CenterY { get { return GV.WindowOffsetY; } set { GV.WindowOffsetY = value; } }
        /// <summary>
        /// 物体在地图上面的坐标x
        /// </summary>
        public double X { get; set; }
        /// <summary>
        /// 物体在地图上面的坐标y
        /// </summary>
        public double Y { get; set; }

        // 地图宽
        public double Width_
        {
            get 
            {
                return ImageContent.Width;
            }
            set
            {
                ImageContent.Width = value;
            }
        }

        // 地图高

        public double Height_
        {
            get
            {
                return ImageContent.Height;
            }
            set
            {
                ImageContent.Height = value;
            }
        }

        // 地图图片源

        public ImageSource Source
        {
            set
            {
                ImageContent.Source = value;
            }
        }

        // 地图透明度

        public double Opacity_ { get; set; }

        public QXMap()
        {
            InitializeComponent();
        }

        public void ReadMap()
        {
            /*
            IntPtr PngBuff = IntPtr.Zero;
            int len = 0;
            CppAPI.GetBitMap(@"D:\Demo\myex\Beta\Resources\1207.map.tga", out PngBuff, out len);
            byte[] managed_data = new byte[len];
            Marshal.Copy(PngBuff, managed_data, 0, len);
            MemoryStream ms = new MemoryStream(managed_data);
            
            BitmapImage image = new BitmapImage();
            image.BeginInit();
            image.StreamSource = ms;
            image.EndInit();
             **/
            BitmapImage image = new BitmapImage(new Uri(@"D:\Demo\myex\Beta\Resources\1442304430-7.jpg"));
            
            Source = image;
            Height_ = image.Height;
            Width_ = image.Width;
        }




    }
}
