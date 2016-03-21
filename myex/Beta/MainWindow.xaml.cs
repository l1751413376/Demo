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
        private int GridSize = 10; //单位格子大小
        /// <summary>
        /// 障碍物
        /// </summary>
        byte[,] FixedObstruction;
        int WidthWindow = 800;
        int HeightWindow = 600;
        QXSprite Spirit = new QXSprite();
        WasFile wasFile1 = new WasFile();
        WasFile wasFile2 = new WasFile();

        QXMap map = new QXMap();

        private void InitSpirit()
        {

            //Spirit.X = 100; //为精灵关联属性X
            //Spirit.Y = 100; //为精灵关联属性Y赋值
            //var filename = "D:/Demo/myex/Alpha/project/bin/EB5088EA.was";
            //wasFile1.LoadFileFromPath(filename);
            //wasFile2.LoadFileFromPath("D:/Demo/myex/Alpha/project/bin/0AF85B1A.was");
            //Spirit.wasFile = wasFile2;
            //Spirit.limitcount = wasFile2.FrameCount - 1;
            //Spirit.Timer.Interval = TimeSpan.FromMilliseconds(100); //精灵图片切换频率
            //Carrier.Children.Add(Spirit);
            Spirit.Coordinate = new Point(100,100);
            Spirit.CoordinateChanged += MapMove;
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
            //转换坐标为地图坐标


            GV.Storyboard.NewStoryboard(sprite.Name);
            var moveCost = Super.GetAnimationTimeConsuming(sprite.Coordinate, p.ToMapCoordinate(), GridSize, sprite.VRunSpeed);

            //创建坐标变换属性动画
            PointAnimation pointAnimation = new PointAnimation()
            {
                To = p.ToMapCoordinate(),
                Duration = new Duration(TimeSpan.FromMilliseconds(moveCost)),
            };
            Storyboard.SetTarget(pointAnimation, sprite);
            Storyboard.SetTargetProperty(pointAnimation, new PropertyPath("Coordinate"));
            sprite.Action = Actions.Run;

            //改变朝向
            double newDirection = Super.GetDirectionByTan(p, sprite.Coordinate.ToWindowCoordinate());
            DoubleAnimation doubleAnimation = new DoubleAnimation()
            {
                To = newDirection,
                Duration = new Duration(TimeSpan.Zero)
            };
            Storyboard.SetTarget(doubleAnimation, sprite);
            Storyboard.SetTargetProperty(doubleAnimation, new PropertyPath("Direction"));
            GV.Storyboard[sprite.Name].Children.Add(doubleAnimation);
            //启动属性动画
            GV.Storyboard[sprite.Name].Children.Add(pointAnimation);
            GV.Storyboard[sprite.Name].Begin();
        }
        /// <summary>
        /// A*寻路移动
        /// </summary>
        private void AStarMove(QXSprite sprite, Point p) 
        {
            //进行单元格缩小
            Point start = new Point()
            {
                X = sprite.Coordinate.X / GridSize,
                Y = sprite.Coordinate.Y / GridSize
            };
            Point end = new Point()
            {
                X = p.X / GridSize,
                Y = p.Y / GridSize
            };
            //相同点 return
            if (start == end)
                return;

            PathFinderFast pathFinderFast = new PathFinderFast(FixedObstruction)
            {
                HeavyDiagonals = false,
                HeuristicEstimate = 2,
                SearchLimit = 2000
            };
            List<PathFinderNode> path = pathFinderFast.FindPath(new QXGame_Silverlight3.AStar.Point(start.X, start.Y), new QXGame_Silverlight3.AStar.Point(end.X, end.Y));

            if (path == null || path.Count <= 1)
            {
                //路径不存在（有可能精灵在移动中）
                sprite.Action = Actions.Stop;
                GV.Storyboard.RemoveStoryboard(sprite.Name);
                return;
            }

            GV.Storyboard.NewStoryboard(sprite.Name);
            GV.Storyboard[sprite.Name].Completed += new EventHandler(Move_Completed);
            //创建朝向动画
            DoubleAnimationUsingKeyFrames doubleAnimationUsingKeyFrames = new DoubleAnimationUsingKeyFrames()
            {
                Duration = new Duration(TimeSpan.FromMilliseconds(path.Count * sprite.VRunSpeed)) //总共花费时间
            };
            Storyboard.SetTarget(doubleAnimationUsingKeyFrames, sprite);
            Storyboard.SetTargetProperty(doubleAnimationUsingKeyFrames, new PropertyPath("Direction"));
            //创建坐标变换逐帧动画
            PointAnimationUsingKeyFrames pointAnimationUsingKeyFrames = new PointAnimationUsingKeyFrames()
            {
                Duration = new Duration(TimeSpan.FromMilliseconds(path.Count * sprite.VRunSpeed))
            };
            Storyboard.SetTarget(pointAnimationUsingKeyFrames, sprite);
            Storyboard.SetTargetProperty(pointAnimationUsingKeyFrames, new PropertyPath("Coordinate"));
            
            for (int iMax = path.Count - 1, i = iMax; i >= 0; i--)
            {
                //加入坐标变换的匀速关键帧
                var linearPointKeyFrame = new LinearPointKeyFrame()
                {
                    KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromMilliseconds(sprite.VRunSpeed * (iMax - i)))
                };
                //平滑衔接动画(将寻路坐标系中的坐标放大回地图坐标系中的坐标)
                if (i == iMax)
                {
                    linearPointKeyFrame.Value = sprite.Coordinate;
                }
                else if (i == 0)
                {
                    linearPointKeyFrame.Value = sprite.Destination;
                }
                else
                {
                    linearPointKeyFrame.Value = new Point(path[i].X * GridSize + GridSize / 2, path[i].Y * GridSize + GridSize / 2); //+ GridSize / 2为偏移处理
                }
                pointAnimationUsingKeyFrames.KeyFrames.Add(linearPointKeyFrame);
                //加入朝向匀速关键帧
                var linearDoubleKeyFrame = new LinearDoubleKeyFrame()
                {
                    KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromMilliseconds(sprite.VRunSpeed * (iMax - i)))
                };
                linearDoubleKeyFrame.Value = i == 0
                    ? Super.GetDirectionByAspect((int)path[i].X, (int)path[i].Y, (int)path[i + 1].X, (int)path[i + 1].Y)
                    : Super.GetDirectionByAspect((int)path[i - 1].X, (int)path[i - 1].Y, (int)path[i].X, (int)path[i].Y)
                ;
                doubleAnimationUsingKeyFrames.KeyFrames.Add(linearDoubleKeyFrame);
            }
            GV.Storyboard[sprite.Name].Children.Add(pointAnimationUsingKeyFrames);
            GV.Storyboard[sprite.Name].Children.Add(doubleAnimationUsingKeyFrames);
            //启动属性动画
            GV.Storyboard[sprite.Name].Begin();
            sprite.Action = Actions.Run;

        }
        /// <summary>
        /// 移动结束
        /// </summary>
        private void Move_Completed(object sender, EventArgs e)
        {
        }
        /// <summary>
        /// 获取画布Name精灵控件
        /// </summary>
        private QXSprite FindSprite(string name)
        {
            return Carrier.FindName(name) as QXSprite;
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

        /// <summary>
        /// 地图牵引
        /// <param name="lc">主角Leader</param>
        /// </summary>
        private void MapMove(QXSprite leader)
        {
            var lc=leader.Coordinate;

            var marginLeft = lc.X - GV.CenterX;
            var marginRight =GV.CenterX + WidthWindow -lc.X;
            var marginTop = lc.Y - GV.CenterY;
            var marginBottom = GV.CenterY + HeightWindow - lc.Y;

            if (GV.CenterX>0 && marginLeft < GV.LeaderMargin)
            {
                var offset=GV.LeaderMargin-marginLeft;
                Canvas.SetLeft(map.ImageContent, -(GV.CenterX -= (int)offset));
            }
            if (GV.CenterX + WidthWindow < map.Width_ && marginRight <= GV.LeaderMargin)
            {
                var offset = GV.LeaderMargin - marginRight;
                Canvas.SetLeft(map.ImageContent, -(GV.CenterX += (int)offset));
            }
            if (GV.CenterY > 0  &&  marginTop <= GV.LeaderMargin)
            {
                var offset = GV.LeaderMargin - marginTop;
                Canvas.SetTop(map.ImageContent, -(GV.CenterY -= (int)offset));
            }
            if (GV.CenterY + Height < map.Height_ && marginBottom <= GV.LeaderMargin)
            {
                var offset = GV.LeaderMargin - marginBottom;
                Canvas.SetTop(map.ImageContent, -(GV.CenterY += (int)offset));
            }
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
