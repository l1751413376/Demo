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

            var element = VisualTreeHelper.HitTest(Parent, (p) => { return HitTestFilterBehavior.Continue; }, null, null);
            var el = (Image)element.VisualHit;
            var ccx = alert.Border1.ActualWidth;
            var ccy = alert.Border1.ActualHeight;
            Canvas.SetLeft(alert, cx - ccx);
            Canvas.SetTop(alert, cy - ccy);
            Canvas.SetZIndex(alert, 10000);
            var timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(5000);
            timer.Tick += new EventHandler((s, e) => { timer.Stop(); timer = null; Parent.Children.Remove(alert); });
            timer.Start();
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
            Parent.Children.Remove(this);
        }
    }
}
