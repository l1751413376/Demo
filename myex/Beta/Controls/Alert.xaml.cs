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
    /// Alert.xaml 的交互逻辑
    /// </summary>
    public partial class Alert : UserControl
    {
        public Alert()
        {
            InitializeComponent();
        }


        public static Canvas Parent;
        public static void Init(Canvas p)
        {
            Parent = p;
        }
        public static void Show(String message)
        {
            Alert alert = new Alert();
            alert.Message.Text = message;
            //计算中心点
            Parent.Children.Add(alert);

            alert.UpdateLayout();
            var cx = Parent.Width / 2;
            var cy = Parent.Height / 2;
            var list = new List<HitTestResult>();
            //命中点
            //VisualTreeHelper.HitTest(Parent, null, new HitTestResultCallback(r => { list.Add(r); return HitTestResultBehavior.Continue; }), new PointHitTestParameters(new Point(349, 282)));

            var ccx = alert.Border1.ActualWidth;
            var ccy = alert.Border1.ActualHeight;
            Canvas.SetLeft(alert, cx - ccx);
            Canvas.SetTop(alert, cy - ccy);
            Canvas.SetZIndex(alert, 10000);
            var timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(3000);
            timer.Tag = alert;
            timer.Tick += new EventHandler(EventHandlerInstance);
            timer.Start();
        }

        public static void EventHandlerInstance(object s, EventArgs e)
        {
            DispatcherTimer timer = s as DispatcherTimer;
            var alert = timer.Tag as Alert;
            timer.Stop();
            Parent.Children.Remove(alert);
        }
        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
            Parent.Children.Remove(this);
        }
    }
}
