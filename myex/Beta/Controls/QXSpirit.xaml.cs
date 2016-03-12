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
    public partial class QXSpirit : UserControl
    {
        public QXSpirit()
        {
            InitializeComponent();
            InitThread();
        }
        public DispatcherTimer Timer = new DispatcherTimer();

        private void InitThread()
        {

            Timer.Tick += new EventHandler(Timer_Tick);

            Timer.Start();

        }
        public string ImageAddress;
        //精灵线程间隔事件

        int count = 0;

        private void Timer_Tick(object sender, EventArgs e)
        {

            //Body.Source = new BitmapImage((new Uri(".png", UriKind.Relative)));

            count = count == 7 ? 0 : count + 1;

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

            typeof(QXSpirit), //属性主人类型

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

            typeof(QXSpirit),

            new FrameworkPropertyMetadata(

                 (double)0,

                 FrameworkPropertyMetadataOptions.None,

                 null,

                 null

                 )

            );

    }
}
