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
            data = 2;
            InitializeComponent();
            BitmapImage bi = new BitmapImage();
            
            
        }
        public event PropertyChangedEventHandler PropertyChanged;
        private void slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
           
        }
        

        

    }
}
