using Beta.PictureProcess;
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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Beta.Controls
{
    /// <summary>
    /// QXSpirit.xaml 的交互逻辑
    /// </summary>
    public partial class QXSprite : UserControl
    {
        #region 系统属性
        /// <summary>
        /// 图片位于控件中心的位置X
        /// </summary>
        public static int CenterX = 75;
        /// <summary>
        /// 图片位于控件中心的位置Y
        /// </summary>
        public static int CenterY = 120;

        /// <summary>
        /// 获取跑步速度ms(每移动一个单元格的花费时间,越小越快)
        /// </summary>
        public double VRunSpeed = 100;

        /// <summary>
        /// 位置(相对于地图坐标 0-当前,1-目的地)
        /// </summary>
        public Point[] Position = new Point[2];

        /// <summary>
        /// 定时器
        /// </summary>
        public DispatcherTimer Timer = new DispatcherTimer();
        /// <summary>
        /// Was文件
        /// </summary>
        public WasFile WasStantFile;
        /// <summary>
        /// Was文件
        /// </summary>
        public WasFile WasRunFile;
        /// <summary>
        /// Was文件
        /// </summary>
        public List<WasFile> WasFiles;

        /// <summary>
        /// 计数 0-当前,1最大值,2攻击时
        /// </summary>
        public int[] Count = new int[3];

        /// <summary>
        /// 获取或设置精灵坐标(关联属性)
        /// </summary>
        public Point Coordinate
        {
            get { return (Point)GetValue(CoordinateProperty); }
            set { SetValue(CoordinateProperty, value); }
        }

        public Point AttackCoordinate
        {
            get { return (Point)GetValue(AttackCoordinateProperty); }
            set { SetValue(AttackCoordinateProperty, value); }
        }
        /// <summary>
        /// 坐标变换事件
        /// </summary>
        public event CoordinateEventHandler CoordinateChanged;

        /// <summary>
        /// 动作
        /// </summary>
        public Actions Action;
        /// <summary>
        /// 
        /// </summary>
        public AttackActions AttackAction;
        /// <summary>
        /// 设置动作
        /// </summary>
        public void SetAction(Actions action)
        {
            Action = action;
            Count[0] = 0;
        }

        /// <summary>
        /// 获取或设置精灵当前朝向:0朝上4朝下,顺时针依次为0,1,2,3,4,5,6,7(关联属性)
        /// </summary>
        public double Direction
        {
            get { return (double)GetValue(DirectionProperty); }
            set { SetValue(DirectionProperty, value); }
        }
        #endregion
        #region 单位属性
        /// <summary>
        /// 属性
        /// </summary>
        UnitProperties UnitProperties;
        #endregion
        #region 初始化方法

        public QXSprite(int imgType)
        {
            InitializeComponent();
            InitThread(imgType);
            var img = GV.Shadow.GetImg(0,0);
            ShadowBody.Source = img;
            ShadowBody.Width = img.Width;
            ShadowBody.Height = img.Height;
            Canvas.SetZIndex(ShadowCanvas, -1);
        }
        public QXSprite()
        {
            InitializeComponent();
            InitThread();
        }

        private void InitThread(int imgType = 0)
        {
            if (imgType == 0)
            {
                Ellipse player = new Ellipse(); //用一个圆来模拟目标对象
                player.Fill = new SolidColorBrush(Colors.Red);
                player.Width = 10;
                player.Height = 10;
                Canvas.SetLeft(player, CenterX - player.Width / 2);
                Canvas.SetTop(player, CenterY - player.Height / 2);
                ImgCanvas.Children.Add(player);
            }
            else
            {
                Timer.Interval = TimeSpan.FromMilliseconds(100);
                Timer.Tick += new EventHandler(Timer_Tick);
                Timer.Start();
            }

        }
        #endregion


        /// <summary>
        /// 间隔事件（生命钟）
        /// </summary>
        private void Timer_Tick(object sender, EventArgs e)
        {
            if (Action == Actions.Stop)
            {
                var img = WasStantFile.GetImg(GV.DirectImg[(int)Direction], Count[0]);
                Body.Source = img;
                Body.Width = img.Width;
                Body.Height = img.Height;
                Canvas.SetLeft(Body, CenterX - WasStantFile.CentryX);
                Canvas.SetTop(Body, CenterY - WasStantFile.CentryY);
                Count[0] = Count[0] == WasStantFile.FrameCount - 1 ? 0 : Count[0] + 1;
                
            }
            else if (Action == Actions.Run)
            {
                var img = WasRunFile.GetImg(GV.DirectImg[(int)Direction], Count[0]);
                Body.Source = img;
                Body.Width = img.Width;
                Body.Height = img.Height;
                Canvas.SetLeft(Body, CenterX - WasRunFile.CentryX);
                Canvas.SetTop(Body, CenterY - WasRunFile.CentryY);
                Count[0] = Count[0] == WasRunFile.FrameCount - 1 ? 0 : Count[0] + 1;
                //SName.Text = Count[0].ToString();
            }
            else if (Action == Actions.Attack)
            {
                if (AttackAction == AttackActions.Stop)
                {
                    var img = WasFiles[0].GetImg(0, Count[0]);
                    Body.Source = img;
                    Body.Width = img.Width;
                    Body.Height = img.Height;
                    Canvas.SetLeft(Body, CenterX - WasStantFile.CentryX);
                    Canvas.SetTop(Body, CenterY - WasStantFile.CentryY);
                    Count[0] = Count[0] == Count[1] ? 0 : Count[0] + 1;
                }
            }
        }

        public void Attack(QXSprite target)
        {
            var storyboard = new Storyboard();
            var targetCoordinate = new Point(300, 300);
            var runImg = WasFiles[1];
            var attactImg = WasFiles[2];
            var backImg = WasFiles[3];
            #region 创建坐标变换属性动画
            AttackAction = AttackActions.Run;
            PointAnimation run = new PointAnimation()
            {
                From = AttackCoordinate,
                To = targetCoordinate,
                Duration = new Duration(TimeSpan.FromMilliseconds(1000)),

            };
            Storyboard.SetTarget(run, this);
            Storyboard.SetTargetProperty(run, new PropertyPath("AttackCoordinate"));
            run.Completed += (sender, e) => AttackAction = AttackActions.Attack;
            PointAnimation attack = new PointAnimation()
            {
                From = targetCoordinate,
                To = targetCoordinate,
                Duration = new Duration(TimeSpan.FromMilliseconds(1000)),
                BeginTime = TimeSpan.FromMilliseconds(1000),

            };
            Storyboard.SetTarget(attack, this);
            Storyboard.SetTargetProperty(attack, new PropertyPath("AttackCoordinate"));
            attack.Completed += (sender, e) => AttackAction = AttackActions.Back;
            PointAnimation back = new PointAnimation()
            {
                From = targetCoordinate,
                To = AttackCoordinate,
                Duration = new Duration(TimeSpan.FromMilliseconds(1000)),
                BeginTime = TimeSpan.FromMilliseconds(2000),
            };
            Storyboard.SetTarget(back, this);
            Storyboard.SetTargetProperty(back, new PropertyPath("AttackCoordinate"));
            back.Completed += (sender, e) => AttackAction = AttackActions.Stop;

            storyboard.Children.Add(run);
            storyboard.Children.Add(attack);
            storyboard.Children.Add(back);
            #endregion
            storyboard.Begin();
        }
        #region 属性绑定
        /// <summary>
        /// 坐标
        /// </summary>
        public static readonly DependencyProperty CoordinateProperty = DependencyProperty.Register(
            "Coordinate",
            typeof(Point),
            typeof(QXSprite),
            new PropertyMetadata(ChangeCoordinateProperty)
        );
        /// <summary>
        /// 坐标
        /// </summary>
        public static readonly DependencyProperty AttackCoordinateProperty = DependencyProperty.Register(
            "AttackCoordinate",
            typeof(Point),
            typeof(QXSprite),
            new PropertyMetadata(ChangeAttackCoordinateProperty)
        );

        /// <summary>
        /// 方向
        /// </summary>
        public static readonly DependencyProperty DirectionProperty = DependencyProperty.Register(
           "Direction",
           typeof(double),
           typeof(QXSprite),
           null
       );


        #endregion
        /// <summary>
        /// 坐标改变事件
        /// </summary>
        private static void ChangeCoordinateProperty(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            QXSprite obj = (QXSprite)d;
            if (obj.Visibility == Visibility.Visible)
            {
                Point p = (Point)e.NewValue;
                obj.Position[0] = p.ToPosition();
                obj.SetValue(Canvas.LeftProperty, p.X - GV.WindowOffsetX - CenterX);
                obj.SetValue(Canvas.TopProperty, p.Y - GV.WindowOffsetY - CenterY);
                obj.SetValue(Canvas.ZIndexProperty, (int)p.Y);
                obj.SName.Text = string.Format("p.X={0:f0},p.Y={1:f0},\nCenterX={2},CenterY={3}", p.X, p.Y, GV.WindowOffsetX, GV.WindowOffsetY);
                if (obj.CoordinateChanged != null)
                {
                    obj.CoordinateChanged(obj);
                }
            }
        }
        /// <summary>
        /// 坐标改变事件
        /// </summary>
        private static void ChangeAttackCoordinateProperty(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            QXSprite obj = (QXSprite)d;
            if (obj.Visibility == Visibility.Visible)
            {
                Point p = (Point)e.NewValue;
                obj.SetValue(Canvas.LeftProperty, p.X - CenterX);
                obj.SetValue(Canvas.TopProperty, p.Y - CenterY);
                obj.SetValue(Canvas.ZIndexProperty, (int)p.Y);
                var img = obj.WasFiles[(int)obj.AttackAction];
                obj.Body.Source = img.GetImg(0, obj.Count[2]);
                obj.Body.Width = img.Width;
                obj.Body.Height = img.Height;
                Canvas.SetLeft(obj.Body, CenterX - img.CentryX);
                Canvas.SetTop(obj.Body, CenterY - img.CentryY);
                //obj.SName.Text = string.Format("p.X={0:f0},p.Y={1:f0},\nCenterX={2},CenterY={3}", p.X, p.Y, GV.WindowOffsetX, GV.WindowOffsetY);

            }
        }



    }
}
