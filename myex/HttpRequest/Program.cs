using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Text.RegularExpressions;
namespace HttpRequest
{
    class Program
    {
        public static string ASPXAUTH()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(
@"POST http://10.14.40.49/InterVacation/Home/Login HTTP/1.1
Host: 10.14.40.49
Connection: keep-alive
Content-Length: 47
Pragma: no-cache
Cache-Control: no-cache
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Origin: http://10.14.40.49
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.122 Safari/537.36 SE 2.X MetaSr 1.0
Content-Type: application/x-www-form-urlencoded
Referer: http://10.14.40.49/InterVacation/Home/Login
Accept-Encoding: gzip,deflate
Accept-Language: zh-CN,zh;q=0.8
Cookie: 

LoginName=09614&password=85237952m%2C&subPhone=");
            sb.AppendLine("\r\n");
            //这个一定要有不然接收回来可能没有数据
            return sb.ToString();
        }

        public static string LineOperatorAndSession()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(
@"GET http://10.14.40.49/intervacation/ HTTP/1.1
Host: 10.14.40.49
Connection: keep-alive
Cache-Control: max-age=0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.110 Safari/537.36
Accept-Encoding: deflate, sdch
Accept-Language: zh-CN,zh;q=0.8");
            sb.AppendLine("");
            return sb.ToString();
        }

        public static string OrderQuery() 
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(
@"GET http://10.14.40.49/InterVacation/visaorder/OrderProcessList/OrderQuery?dd=0.13490661784228353&Shortcut=&Transactor=&ThirdSerialid=&SelfSupport=&VisaLineId=&BookingDateBegin=&BookingDateEnd=&PaymentDateBegin=&PaymentDateEnd=&SendSignDateBegin=&SendSignDateEnd=&VisaDateBegin=&VisaDateEnd=&ContactPerson=&ContactMoblie=&ORJobNo=&OMJobNo=&Interview=&EVisa=-1&SupplierId=&ShowTestOrder=0&OrderField=&PageSize=15&PageIndex=1 HTTP/1.1
Host: 10.14.40.49
Connection: keep-alive
Accept: text/html, */*; q=0.01
X-Requested-With: XMLHttpRequest
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.110 Safari/537.36
Accept-Encoding: deflate, sdch
Accept-Language: zh-CN,zh;q=0.8");
            sb.AppendLine("");
            return sb.ToString();
        }
        static void Main(string[] args)
        {
            var getAuthCookies = SocketSendReceive(ASPXAUTH(), "10.14.40.49", 80);
            var request = LineOperatorAndSession();
            AddCookie(ref request, getAuthCookies);
            var logginInfo = SocketSendReceive(request, "10.14.40.49", 80);
            List<Cookie> logininfocookie = new List<Cookie>();
            logininfocookie.AddRange(getAuthCookies);
            logininfocookie.AddRange(logginInfo);
            //获取登录信息
            var OrderQueryReq = OrderQuery();
            AddCookie(ref OrderQueryReq, logininfocookie);
            SocketSendReceive(OrderQueryReq, "10.14.40.49", 80);

        }


        private static Socket ConnectSocket(string server, int port)
        {
            Socket s = null;
            IPHostEntry hostEntry = null;
            hostEntry = Dns.GetHostEntry(server);
            foreach (IPAddress address in hostEntry.AddressList)
            {
                IPEndPoint ipe = new IPEndPoint(address, port);
                Socket tempSocket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
                tempSocket.Connect(ipe);
                if (tempSocket.Connected)
                {
                    s = tempSocket;
                    break;
                }
                else
                {
                    continue;
                }
            }
            return s;
        }
        private static Socket ConnectSocketIP(string server, int port)
        {
            Socket s = null;
            IPEndPoint ipe = new IPEndPoint(IPAddress.Parse(server), port);
            Socket tempSocket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            tempSocket.Connect(ipe);
            if (tempSocket.Connected)
            {
                s = tempSocket;
            }
            return s;
        }
        public static List<Cookie> SocketSendReceive(string request, string server, int port)
        {

            Byte[] bytesSent = Encoding.ASCII.GetBytes(request);
            Byte[] bytesReceived = new Byte[655350];
            Socket s = ConnectSocketIP(server, port);
            s.Send(bytesSent, bytesSent.Length, 0);
            int bytes = 0;
            string page = "Default HTML page on " + server + ":\r\n";

            bytes = s.Receive(bytesReceived, bytesReceived.Length, 0);
            page = page + Encoding.UTF8.GetString(bytesReceived, 0, bytes);
            var contentlength = GetParam(page, "Content-Length");
            var body = GetBody(page);
            var r = body.Length == Convert.ToInt32(contentlength);
            var ret = Cookie.GetSetCookie(page);
            return ret;

        }
        public static string GetParam(string data, string name)
        {
            var reg = new Regex(@"[\S]+");
            var i = data.IndexOf(name) + name.Length + 1;
            var ret = reg.Match(data, i);
            return ret.Value;
        }
        public static string GetBody(string data)
        {
            var begin = data.IndexOf("<html>");
            var body = data.Substring(begin, data.Length - begin);
            return body;
        }
        public static void AddCookie(ref string data, List<Cookie> cookies)
        {

            data += "Cookie:";
            foreach (var item in cookies)
            {
                data += string.Format(" {0}={1};", item.Key, item.Value);
            }
            data += "\n\r\n";
        }
    }
    public class Cookie
    {
        public string Key;
        public string Value;
        public static List<Cookie> GetSetCookie(string data)
        {
            var reg = new Regex(@"Set-Cookie:( \S+)*");
            var result = reg.Matches(data);
            List<Cookie> list = new List<Cookie>(); ;
            for (int i = 0; i < result.Count; i++)
            {
                var item = result[i];
                var value = item.Value;
                value = value.Replace("Set-Cookie: ", "");
                var begin = value.IndexOf("=");
                var end = value.IndexOf(";");
                if (begin > -1 && end > -1)
                {
                    Cookie c = new Cookie()
                    {
                        Key = value.Substring(0, begin),
                        Value = value.Substring(begin + 1, end - begin - 1),
                    };
                    list.Add(c);
                }
            }
            return list;

        }
    }
}

