using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace Beta.Controls
{
    interface QXObject
    {
        /// <summary>
        /// 窗口对于地图的偏移量x
        /// </summary>
        int CenterX { get; set; }
        //int CenterX { get { return GV.CenterX; } set { GV.CenterX = value; } }
        /// <summary>
        /// 窗口对于地图的偏移量Y
        /// </summary>
        int CenterY { get; set; }
        //int CenterY { get { return GV.CenterY; } set { GV.CenterY = value; } }
        /// <summary>
        /// 物体在地图上面的坐标X
        /// </summary>
        double X { get; set; }
        /// <summary>
        /// 物体在地图上面的坐标Y
        /// </summary>
        double Y { get; set; }
    }
}
