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
       
           
        public static Regex RegProtocol = new Regex("(GET|POST)|([^ ]+)");
        public static Regex RegItem = new Regex("([^ :]+)");
        static void Main(string[] args)
        {
            var str = @"POST http://ebk.t.17u.cn/ivacation/VisaInfo/UploadETA?q=1&PassengerId=3891314&PassengerName=%E6%B5%8B%E8%AF%95&CustomerSerialId=&OrderSerialid=undefined HTTP/1.1
Host: ebk.t.17u.cn
Connection: keep - alive
Content - Length: 846125
Pragma: no - cache
Cache - Control: no - cache
Accept: */*
Origin: http://ebk.t.17u.cn
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.122 Safari/537.36 SE 2.X MetaSr 1.0
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryySzFkyO1blOdPmrr
Referer: http://ebk.t.17u.cn/ivacation/VisaInfo/VisaOrderList
Accept-Encoding: gzip,deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: ASP.NET_SessionId=vqbklzbaukniy32ehd3jmti1; Hm_lvt_efeba49561e17cb591fcf93d3a48f843=1463718412; Hm_lpvt_efeba49561e17cb591fcf93d3a48f843=1463718412

------WebKitFormBoundaryySzFkyO1blOdPmrr
";

            var matches = RegItem.Matches(str);

            _HttpRequest request = new _HttpRequest();
            request.ParseFromByte(Encoding.UTF8.GetBytes(str));
        }

    }

    public class _HttpRequest
    {
        public Dictionary<string, string> Parameter = new Dictionary<string, string>();
        private int Cols;

        #region Reg
        public static Regex RegProtocol = new Regex("(GET|POST)|[^ ]+");
        public static Regex RegItem = new Regex("([^ :]+)");

        #endregion
        public void ParseFromByte(byte[] buffer)
        {
            Cols = 0;
            MemoryStream stream = new MemoryStream(buffer);
            HeaderProcess(ReadCol(stream));
            string content;
            while ((content = ReadCol(stream)) != null)
            {
                ItemProcess(content);
            }

        }
        public string ReadCol(MemoryStream stream)
        {
            var p = stream.Position;
            var count = 0;
            while (true)
            {
                if (stream.ReadByte() == 0x0D && stream.ReadByte() == 0x0A)
                {
                    break;
                }
                count++;
            }
            if (count == 0)
            {
                return null;//head 结束
            }
            stream.Seek(p, SeekOrigin.Begin);
            count += 2;
            var content = new byte[count];
            stream.Read(content, 0, count);
            var str = Encoding.UTF8.GetString(content);
            return str;
        }
        public void HeaderProcess(string content)
        {
            var matches = RegProtocol.Matches(content);
            Parameter.Add("Method", matches[0].Value);
            Parameter.Add("Url", matches[1].Value);
            Parameter.Add("Protocol", matches[2].Value);
        }
        public void ItemProcess(string content)
        {
            var Match = RegItem.Match(content);
            Parameter.Add(Match.Value, content.Replace(Match.Value+": ", ""));

        }
    }

}