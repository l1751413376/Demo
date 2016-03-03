using System;
using System.Collections.Generic;
using System.ComponentModel;
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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Beta
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window 
    {
       
        Rectangle rect;//创建一个方块作为演示对象
        double speed = 1; //设置移动速度

        Point moveTo; //设置移动目标

        public MainWindow()
        {
            InitializeComponent();
            rect = new Rectangle();
            rect.Fill = new SolidColorBrush(Colors.Red);
            rect.Width = 50;
            rect.Height = 50;
            rect.RadiusX = 5;
            rect.RadiusY = 5;
            Canvas.SetLeft(rect,0);
            Canvas.SetTop(rect, 0);
            Carrier.Children.Add(rect);

            DispatcherTimer dispatcherTimer = new DispatcherTimer(DispatcherPriority.Normal);

            dispatcherTimer.Tick += new EventHandler(Timer_Tick);

            dispatcherTimer.Interval = TimeSpan.FromMilliseconds(50); //重复间隔

            dispatcherTimer.Start();

        }
        private void Timer_Tick(object sender, EventArgs e)
        {

            double rect_X = Canvas.GetLeft(rect);

            double rect_Y = Canvas.GetTop(rect);

            Canvas.SetLeft(rect, rect_X + (rect_X < moveTo.X ? speed : -speed));

            Canvas.SetTop(rect, rect_Y + (rect_Y < moveTo.Y ? speed : -speed));

        }

        private void Carrier_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            moveTo = e.GetPosition(Carrier);
        }

        /// <summary>
        /// 裁剪图片
        /// </summary>
        private BitmapSource cutImage(string imgaddress, int x, int y, int width, int height)
        {
            return new CroppedBitmap(
                BitmapFrame.Create(new Uri(imgaddress, UriKind.Relative)),
                new Int32Rect(x, y, width, height)
                 );
        }
            /*
            //图片操作
            IntPtr PngBuff = IntPtr.Zero;
               var len=CppAPI.GetBitMap2(out PngBuff,1);
               byte[] managed_data = new byte[len];
               Marshal.Copy(PngBuff, managed_data, 0, len);
               MemoryStream ms = new MemoryStream(managed_data);

               BitmapImage image = new BitmapImage();
               image.BeginInit();
               image.StreamSource = ms;
               image.EndInit();
               bgMap.Source = image;*/
            /*Canvan 位移动画（特效:从A点到B点 如果在移动过程中出现C点 则会取消到B点的移动）
            1.DoubleAnimation 创建线性缓冲动画
            2.Storyboard 创建故事版
            3.关联物体,动画，故事版 Storyboard.SetTarget(doubleAnimation, rect);
              Storyboard.SetTargetProperty(doubleAnimation, new PropertyPath("(Canvas.Left)"));
              storyboard.Children.Add(doubleAnimation);
            4.storyboard.Begin();开始动画
            注意Canvas.SetLeft(rect,0); 
                Canvas.SetTop(rect, 0);、不然Canvas.GetLeft(rect)将会失败
            */
            /*CompositionTarget.Rendering += new EventHandler(Timer_Tick);基于屏幕刷新频率的*/

        }
}
