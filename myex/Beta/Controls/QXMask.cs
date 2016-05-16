using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;

namespace Beta.Controls
{
    public class QXMask : QXMap
    {
        public MaskInfo MaskInfo;

        public static void MoveMask(Canvas content)
        {
            foreach (var item in content.Children)
            {
                if (item.GetType() != typeof(QXMask))
                {
                    continue;
                }
                var maskItem = item as QXMask;
                int xMin = GV.WindowOffsetX;
                int xMax = GV.WindowOffsetX + GV.WindowWidth;
                int yMin = GV.WindowOffsetY;
                int yMax = GV.WindowOffsetY + GV.WindowHegith;
                int mxMin = maskItem.MaskInfo.startX;
                int mxMax = maskItem.MaskInfo.startX + maskItem.MaskInfo.width;
                int myMin = maskItem.MaskInfo.startY;
                int myMax = maskItem.MaskInfo.startY + maskItem.MaskInfo.height;

                int nMaxLeft = mxMin >= xMin ? mxMin : xMin;
                int nMaxTop = myMin >= yMin ? myMin : yMin;
                int nMinRight = mxMax <= xMax ? mxMax : xMax;
                int nMinBottom = myMax <= yMax ? myMax : yMax;
                // 判断是否相交  
                if (nMaxLeft > nMinRight || nMaxTop > nMinBottom)
                {
                    maskItem.Visibility = Visibility.Hidden;
                }
                else
                {
                    maskItem.Visibility = Visibility.Visible;
                    Canvas.SetLeft(maskItem, maskItem.MaskInfo.startX+ GV.WindowOffsetX);
                    Canvas.SetTop(maskItem, maskItem.MaskInfo.startY + GV.WindowOffsetY);
                }
                


            }
        }


    }
}
