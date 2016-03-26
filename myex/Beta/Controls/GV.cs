using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Threading;

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
        public static int WindowOffsetX;
        /// <summary>
        /// 窗口对于地图的偏移量Y
        /// </summary>
        public static int WindowOffsetY;

        /// <summary>
        /// 精灵移动用Storyboard管理器
        /// </summary>
        public static Dictionary<String, Storyboard> Storyboard = new Dictionary<String, Storyboard>();

        /// <summary>
        /// 主角距离窗口的边距,超过边距时移动地图
        /// </summary>
        public const int LeaderMargin = 270;

        public static FontFamily DroidSansFallback = new FontFamily(new Uri(@"D:\Demo\myex\Beta\Resources\DroidSansFallback.ttf"), "方正准圆-塞班修改版");


    }

    public class Cache 
    {
        public static DispatcherTimer Timer = new DispatcherTimer();
        static Cache()
        {
            Timer.Tick += new EventHandler(Timer_Tick);
            //Timer.Start();
        }
        private static void Timer_Tick(object sender, EventArgs e)
        {

        }
        public string Key;
        public DateTime Expiration;
        public object Obj;

        public static List<Cache> Caching = new List<Cache>();

        public Cache(String key, DateTime expiration,object obj) 
        {
            Key = key;
            Expiration = expiration;
            Obj = obj;
            Caching.Add(this);
        }
    }


}
