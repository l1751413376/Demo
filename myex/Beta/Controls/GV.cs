using Beta.PictureProcess;
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
        static GV()
        {
            Shadow = new WasFile();
            Shadow.LoadFileFromWdf("D:\\BaiduYunDownload\\梦幻西游\\shape.wdf", 0xdce4b562);
        }

        /// <summary>
        /// 窗口对于地图的偏移量X
        /// </summary>
        public static int WindowOffsetX;
        /// <summary>
        /// 窗口对于地图的偏移量Y
        /// </summary>
        public static int WindowOffsetY;


        /// <summary>
        /// 窗口宽
        /// </summary>
        public const int WindowWidth=800;
        /// <summary>
        /// 窗口高
        /// </summary>
        public const int WindowHegith=600;
        /// <summary>
        /// 精灵移动用Storyboard管理器
        /// </summary>
        public static Dictionary<String, Storyboard2> Storyboard = new Dictionary<String, Storyboard2>();

        /// <summary>
        /// 主角距离窗口的边距,超过边距时移动地图
        /// </summary>
        public const int LeaderMargin = 270;

        public static FontFamily DroidSansFallback = new FontFamily(new Uri(@"D:\Demo\myex\Beta\Resources\DroidSansFallback.ttf"), "方正准圆-塞班修改版");


        public static int[] DirectImg = new int[] {6,3,7,0,4,1,5,2 };

        public static WasFile Shadow;
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
