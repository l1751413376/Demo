using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
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

namespace WebService
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        const char c = '\r';
        public MainWindow()
        {
            InitializeComponent();
        }
        Socket Master;
        ExitEventHandler func = null;
        private void button_Click(object sender, RoutedEventArgs e)
        {
            var btn = ((Button)sender);

            if (btn.Content.ToString() == "启动")
            {
                //创建IP地址
                IPAddress address = IPAddress.Parse("127.0.0.1");
                //创建IP节点(包含IP和端口)
                IPEndPoint endpoint = new IPEndPoint(address, int.Parse("520"));
                //创建监听套接字(寻址协议，流方式，TCP协议)
                Master = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                Master.Bind(endpoint);
                Master.Listen(10);//参数：指能同时处理的连接数
                Thread threadWatchPort = new Thread(WatchPort);
                func = (s1, e1) => { Master.Close(); threadWatchPort.Abort(); };
                threadWatchPort.Start();
                Application.Current.Exit += func;
            }
            else
            {
                if (func != null)
                {
                    func(null, null);
                }
            }


            btn.Content = btn.Content.ToString() == "启动" ? "关闭" : "启动";

        }


        public void WatchPort()
        {
            while (true)
            {
                try
                {
                    Socket RequestSocket = Master.Accept();
                    new Thread(() => ProcessRequestSocket(RequestSocket)).Start();
                }
                catch (Exception ex)
                {

                }
            }
        }

        public void ProcessRequestSocket(Socket s)
        {
            var response = @"
HTTP/1.1
Cache-Control: private
Content-Type: text/html; charset=utf-8
Vary: Accept-Encoding
Content-Length: 10

HelloWorld";
            Byte[] bytesSent = Encoding.ASCII.GetBytes(response);
            //接收信息
            _HttpRequest http = new _HttpRequest();
            http.ParseFromSocket(s);
            s.Send(bytesSent, bytesSent.Length, 0);
            s.Close();
            
            var img = new BitmapImage();
            var key = http.Files.Keys.First();
            MemoryStream ms = new MemoryStream(http.Files[key].File);
            img.BeginInit();
            img.StreamSource = ms;
            img.EndInit();
            //() => Content.Background = new ImageBrush(img)
            
            img.Freeze();
            this.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
            {
                Content.Background = new ImageBrush(img);
            });
            
        }


    }
}
