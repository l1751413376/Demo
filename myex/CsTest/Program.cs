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
            var str = "尊敬的客户，您预订的#@VisaLineTitle#，签证客户单号#@VisaCustomerSerialid#已经有出签结果，出签结果已经发送到您的邮箱：#@VisaContractMail#，请注意查收；护照本等相关材料已经邮寄至您预留的地址：#@VisaPMStreetAddress#，请注意清收。同程旅游，快乐每一程。";
            var rex = new Regex(@"#@C#", RegexOptions.Multiline); 
            var matchs=rex.Matches(str,0);
            foreach(var item in matchs)
            {
                Console.Write(item);
            }

        }

        public class a 
        {
            public int pa;
        }

        public class b:a
        {
            public int pa=10;
        }
    }

    

}