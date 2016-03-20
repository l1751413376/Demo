using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Animation;

namespace Beta.Controls
{
    public static class Super
    {
        /// <summary>
        /// 计算出动画时间花费
        /// </summary>
        /// <param name="unitCost">每个格子移动所需时间ms(越小越快)</param>
        public static double GetAnimationTimeConsuming(Point start, Point end, int gridSize, double unitCost)
        {
            return Math.Sqrt(Math.Pow((end.X - start.X) / gridSize, 2) + Math.Pow((end.Y - start.Y) / gridSize, 2)) * unitCost;
        }

        public static void NewStoryboard(this Dictionary<string, Storyboard> storyboard, string key) 
        {
            storyboard.RemoveStoryboard(key);
            storyboard.Add(key, new Storyboard());
        }

        /// <summary>
        /// 暂停精灵Storyboard移动动画板
        /// </summary>
        /// <param name="sprite">对象精灵</param>
        public static void RemoveStoryboard(this Dictionary<string, Storyboard> storyboard, string key)
        {
            if (storyboard.ContainsKey(key))
            {
                storyboard[key].Pause();
                storyboard[key] = null;
                storyboard.Remove(key);
            }
        }
    }
  

    /// <summary>
    /// 坐标改变委托
    /// </summary>
    public delegate void CoordinateEventHandler(QXSprite sprite);
}
