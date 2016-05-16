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
        public MainWindow()
        {
            InitializeComponent();
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog loadMap = new OpenFileDialog()
            {

                CheckFileExists = true,

                CheckPathExists = true,

                Multiselect = false,

                Filter = "图像文件(*.jpg,*.png)|*.jpg;*.png",

            };

            loadMap.FileOk += new System.ComponentModel.CancelEventHandler(loadMap_FileOk);

            loadMap.ShowDialog();
        }

        public void loadMap_FileOk(object sender, CancelEventArgs e)
        {
            
        }
    }
}
