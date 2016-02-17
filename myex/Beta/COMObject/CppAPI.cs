using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Beta
{
    public class CppAPI
    {
        [DllImport("Beta_CppApi.dll")]
        public extern static int add(int x, int y);
        [DllImport("Beta_CppApi.dll")]
        public extern static IntPtr GetBitMap();//与dll中一致 
    }
}
