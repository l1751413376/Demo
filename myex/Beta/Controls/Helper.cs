using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

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

        public static void NewStoryboard(this Dictionary<string, Storyboard2> storyboard, string key)
        {
            storyboard.RemoveStoryboard(key);
            storyboard.Add(key, new Storyboard2());
        }

        /// <summary>
        /// 暂停精灵Storyboard移动动画板
        /// </summary>
        /// <param name="sprite">对象精灵</param>
        public static void RemoveStoryboard(this Dictionary<string, Storyboard2> storyboard, string key)
        {
            if (storyboard.ContainsKey(key))
            {
                storyboard[key].Pause();
                storyboard[key].Completed2 = null;
                storyboard[key] = null;
                storyboard.Remove(key);
            }
        }

        /// <summary>
        /// 通过正切值获取精灵的朝向代号
        /// </summary>
        /// <returns>精灵朝向代号(以北为0顺时针依次1,2,3,4,5,6,7)</returns>
        public static double GetDirectionByTan(Point target, Point current)
        {
            double targetX = target.X, targetY = target.Y;
            double currentX = current.X, currentY = current.Y;
            double tan = (targetY - currentY) / (targetX - currentX);
            var tanAbs = Math.Abs(tan);
            var tanPI38 = Math.Tan(Math.PI * 3 / 8);
            var tanPI8 = Math.Tan(Math.PI / 8);
            if (tanAbs >= tanPI38 && targetY <= currentY)
            {
                return 0;
            }
            else if (tanAbs > tanPI8 && tanAbs < tanPI38 && targetX > currentX && targetY < currentY)
            {
                return 1;
            }
            else if (tanAbs <= tanPI8 && targetX >= currentX)
            {
                return 2;
            }
            else if (tanAbs > tanPI8 && tanAbs < tanPI38 && targetX > currentX && targetY > currentY)
            {
                return 3;
            }
            else if (tanAbs >= tanPI38 && targetY >= currentY)
            {
                return 4;
            }
            else if (tanAbs > tanPI8 && tanAbs < tanPI38 && targetX < currentX && targetY > currentY)
            {
                return 5;
            }
            else if (tanAbs <= tanPI8 && targetX <= currentX)
            {
                return 6;
            }
            else if (tanAbs > tanPI8 && tanAbs < tanPI38 && targetX < currentX && targetY < currentY)
            {
                return 7;
            }
            else
            {
                return 0;
            }
        }

        /// <summary>
        /// 寻路模式中根据单元格方向来判断精灵朝向
        /// </summary>
        /// <returns>精灵朝向代号(以北为0顺时针依次1,2,3,4,5,6,7)</returns>
        public static double GetDirectionByAspect(int targetX, int targetY, int currentX, int currentY)
        {
            int direction = 2;
            if (targetX < currentX)
            {
                if (targetY < currentY)
                {
                    direction = 7;
                }
                else if (targetY == currentY)
                {
                    direction = 6;
                }
                else if (targetY > currentY)
                {
                    direction = 5;
                }
            }
            else if (targetX == currentX)
            {
                if (targetY < currentY)
                {
                    direction = 0;
                }
                else if (targetY > currentY)
                {
                    direction = 4;
                }
            }
            else if (targetX > currentX)
            {
                if (targetY < currentY)
                {
                    direction = 1;
                }
                else if (targetY == currentY)
                {
                    direction = 2;
                }
                else if (targetY > currentY)
                {
                    direction = 3;
                }
            }
            return direction;
        }

        public static LinearGradientBrush CreateRainbowBrush(byte r0, byte g0, byte b0, byte r1, byte g1, byte b1)
        {
            return new LinearGradientBrush()
            {
                StartPoint = new Point(0, 0),
                EndPoint = new Point(0, 1),
                GradientStops = new GradientStopCollection() {
                    new GradientStop() {
                        Color = Color.FromArgb(255, r0, g0, b0),
                        Offset = 0
                    },
                    new GradientStop() {
                        Color = Color.FromArgb(255, r1, g1, b1),
                        Offset = 1
                    }
                }
            };
        }

        public static void MoveBitmap(this Image image, BitmapSource source)
        {
            image.Source = source;
            image.Width = source.PixelWidth;
            image.Height = source.PixelHeight;
        }

        public static void FillColour(this TextBlock text, string colour)
        {
            text.Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString(colour));
        }
        public static byte[] ToBuffer(this IntPtr pointer,int size)
        {
            byte[] managed_data = new byte[size];
            Marshal.Copy(pointer, managed_data, 0, size);
            CppAPI.Delete(pointer);
            return managed_data;
        }

        public static MemoryStream ToStream(this IntPtr pointer, int size)
        {
            MemoryStream stream = new MemoryStream(ToBuffer(pointer,size));
            return stream;
        }

        public static uint ReadDword(this FileStream stream)
        {
            byte[] b = new byte[4];
            stream.Read(b, 0, 4);
            return BitConverter.ToUInt32(b, 0);
        }
        public static uint ReadDword(this MemoryStream stream)
        {
            byte[] b = new byte[4];
            stream.Read(b, 0, 4);
            return BitConverter.ToUInt32(b, 0);
        }
        #region 从缓冲区获得图片实例
        /// <summary>
        /// 从字节从获取
        /// </summary>
        public static void FromBytes(this BitmapImage bitmapImage, byte[] bytes)
        {
            MemoryStream ms = new MemoryStream(bytes);
            bitmapImage.BeginInit();
            bitmapImage.StreamSource = ms;
            bitmapImage.EndInit();
        }
        /// <summary>
        /// 从非托管内存获取
        /// </summary>
        public static void FromIntPtr(this BitmapImage bitmapImage, IntPtr intPtr, int buffLen)
        {
            IntPtr PngBuff = IntPtr.Zero;
            byte[] managed_data = new byte[buffLen];
            Marshal.Copy(intPtr, managed_data, 0, buffLen);
            FromBytes(bitmapImage, managed_data);
        }
        #endregion

    }
    /// <summary>
    /// 坐标转换
    /// </summary>
    public static class PointHelper
    {

        public static int GridSize = 20;

        /// <summary>
        /// 窗口坐标->地图坐标
        /// </summary>
        public static Point ToMapCoordinate(this Point p)
        {
            p.X += GV.WindowOffsetX;
            p.Y += GV.WindowOffsetY;
            return p;
        }
        /// <summary>
        /// 地图坐标->窗口坐标
        /// </summary>
        public static Point ToWindowCoordinate(this Point p)
        {
            p.X -= GV.WindowOffsetX;
            p.Y -= GV.WindowOffsetY;
            return p;
        }

        /// <summary>
        /// 位置->坐标
        /// </summary>
        public static Point ToCoordinate(this Point p)
        {
            p.X = p.X * GridSize + GridSize / 2;
            p.Y = p.Y * GridSize + GridSize / 2;
            return p;
        }
        /// <summary>
        /// 坐标->位置
        /// </summary>
        public static Point ToPosition(this Point p)
        {
            p.X = (int)(p.X / (GridSize));
            p.Y = (int)(p.Y / (GridSize));
            return p;
        }

    }

    /// <summary>
    /// 坐标改变委托
    /// </summary>
    public delegate void CoordinateEventHandler(QXSprite sprite);

    public class Storyboard2 : Storyboard
    {
        public EventHandler Completed2;
        public Storyboard2()
        {
            Completed += CompletedMethod;
        }
        public void CompletedMethod(object sender, EventArgs e)
        {
            if (Completed2 != null)
            {
                Completed2(sender, e);
            }
        }
    }
}



