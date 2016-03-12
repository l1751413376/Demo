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


namespace Beta
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window 
    {
        Rectangle rect;

        private IPathFinder PathFinder = null;

        private byte[,] Matrix = new byte[1024, 1024]; //寻路用二维矩阵

        private int GridSize = 20; //单位格子大小

        private Point Start; //移动起点坐标

        private Point End; //移动终点坐标

        Ellipse player = new Ellipse(); //用一个圆来模拟目标对象

        QXSpirit Spirit = new QXSpirit();

        private void InitSpirit()
        {

            Spirit.X = 300; //为精灵关联属性X赋值

            Spirit.Y = 400; //为精灵关联属性Y赋值

            Spirit.Timer.Interval = TimeSpan.FromMilliseconds(150); //精灵图片切换频率

            Spirit.ImageAddress = @"..\Player\"; //精灵图片源地址

            Carrier.Children.Add(Spirit);

        }

        public MainWindow()
        {
            InitializeComponent();
            ResetMatrix();
            InitPlayer();


        }
        private void InitPlayer()
        {

            player.Fill = new SolidColorBrush(Colors.Blue);

            player.Width = GridSize;

            player.Height = GridSize;

            Carrier.Children.Add(player);

            //开始位置(1,1)

            Canvas.SetLeft(player, GridSize);

            Canvas.SetTop(player, 5 * GridSize);

        }
        private void ResetMatrix()
        {

            for (int y = 0; y < Matrix.GetUpperBound(1); y++)
            {

                for (int x = 0; x < Matrix.GetUpperBound(0); x++)
                {

                    //默认值可以通过(非障碍物)在矩阵中用1表示

                    Matrix[x, y] = 1;

                }

            }

            //构建障碍物(举例)

            for (int i = 0; i < 18; i++)
            {

                //障碍物在矩阵中用0表示

                Matrix[i, 12] = 0;

                rect = new Rectangle();

                rect.Fill = new SolidColorBrush(Colors.Red);

                rect.Width = GridSize;

                rect.Height = GridSize;

                Carrier.Children.Add(rect);

                Canvas.SetLeft(rect, i * GridSize);

                Canvas.SetTop(rect, 12 * GridSize);

            }

            //构建其他障碍物……(省略)

            Start = new Point(1, 1); //设置起点坐标

        }

        private void Carrier_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point p = e.GetPosition(Carrier);

            //进行坐标系缩小

            int start_x = (int)Canvas.GetLeft(player) / GridSize;

            int start_y = (int)Canvas.GetTop(player) / GridSize;

            Start = new Point(start_x, start_y); //设置起点坐标

            int end_x = (int)p.X / GridSize;

            int end_y = (int)p.Y / GridSize;

            End = new Point(end_x, end_y); //设置终点坐标

            PathFinder = new PathFinderFast(Matrix);

            PathFinder.Formula = HeuristicFormula.Manhattan; //使用我个人觉得最快的曼哈顿A*算法

            PathFinder.HeavyDiagonals = true; //使用对角线移动

            PathFinder.HeuristicEstimate = 0;

            List<PathFinderNode> path = PathFinder.FindPath(new QXGame_Silverlight3.AStar.Point(Start.X,Start.Y) , new QXGame_Silverlight3.AStar.Point(End.X, End.Y)); //开始寻径

            if (path == null)
            {

                MessageBox.Show("路径不存在！");

            }
            else {

                Point[] framePosition = new Point[path.Count]; //定义关键帧坐标集

                for (int i = path.Count - 1; i >= 0; i--)
                {

                    //从起点开始以GridSize为单位，顺序填充关键帧坐标集，并进行坐标系放大

                    framePosition[path.Count - 1 - i] = new Point(path[i].X * GridSize, path[i].Y * GridSize);

                }

                //创建故事板

                Storyboard storyboard = new Storyboard();

                int cost = 100; //每移动一个小方格(20*20)花费100毫秒

                //创建X轴方向逐帧动画

                DoubleAnimationUsingKeyFrames keyFramesAnimationX = new DoubleAnimationUsingKeyFrames();

                //总共花费时间 = path.Count * cost

                keyFramesAnimationX.Duration = new Duration(TimeSpan.FromMilliseconds(path.Count * cost));

                Storyboard.SetTarget(keyFramesAnimationX, player);

                Storyboard.SetTargetProperty(keyFramesAnimationX, new PropertyPath("(Canvas.Left)"));

                //创建Y轴方向逐帧动画

                DoubleAnimationUsingKeyFrames keyFramesAnimationY = new DoubleAnimationUsingKeyFrames();

                keyFramesAnimationY.Duration = new Duration(TimeSpan.FromMilliseconds(path.Count * cost));

                Storyboard.SetTarget(keyFramesAnimationY, player);

                Storyboard.SetTargetProperty(keyFramesAnimationY, new PropertyPath("(Canvas.Top)"));

                for (int i = 0; i < framePosition.Count(); i++)
                {

                    //加入X轴方向的匀速关键帧

                    LinearDoubleKeyFrame keyFrame = new LinearDoubleKeyFrame();

                    keyFrame.Value = i == 0 ? Canvas.GetLeft(player) : framePosition[i].X; //平滑衔接动画

                    keyFrame.KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromMilliseconds(cost * i));

                    keyFramesAnimationX.KeyFrames.Add(keyFrame);

                    //加入Y轴方向的匀速关键帧

                    keyFrame = new LinearDoubleKeyFrame();

                    keyFrame.Value = i == 0 ? Canvas.GetTop(player) : framePosition[i].Y;

                    keyFrame.KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromMilliseconds(cost * i));

                    keyFramesAnimationY.KeyFrames.Add(keyFrame);

                }

                storyboard.Children.Add(keyFramesAnimationX);

                storyboard.Children.Add(keyFramesAnimationY);

                //添加进资源

                if (!Resources.Contains("storyboard"))
                {

                    Resources.Add("storyboard", storyboard);

                }

                //故事板动画开始

                storyboard.Begin();

                //用白色点记录移动轨迹

                for (int i = path.Count - 1; i >= 0; i--)
                {

                    rect = new Rectangle();

                    rect.Fill = new SolidColorBrush(Colors.Snow);

                    rect.Width = 5;

                    rect.Height = 5;

                    Carrier.Children.Add(rect);

                    Canvas.SetLeft(rect, path[i].X * GridSize);

                    Canvas.SetTop(rect, path[i].Y * GridSize);

                }

            }
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
