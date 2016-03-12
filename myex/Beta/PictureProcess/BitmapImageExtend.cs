using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace Beta.PictureProcess
{
    public static class BitmapImageExtend
    {
        /// <summary>
        /// 从字节从获取
        /// </summary>
        public static void FromBytes(this BitmapImage bitmapImage,byte[] bytes) 
        {
            MemoryStream ms = new MemoryStream(bytes);
            bitmapImage.BeginInit();
            bitmapImage.StreamSource = ms;
            bitmapImage.EndInit();
        }
        /// <summary>
        /// 从非托管内存获取
        /// </summary>
        public static void FromIntPtr(this BitmapImage bitmapImage, IntPtr intPtr,int buffLen)
        {
            IntPtr PngBuff = IntPtr.Zero;
            byte[] managed_data = new byte[buffLen];
            Marshal.Copy(intPtr, managed_data, 0, buffLen);
            FromBytes(bitmapImage, managed_data);
        }
    }
}
