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
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Beta.Controls
{
    /// <summary>
    /// Skill.xaml 的交互逻辑
    /// </summary>
    public partial class Skill : UserControl
    {
        public Skill()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 
        /// </summary>
        public EventHandler Complate;
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
        /// Was文件
        /// </summary>
        public WasFile WasFile;

        /// <summary>
        /// 图标
        /// </summary>
        public BitmapImage Icon;

        /// <summary>
        /// 计数 0-当前,1最大值
        /// </summary>
        public int[] Count = new int[2];


        /// <summary>
        /// 获取或设置精灵坐标(关联属性)
        /// </summary>
        public Point Coordinate
        {
            get { return (Point)GetValue(CoordinateProperty); }
            set { SetValue(CoordinateProperty, value); }
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
        /// <summary>
        /// 间隔事件（生命钟）
        /// </summary>
        private void Timer_Tick(object sender, EventArgs e)
        {
            var img = WasFile.GetImg(0, Count[0]);
            Body.Source = img;
            Body.Width = img.Width;
            Body.Height = img.Height;
            Canvas.SetLeft(Body, CenterX - WasFile.CentryX);
            Canvas.SetTop(Body, CenterY - WasFile.CentryY);
            Count[0]++;
            if (Count[0] == Count[1])
            {
                Complate(this,null);
            }

        }
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
        /// 坐标改变事件
        /// </summary>
        private static void ChangeCoordinateProperty(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Skill obj = (Skill)d;
            if (obj.Visibility == Visibility.Visible)
            {
                Point p = (Point)e.NewValue;
                obj.SetValue(Canvas.LeftProperty, p.X - CenterX);
                obj.SetValue(Canvas.TopProperty, p.Y - CenterY);
                obj.SetValue(Canvas.ZIndexProperty, (int)p.Y);
            }
        }

        /// <summary>
        /// 定时器
        /// </summary>
        public DispatcherTimer Timer = new DispatcherTimer();
    }
}
