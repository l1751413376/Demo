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

namespace Beta.Controls
{
    /// <summary>
    /// QXFacePlate.xaml 的交互逻辑
    /// </summary>
    public partial class QXFacePlate : UserControl
    {
        public static int MaxWidth = 130;
        //life
        LinearGradientBrush[] rainbowBrush = new LinearGradientBrush[]
        {
            //HP
            Super.CreateRainbowBrush(255, 6, 17, 228, 6, 17),
            //MP
            Super.CreateRainbowBrush(35, 73, 255, 35, 73, 182),
            //TP
            Super.CreateRainbowBrush(26, 255, 51, 26, 181, 51),
            
            Super.CreateRainbowBrush(202, 95, 235, 77, 40, 92),

        };
        public QXFacePlate()
        {
            InitializeComponent();
            HPVector.Fill = rainbowBrush[0];
            MPVector.Fill = rainbowBrush[1];
            TPVector.Fill = rainbowBrush[3];
        }

        public void SetValue(int type, double[] value)
        {
            switch (type)
            {
                case 0:
                    HPVector.Width = value[0] / value[1] * MaxWidth;
                    HPText.Text = string.Format("{0:f0}/{1:f0}", value[0], value[1]);
                    break;
                case 1:
                    MPVector.Width = value[0] / value[1] * MaxWidth;
                    MPText.Text = string.Format("{0:f0}/{1:f0}", value[0], value[1]);
                    break;
                case 2:
                    TPVector.Width = value[0] / value[1] * MaxWidth;
                    TPText.Text = string.Format("{0:f0}/{1:f0}", value[0], value[1]);
                    break;
            }

        }
    }
}
