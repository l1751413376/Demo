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

                (x + y) * 0.5 * GridSize + GridSize / 2
            );
        }
        public static Point getGamePosition(double x, double y)
        {

            return new Point(

                (int)((y + (x / 1.732)) / GridSize),

                (int)((y - (x / 1.732)) / GridSize)

            );

        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="from">对象</param>
        /// <param name="target">目标</param>
        /// <returns>Point[0] 点 Piont[1].X朝向</returns>
        public static Point[] getAttackPosition(Point from, Point target)
        {
            var ret = new Point[2];
            from = getWindowCenterPosition(from.X, from.Y);
            var target2 = getWindowCenterPosition(target.X, target.Y);
            if (from.X >= target2.X && from.Y < target2.Y)
            {
                //1象限
                ret[0].X = target.X;
                ret[0].Y = target.Y - 1;
                ret[1].X = 1;
            }
            else if (from.X < target2.X && from.Y <= target2.Y)
            {
                //2象限
                ret[0].X = target.X - 1;
                ret[0].Y = target.Y;
                ret[1].X = 2;
            }
            else if (from.X <= target2.X && from.Y > target2.Y)
            {
                //3象限
                ret[0].X = target.X;
                ret[0].Y = target.Y + 1;
                ret[1].X = 3;
            }
            else if (from.X > target2.X && from.Y >= target2.Y)
            {
                //4象限
                ret[0].X = target.X + 1;
                ret[0].Y = target.Y;
                ret[1].X = 4;
            }
            return ret;
        }
        public static Point[] UpLayer = new Point[] { new Point(8, 0), new Point(8, -1), new Point(8, 1), new Point(8, -2), new Point(8, 2) };
        public static Point[] DownLayer = new Point[] { new Point(12, 2) };
    }
}
