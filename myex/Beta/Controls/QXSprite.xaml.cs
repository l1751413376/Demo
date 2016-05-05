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
        /// 图片类型0-图形1-was文件
        /// </summary>
        public int ImgType;

        /// <summary>
        /// 获取跑步速度ms(每移动一个单元格的花费时间,越小越快)
        /// </summary>
        public double VRunSpeed = 50;

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
        public WasFile wasFile;

        /// <summary>
        /// 计数 0-当前,1最大值
        /// </summary>
        public int[] Count=new int[2];

        /// <summary>
        /// 获取或设置精灵坐标(关联属性)
        /// </summary>
        public Point Coordinate
        {
            get { return (Point)GetValue(CoordinateProperty); }
            set { SetValue(CoordinateProperty, value); }
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
        /// 获取或设置精灵当前朝向:0朝上4朝下,顺时针依次为0,1,2,3,4,5,6,7(关联属性)
        /// </summary>
        public double Direction
        {
            get { return (double)GetValue(DirectionProperty); }
            set { SetValue(DirectionProperty, value); }
        }
        #endregion

        #region 初始化方法

        public QXSprite()
        {
            InitializeComponent();
            InitThread();
        }

        private void InitThread()
        {
            if (ImgType == 0)
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
                Timer.Tick += new EventHandler(Timer_Tick);
                Timer.Start();
            }

        }
        #endregion
        

        /// <summary>
        /// 间隔事件
        /// </summary>
        private void Timer_Tick(object sender, EventArgs e)
        {
            var img = wasFile.GetImg(0, Count[0]);
            Body.Source = img;
            Body.Width = img.Width;
            Body.Height = img.Height;
            Canvas.SetLeft(Body, CenterX - wasFile.CentryX);
            Canvas.SetTop(Body, CenterY - wasFile.CentryY);
            Count[0] = Count[0] == Count[1] ? 0 : Count[0] + 1;
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
       

       
       
    }
}
