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
                return Content.Width;
            }
            set
            {
                Content.Width = value;
            }
        }

        // 地图高

        public double Height_
        {
            get
            {
                return Content.Height;
            }
            set
            {
                Content.Height = value;
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
        public XYMap map = new XYMap();
        public void ReadMap()
        {
            map.Load("D:\\BaiduYunDownload\\梦幻西游\\scene\\1002.map");
            Height_ = map.Height;
            Width_ = map.Width;
            Source = map.MapImg2;
            //图的实际宽高（因为原地图是320*240一块一块）
            ImageContent.Width = map.MapImg2.Width;
            ImageContent.Height = map.MapImg2.Height;
        }




    }
}
