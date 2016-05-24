using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace WebService
{
    public class _HttpRequest
    {
        public Dictionary<string, string> Parameter = new Dictionary<string, string>();
        public MemoryStream Stream;
        public Dictionary<string, HttpFile> Files = new Dictionary<string, HttpFile>();
        public List<Byte[]> Buffer = new List<byte[]>();
        public int BufferCount = 0;
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
        public void ParseFromByte(byte[] buffer,int rLength,Socket socket)
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
        public void ParseFromSocket(Socket s)
        {
            Byte[] contentbuff = new Byte[65535];
            var rLength = s.Receive(contentbuff, contentbuff.Length, 0);
            Stream = new MemoryStream(contentbuff);
            HeaderProcess(ReadCol());
            string content;
            while ((content = ReadCol()) != null)
            {
                ItemProcess(content);
            }

            var allLength = Convert.ToInt32(Parameter["Content-Length"]);
            var current= (int)Stream.Position;
            rLength -= current;
            var buff = new Byte[rLength];
            for (int i = 0; i < rLength; i++)
            {
                buff[i] = contentbuff[i+current];
            }
            Buffer.Add(buff);
            while ((allLength -= rLength) > 0)
            {
                rLength = s.Receive(contentbuff, contentbuff.Length, 0);
                buff = new Byte[rLength];
                for (int i = 0; i < rLength; i++)
                {
                    buff[i] = contentbuff[i];
                }
                Buffer.Add(buff);
            }
            Multipart_Form_DataProcess();

        }
        private string ReadCol()
        {
            var p = Stream.Position;
            var count = 0;
            while (Stream.Position < Stream.Length)
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
            long count = 0;
            while (true)
            {
                if (Stream.Position >= Stream.Length)
                {
                    BufferCount++;
                    if (BufferCount >= Buffer.Count)
                    {
                        break;
                    }
                    Stream = new MemoryStream(Buffer[BufferCount]);
                }
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
            count += 8;
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
            var matches = Regmultipart_form_data.Matches(ContentType);
            if (!(matches.Count == 3 && matches[0].Value == "multipart/form-data"))
            {
                return;
            }
            var WebKitFormBoundary = "--" + matches[2].Value;
            var WebKitFormBoundaryEnd = WebKitFormBoundary + "--\r\n";
            string content;
            Stream = new MemoryStream(Buffer[0]);
            while (!string.IsNullOrEmpty(content = ReadCol()) && content != WebKitFormBoundaryEnd)
            {
                var pType = ReadCol();
                var m2 = RegContentDisposition.Matches(pType);
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
