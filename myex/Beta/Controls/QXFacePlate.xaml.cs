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
    /// QXFacePlate.xaml 的交互逻辑
    /// </summary>
    public partial class QXFacePlate : UserControl
    {
        static QXFacePlate() 
        {
            CutStripImg();
        }
        public static int[] HeartTextMarginLeft = new int[] 
        {
            0,-5,-10,-15
        };
        public static int MaxWidth = 130;
        //life
        LinearGradientBrush[] rainbowBrush = new LinearGradientBrush[]
        {
            //HP
            Super.CreateRainbowBrush(255, 6, 17, 228, 6, 17),
            //MP
            Super.CreateRainbowBrush(35, 73, 255, 35, 73, 182),
            //TP
            Super.CreateRainbowBrush(26, 255, 51, 26, 181, 51),
            
            Super.CreateRainbowBrush(202, 95, 235, 77, 40, 92),

        };
        public QXFacePlate()
        {
            InitializeComponent();

            HPBorder.MoveBitmap(BorderImg[0]);
            HPStrip.MoveBitmap(StripImg[0]);

            MPBorder.MoveBitmap(BorderImg[1]);
            MPStrip.MoveBitmap(StripImg[1]);

            HeartBorder.MoveBitmap(HeartImg[0]);
            HeartBorderMaskImg.MoveBitmap(HeartImg[0]);
            HeartStrip.MoveBitmap(HeartImg[1]);

            HPText.FontFamily = GV.DroidSansFallback;
            MPText.FontFamily = GV.DroidSansFallback;
            HeartText.FontFamily = GV.DroidSansFallback;
            HPLabel.FontFamily = GV.DroidSansFallback;
            MPLabel.FontFamily = GV.DroidSansFallback;
            HeartText.Tag = Canvas.GetLeft(HeartText);
        }
        static BitmapSource[] BorderStripImg = new BitmapSource[] 
        {
            BitmapFrame.Create(new Uri(@"D:\Demo\myex\Beta\Resources\Gauge-FullGreen.png", UriKind.Absolute)),
            BitmapFrame.Create(new Uri(@"D:\Demo\myex\Beta\Resources\Gauge-FullYellow.png", UriKind.Absolute)),
            BitmapFrame.Create(new Uri(@"D:\Demo\myex\Beta\Resources\Gauge-Heart.png", UriKind.Absolute)),
        };
        //框框
        static BitmapSource[] BorderImg;
        //条子
        static BitmapSource[] StripImg;
        public static void CutStripImg() 
        {
            //条子0-1
            BorderImg=new BitmapSource[2];
            StripImg=new BitmapSource[2];
            for (int i = 0; i < 2;i++ )
            {
                var img=BorderStripImg[i];
                BorderImg[i] =new CroppedBitmap(img, new Int32Rect(0, 0, 124, 24));
                StripImg[i] = new CroppedBitmap(img, new Int32Rect(1, 25, 122, 22));
            }
            //心形
            HeartImg[0] = new CroppedBitmap(BorderStripImg[2], new Int32Rect(0, 0, 48, 48));
            HeartImg[1] = new CroppedBitmap(BorderStripImg[2], new Int32Rect(1, 49, 46, 46));
        }

        static BitmapSource[] HeartImg = new BitmapSource[2]; 
       

        public void SetValue(int type, double[] value)
        {
            const int StripMaskMaxLenght = 118;
            const int StripMaskMinLenght = 2;
            const int StripMaskDiffLenght = StripMaskMaxLenght - StripMaskMinLenght;
            switch (type)
            {
                case 0:
                    HPStripMask.Width = StripMaskMaxLenght-(StripMaskDiffLenght - value[0] / value[1] * StripMaskDiffLenght);
                    HPText.Text = string.Format("{0:f0}/{1:f0}", value[0], value[1]);
                    break;
                case 1:
                    MPStripMask.Width = StripMaskMaxLenght - (StripMaskDiffLenght - value[0] / value[1] * StripMaskDiffLenght);
                    MPText.Text = string.Format("{0:f0}/{1:f0}", value[0], value[1]);
                    break;
                case 2:
                    var v=(34 - value[0] / value[1] * 34);
                    HeartText.Text= string.Format("{0:f0}", value[0]);
                    HeartBorderMask.Height = v+6;
                    Canvas.SetLeft(HeartText, (double)HeartText.Tag + HeartTextMarginLeft[HeartText.Text.Count()-1]);
                    //Canvas.SetLeft(HeartText, -100);
                    break;
            }

        }
    }
}
