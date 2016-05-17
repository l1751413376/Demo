using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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

namespace BetaMapEdit
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private int GridSize = 20; //单位格子大小
        public MainWindow()
        {
            InitializeComponent();
        }
        SolidColorBrush redsolid = new SolidColorBrush(Colors.Red);
        private void button_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog loadMap = new OpenFileDialog()
            {

                CheckFileExists = true,

                CheckPathExists = true,

                Multiselect = false,

                Filter = "图像文件(*.jpg,*.png)|*.jpg;*.png",

            };
            
            loadMap.FileOk += new CancelEventHandler(loadMap_FileOk);
            
            loadMap.ShowDialog();
        }

        public void loadMap_FileOk(object sender, CancelEventArgs e)
        {
            OpenFileDialog dialog = sender as OpenFileDialog;
            var file=dialog.OpenFile();
            BitmapImage img = new BitmapImage();
            img.BeginInit();
            img.StreamSource = file;
            img.EndInit();


            Map.Background = new ImageBrush(img);
            Map.Height = img.Height;
            Map.Width = img.Width;

            for (var x = 0; x < img.Width; x += GridSize)
            {
                Rectangle rect = new Rectangle();
                rect.Width = 1;
                rect.Height = img.Height;
                rect.Fill = new SolidColorBrush(Colors.Red);
                Canvas.SetLeft(rect, x);
                Canvas.SetTop(rect, 0);
                Canvas.SetZIndex(rect, 100);
                GridCarrier.Children.Add(rect);
            }

            for (var x = 0; x < img.Height; x += GridSize)
            {
                Rectangle rect = new Rectangle();
                rect.Width = img.Width;
                rect.Height = 1;
                rect.Fill = new SolidColorBrush(Colors.Red);
                Canvas.SetLeft(rect, 0);
                Canvas.SetTop(rect, x);
                Canvas.SetZIndex(rect, 100);
                GridCarrier.Children.Add(rect);
            }
        }

        private void GridCarrier_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point p = e.GetPosition(Map);
            p = new Point()
            {
                X = p.X / (GridSize),
                Y = p.Y / (GridSize)
            };
            Rectangle rect = new Rectangle();

            Canvas.SetLeft(rect, (int)p.X * GridSize);
            Canvas.SetTop(rect, (int)p.Y * GridSize);
            rect.Width = GridSize;
            rect.Height = GridSize;
            rect.Fill = redsolid;
            Canvas.SetZIndex(rect, 102);
            rect.MouseLeftButtonDown += (s1,e1) => 
            {
                Map.Children.Remove((Rectangle)s1);
                e.Handled = true;
            };
            Map.Children.Add(rect);
        }
    }
}
