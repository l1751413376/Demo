using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;

namespace Beta.Controls
{
    /// <summary>
    /// 全局变量 Global Variables
    /// </summary>
    public static class GV
    {
        /// <summary>
        /// 窗口对于地图的偏移量X
        /// </summary>
        public static int CenterX;
        /// <summary>
        /// 窗口对于地图的偏移量Y
        /// </summary>
        public static int CenterY;

        /// <summary>
        /// 精灵移动用Storyboard管理器
        /// </summary>
        public static Dictionary<String, Storyboard> Storyboard = new Dictionary<String, Storyboard>();

        /// <summary>
        /// 主角距离窗口的边距,超过边距时移动地图
        /// </summary>
        public const int LeaderMargin = 200;
    }
}
