using System;
using System.ComponentModel;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.Win32;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace CsTest
{
    static class Program
    {
        static Regex PicReg = new Regex(@"_\d+x\d+_", RegexOptions.ECMAScript);
        static void Main(string[] args)
        {
            var beforeWeek = DateTime.Now.AddDays(-7);
            int day=(int)beforeWeek.DayOfWeek;
            var date=beforeWeek.AddDays(-day);

        }
    }

    

}