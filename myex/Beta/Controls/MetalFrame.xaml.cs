using System;
using System.Collections.Generic;
using System.Linq;
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
    /// MetalFrame.xaml 的交互逻辑
    /// </summary>
    public partial class MetalFrame : UserControl
    {
        public MetalFrame()
        {
            InitializeComponent();
            for (var i = 0; i < C.Count(); i++)
            {
                var item1 = (Image)content.Children[i];
                var item2 = C[i];
                item1.Source = item2;
                item1.Width = item2.PixelWidth;
                item1.Height = item2.PixelHeight;
                //保存原始坐标
                item1.Tag = new Int32Rect((int)Canvas.GetLeft(item1), (int)(Canvas.GetTop(item1)), (int)item1.Width, (int)item1.Height);
            }
        }
        static BitmapSource img = BitmapFrame.Create(new Uri(@"D:\Demo\myex\Beta\Resources\Window.png", UriKind.Absolute));
        BitmapSource[] C = new BitmapSource[] 
        {
            //脚
            //左上c0,右上c1
            new CroppedBitmap(img,new Int32Rect(64, 0, 16, 16)),
            new CroppedBitmap(img,new Int32Rect(110, 0, 18, 16)),
            //左下c2,右下c3
            new CroppedBitmap(img,new Int32Rect(64, 48, 16, 16)),
            new CroppedBitmap(img,new Int32Rect(110, 46, 18, 18)),
            //边
            //上c4下c5
            new CroppedBitmap(img,new Int32Rect(81, 0, 30, 12)),
            new CroppedBitmap(img,new Int32Rect(81, 52, 30, 12)),
            //左c6右c7
            new CroppedBitmap(img,new Int32Rect(64, 16, 12, 32)),
            new CroppedBitmap(img,new Int32Rect(116, 17, 12, 30)),
        };

        public void SetHeight(int value) 
        {
            if (value <= 64) return;
            //左下c2,右下c3，下c5坐标
            //左c6右c7高度
            var diff = value - 64;
            Canvas.SetTop(c2, ((Int32Rect)c2.Tag).Y + diff);
            Canvas.SetTop(c3, ((Int32Rect)c3.Tag).Y + diff);
            Canvas.SetTop(c5, ((Int32Rect)c5.Tag).Y + diff);

            c6.Height =((Int32Rect)c6.Tag).Height + diff;
            c7.Height = ((Int32Rect)c7.Tag).Height + diff;
            Height = value;
        }
        public void SetWidth(int value)
        {
            if (value <= 64) return;
            //右上c1右下c3右c7
            //上c4下c5
            var diff = value - 64;

            Canvas.SetLeft(c1, ((Int32Rect)c1.Tag).X + diff);
            Canvas.SetLeft(c3, ((Int32Rect)c3.Tag).X + diff);
            Canvas.SetLeft(c7, ((Int32Rect)c7.Tag).X + diff);

            c4.Width = ((Int32Rect)c4.Tag).Width + diff;
            c5.Width = ((Int32Rect)c5.Tag).Width + diff;
            Width = value;
            
        }

        public int BorderWidth { set { SetWidth(value); } }

        /// <summary>
        /// 坐标
        /// </summary>
        public static readonly DependencyProperty BorderWidthProperty = DependencyProperty.Register(
            "BorderWidth",
            typeof(int),
            typeof(MetalFrame),
            new PropertyMetadata(ChangeBorderWidth)
        );

        private static void ChangeBorderWidth(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var obj = (MetalFrame)d;
            var value = (int)e.NewValue;
            obj.BorderWidth = value;  
        }

        public int BorderHeight { set { SetHeight(value); } }

        /// <summary>
        /// 坐标
        /// </summary>
        public static readonly DependencyProperty BorderHeightProperty = DependencyProperty.Register(
            "BorderHeight",
            typeof(int),
            typeof(MetalFrame),
            new PropertyMetadata(ChangeBorderHeight)
        );

        private static void ChangeBorderHeight(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var obj = (MetalFrame)d;
            var value = (int)e.NewValue;
            obj.BorderHeight = value;
        }
    }
}
