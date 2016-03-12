using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QXGame_Silverlight3.AStar
{
    public class Point
    {
        public int X { get; set; }
        public int Y { get; set; }
        public Point(Double x,Double y)
        {
            X = (int)x;Y = (int)y;
        }
    }
}
