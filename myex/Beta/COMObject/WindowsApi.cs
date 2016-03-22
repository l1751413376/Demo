using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Beta
{
    public static class WindowsAPI
    {
        [DllImport(@"user32.lib")]
        public extern static bool AdjustWindowRect(IntPtr lpRect,UInt64 dwStyle,bool bMenu);
    }
}
