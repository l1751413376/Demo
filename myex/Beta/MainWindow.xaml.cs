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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Beta
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window , INotifyPropertyChanged
    {
        public int data { get; set; }
        public MainWindow()
        {
            InitializeComponent();


            IntPtr PngBuff = IntPtr.Zero;
            var len=CppAPI.GetBitMap(out PngBuff);
            byte[] managed_data = new byte[len];
            Marshal.Copy(PngBuff, managed_data, 0, len);
            MemoryStream ms = new MemoryStream(managed_data);

            BitmapImage image = new BitmapImage();
            image.BeginInit();
            image.StreamSource = ms;
            image.EndInit();
            bgMap.Source = image;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
           
        }
        

        

    }
}
