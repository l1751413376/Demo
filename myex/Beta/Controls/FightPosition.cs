using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Beta.Controls
{
    public class FightPosition
    {
        public static int GridSize = 50;
        //1.732
        public static Point getWindowPosition(double x, double y)
        {

            return new Point(

                (x - y) * 0.886 * GridSize,

                (x + y) * 0.5 * GridSize
            );
        }

        public static Point getWindowCenterPosition(double x, double y)
        {

            return new Point(

                (x - y) * 0.886 * GridSize,

                (x + y) * 0.5 * GridSize + GridSize/2
            );
        }
    }
}
