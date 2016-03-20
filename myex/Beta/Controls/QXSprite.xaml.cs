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
    public partial class QXSprite : UserControl,QXObject
    {

        /// <summary>
        /// 图片位于控件中心的位置X
        /// </summary>
        const int CentryX = 75;
        /// <summary>
        /// 图片位于控件中心的位置Y
        /// </summary>
        const int CentryY = 120;

        /// <summary>
        /// 图片类型0-图形1-was文件
        /// </summary>
        public int ImgType;
        /// <summary>
        /// 窗口对于地图的偏移量x
        /// </summary>
        public int CenterX { get { return GV.CenterX; } set { GV.CenterX = value; } }
        /// <summary>
        /// 窗口对于地图的偏移量Y
        /// </summary>
        public int CenterY { get { return GV.CenterY; } set { GV.CenterY = value; } }
        /// <summary>
        /// 获取跑步速度ms(每移动一个单元格的花费时间,越小越快)
        /// </summary>
        public double VRunSpeed = 50;
        public QXSprite()
        {
            InitializeComponent();
            InitThread();
        }
        public DispatcherTimer Timer = new DispatcherTimer();

        private void InitThread()
        {
            if (ImgType == 0)
            {
                Ellipse player = new Ellipse(); //用一个圆来模拟目标对象
                player.Fill = new SolidColorBrush(Colors.Red);
                player.Width = 10;
                player.Height = 10;
                Canvas.SetLeft(player, 0);
                Canvas.SetTop(player, 0);
                ImgCanvas.Children.Add(player);
            }
            else 
            {
                Timer.Tick += new EventHandler(Timer_Tick);
                Timer.Start();
            }
           
        }
        public string ImageAddress;
        //精灵线程间隔事件
        public WasFile wasFile;
        public int count;
        public int limitcount;
        private void Timer_Tick(object sender, EventArgs e)
        {
            
            var img = wasFile.GetImg(0, count);
            Body.Source = img;
            Body.Width = img.Width;
            Body.Height = img.Height;
            Canvas.SetLeft(Body, CentryX - wasFile.CentryX);
            Canvas.SetTop(Body, CentryY - wasFile.CentryY);
            count = count == limitcount ? 0 : count + 1;
        }
        //精灵X坐标(关联属性)

        public double X
        {

            get { return (double)GetValue(XProperty); }

            set { SetValue(XProperty, value); }

        }
        public double Y
        {

            get { return (double)GetValue(YProperty); }

            set { SetValue(YProperty, value); }

        }
        public static readonly DependencyProperty XProperty = DependencyProperty.Register(
            "X", //属性名

            typeof(double), //属性类型

            typeof(QXSprite), //属性主人类型

            new FrameworkPropertyMetadata(

                 (double)0, //初始值0

                 FrameworkPropertyMetadataOptions.None, //不特定界面修改

                 //不需要属性改变回调

                 null,//new PropertyChangedCallback(QXSpiritInvalidated),

                 //不使用强制回调

                 null

                 ));
        //精灵Y坐标(关联属性)

        

        public static readonly DependencyProperty YProperty = DependencyProperty.Register(

            "Y",

            typeof(double),

            typeof(QXSprite),

            new FrameworkPropertyMetadata(

                 (double)0,

                 FrameworkPropertyMetadataOptions.None,

                 null,

                 null

                 )

            );
        public event CoordinateEventHandler CoordinateChanged;
        /// <summary>
        /// 获取或设置精灵坐标(关联属性)
        /// </summary>
        public Point Coordinate
        {
            get { return (Point)GetValue(CoordinateProperty); }
            set { SetValue(CoordinateProperty, value); }
        }
        public static readonly DependencyProperty CoordinateProperty = DependencyProperty.Register(
            "Coordinate",
            typeof(Point),
            typeof(QXSprite),
            new PropertyMetadata(ChangeCoordinateProperty)
        );
        private static void ChangeCoordinateProperty(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            QXSprite obj = (QXSprite)d;
            if (obj.Visibility == Visibility.Visible)
            {
                Point p = (Point)e.NewValue;
                obj.SetValue(Canvas.LeftProperty, p.X - obj.CenterX);
                obj.SetValue(Canvas.TopProperty, p.Y - obj.CenterY);
                obj.SetValue(Canvas.ZIndexProperty, (int)p.Y);
                if (obj.CoordinateChanged != null)
                {
                    obj.CoordinateChanged(obj);
                }
            }
        }
        /// <summary>
        /// 动作
        /// </summary>
        public Actions Action;



        
    }
}
