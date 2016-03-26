﻿using System;
using System.Collections.Generic;
using System.Linq;
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

        /// <summary>
        /// 通过正切值获取精灵的朝向代号
        /// </summary>
        /// <returns>精灵朝向代号(以北为0顺时针依次1,2,3,4,5,6,7)</returns>
        public static double GetDirectionByTan(Point target,Point current)
        {
            double targetX=target.X, targetY=target.Y;
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
        
    }
    /// <summary>
    /// 坐标转换
    /// </summary>
    public static class PointHelper 
    {

        public static int GridSize = 10;

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
        /// 地图位置->地图坐标
        /// </summary>
        public static Point ToCoordinate(this Point p)
        {
            p.X = p.X * GridSize + GridSize / 5;
            p.Y = p.Y * GridSize + GridSize / 5;
            return p;
        }
        /// <summary>
        /// 地图坐标->地图位置
        /// </summary>
        public static Point ToPosition(this Point p)
        {
            p.X = (int)(p.X / GridSize);
            p.X = (int)(p.Y / GridSize);
            return p;
        }

    }

    /// <summary>
    /// 坐标改变委托
    /// </summary>
    public delegate void CoordinateEventHandler(QXSprite sprite);
}
