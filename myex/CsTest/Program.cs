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
            String str = @"
4661377001 华泰A计划 1-7天
4661377002	华泰A计划 8-12天
4661377003	华泰A计划 13-15天
4661377004	华泰A计划 16-21天
4661377005	华泰A计划 22-24天
4661377006	华泰A计划 25-28天
4661377007	华泰A计划 29-35天
4661377008	华泰A计划 36-42天
4661377009	华泰A计划 43-49天";
            Dictionary<string, string> InsuranceDictionary;
            var items = str.Split('\n');
            InsuranceDictionary = new Dictionary<string, string>();
            foreach (var item in items)
            {
                int spaceindex = 0;
                if ((spaceindex = item.IndexOf('\t')) == -1 && (spaceindex = item.IndexOf((char)32)) == -1)
                {
                    continue;
                }   
                
                InsuranceDictionary.Add(item.Substring(0, spaceindex).Trim((char)32,'\t','\r'), item.Substring(spaceindex, item.Length - spaceindex).Trim((char)32, '\t','\r'));
            }
        }
    }

    

}