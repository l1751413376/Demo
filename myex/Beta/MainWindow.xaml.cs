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
using QXGame_Silverlight3.AStar;
using Point = System.Windows.Point;
using Beta.Controls;
using Beta.PictureProcess;


namespace Beta
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private IPathFinder PathFinder = null;
        private byte[,] Matrix = new byte[1024, 1024]; //寻路用二维矩阵
        private int GridSize = 10; //单位格子大小
        private Point Start; //移动起点坐标
        private Point End; //移动终点坐标


        QXSprite Spirit = new QXSprite();
        WasFile wasFile1 = new WasFile();
        WasFile wasFile2 = new WasFile();

        QXMap map = new QXMap();

        private void InitSpirit()
        {

            //Spirit.X = 300; //为精灵关联属性X
            //Spirit.Y = 400; //为精灵关联属性Y赋值
            //var filename = "D:/Demo/myex/Alpha/project/bin/EB5088EA.was";
            //wasFile1.LoadFileFromPath(filename);
            //wasFile2.LoadFileFromPath("D:/Demo/myex/Alpha/project/bin/0AF85B1A.was");
            //Spirit.wasFile = wasFile2;
            //Spirit.limitcount = wasFile2.FrameCount - 1;
            //Spirit.Timer.Interval = TimeSpan.FromMilliseconds(100); //精灵图片切换频率
            //Carrier.Children.Add(Spirit);


            map.ReadMap();
            map.Width = 800;
            map.Height = 600;
            Canvas.SetZIndex(map,-1);
            Carrier.Children.Add(map);

            Carrier.Children.Add(Spirit);
        }

        public MainWindow()
        {
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.Manual;
            Left = 0;//屏幕宽度
            Top = SystemParameters.PrimaryScreenHeight-100;//屏幕高度;;
            InitSpirit();
        }
       
        
        
        /// <summary>
        /// 直线移动
        /// </summary>
        private void LinearMove(QXSprite sprite, Point p) 
        {
            GV.Storyboard.NewStoryboard(sprite.Name);
            var moveCost = Super.GetAnimationTimeConsuming(sprite.Coordinate, p, GridSize, sprite.VRunSpeed);
            //创建坐标变换属性动画
            PointAnimation pointAnimation = new PointAnimation()
            {
                To = p,
                Duration = new Duration(TimeSpan.FromMilliseconds(moveCost))
            };
            Storyboard.SetTarget(pointAnimation, sprite);
            Storyboard.SetTargetProperty(pointAnimation, new PropertyPath("Coordinate"));
            sprite.Action = Actions.Run;
            //启动属性动画
            GV.Storyboard[sprite.Name].Children.Add(pointAnimation);
            GV.Storyboard[sprite.Name].Begin();
        }

        private void Carrier_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point p = e.GetPosition(Carrier);
            LinearMove(Spirit,p);

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

        //定时器动画
        /* DispatcherTimer dispatcherTimer = new DispatcherTimer(DispatcherPriority.Normal);

            dispatcherTimer.Tick += new EventHandler(Timer_Tick);

            dispatcherTimer.Interval = TimeSpan.FromMilliseconds(50); //重复间隔

            dispatcherTimer.Start();*/

    }
}
