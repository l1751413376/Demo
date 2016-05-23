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
Content-Disposition: form-data; name=""p1""

123
------WebKitFormBoundaryySzFkyO1blOdPmrr
Content - Disposition: form - data; name = ""p2""

123
------WebKitFormBoundaryySzFkyO1blOdPmrr
Content-Disposition: form-data; name=""file1""; filename=""icon - freeVisa_03.png""
Content - Type: image / png

PNG
------WebKitFormBoundaryySzFkyO1blOdPmrr--
";

            var matches = RegItem.Matches(str);

            _HttpRequest request = new _HttpRequest();
            request.ParseFromByte(Encoding.UTF8.GetBytes(str));
        }

    }

    public class _HttpRequest
    {
        public Dictionary<string, string> Parameter = new Dictionary<string, string>();
        public MemoryStream Stream;
        public Dictionary<string, HttpFile> Files = new Dictionary<string, HttpFile>();

        public string this[string key]
        {
            get
            {
                if (Parameter.ContainsKey(key)) return Parameter[key];
                else return "";
            }
        }
        #region Reg
        public static Regex RegProtocol = new Regex("(GET|POST)|[^ ]+");
        public static Regex RegItem = new Regex("([^:]+)");
        public static Regex Regmultipart_form_data = new Regex(@"(multipart/form-data)|(boundary=)|(----WebKitFormBoundary\w+)");
        public static Regex RegContentDisposition = new Regex(@"name|filename|(""[^""]+"")");
        #endregion
        public void ParseFromByte(byte[] buffer)
        {
            Stream = new MemoryStream(buffer);
            HeaderProcess(ReadCol());
            string content;
            while ((content = ReadCol()) != null)
            {
                ItemProcess(content);
            }
            Multipart_Form_DataProcess();

        }

        private string ReadCol()
        {
            var p = Stream.Position;
            var count = 0;
            while (true)
            {
                if (Stream.ReadByte() == 0x0D && Stream.ReadByte() == 0x0A)
                {
                    break;
                }
                count++;
            }
            if (count == 0)
            {
                return null;//head 结束
            }
            Stream.Seek(p, SeekOrigin.Begin);
            count += 2;
            var content = new byte[count];
            Stream.Read(content, 0, count);
            var str = Encoding.UTF8.GetString(content);
            return str;
        }
        public byte[] ReadFile()
        {
            var p = Stream.Position;
            var count = 0;
            while (true)
            {
                if (Stream.ReadByte() == 0x0D && Stream.ReadByte() == 0x0A
                    //"------"
                    && Stream.ReadByte() == 0x2D && Stream.ReadByte() == 0x2D && Stream.ReadByte() == 0x2D && Stream.ReadByte() == 0x2D && Stream.ReadByte() == 0x2D && Stream.ReadByte() == 0x2D)
                {
                    break;
                }
                count++;
            }
            if (count == 0)
            {
                return null;//head 结束
            }
            Stream.Seek(p, SeekOrigin.Begin);
            var content = new byte[count];
            Stream.Read(content, 0, count);
            Stream.Seek(2, SeekOrigin.Current);
            return content;
        }

        private void HeaderProcess(string content)
        {
            var matches = RegProtocol.Matches(content);
            Parameter.Add("Method", matches[0].Value);
            Parameter.Add("Url", matches[1].Value);
            Parameter.Add("Protocol", matches[2].Value);
        }
        private void ItemProcess(string content)
        {
            var Match = RegItem.Match(content);
            Parameter.Add(Match.Value, content.Substring(Match.Value.Length + 2, content.Length - Match.Value.Length - 2));

        }
        public void Multipart_Form_DataProcess()
        {
            var ContentType = this["Content-Type"];
            var matches=Regmultipart_form_data.Matches(ContentType);
            if (!(matches.Count == 3 && matches[0].Value == "multipart/form-data"))
            {
                return;
            }
            var WebKitFormBoundary = "--"+matches[2].Value;
            var WebKitFormBoundaryEnd = WebKitFormBoundary+"--\r\n";
            string content;
            while (!string.IsNullOrEmpty(content = ReadCol())&& content!= WebKitFormBoundaryEnd)
            {
                var pType=ReadCol();
                var m2=RegContentDisposition.Matches(pType);
                //二进制文件
                if (m2.Count == 4 && m2[0].Value == "name" && m2[2].Value == "filename")
                {
                    ReadCol();
                    ReadCol();
                    Files.Add(m2[1].Value, new HttpFile() { File = ReadFile() });
                }
                else
                {
                    ReadCol();
                    ReadCol();
                }
            }

        }
    }

    public class HttpFile
    {
        public string FileType;
        public string FileName;
        public byte[] File;
    }

}