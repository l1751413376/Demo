using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace 梦幻西游wdf文件读取
{
   public class WdfClass
    {

       public static String[] FILETYPE = { "未知","was","mp3","jpg","tag","wav","bmp","lua"};


        string wdfpath;
        FileStream wdffile;

        public WDFHeader wdfhead;
        public FileList[] filelist;
        public FileExs[] fileexs;
        public int filesum;
       public WdfClass(string filepath)
       {
           wdfpath = filepath;
       
       }

       /// <summary>
       /// 读取wdf文件的基本信息
       /// </summary>
       public void ReadWdfFile()
       { 
       
           //ReadWdfHead   如果是wdf文件
           wdfhead = new WDFHeader();

           wdffile = File.OpenRead(wdfpath);
           byte[] buf = new byte[12];
           wdffile.Read(buf, 0, 12);

           wdfhead.Flag = BitConverter.ToUInt32(buf, 0);
           wdfhead.FileSum = BitConverter.ToUInt32(buf, 4);
           wdfhead.Offset = BitConverter.ToUInt32(buf,8);
           filesum = (int)wdfhead.FileSum;

          
       
          
           //读取每个文件的基本信息
           filelist = new FileList[filesum];
           byte []filebuf=new byte[4*4];
           wdffile.Seek(wdfhead.Offset, SeekOrigin.Begin);
           for (int j = 0; j < filesum; j++)
           {
               wdffile.Read(filebuf, 0, filebuf.Length);
               filelist[j].Uid = BitConverter.ToUInt32(filebuf, 0);
               filelist[j].FileOffset = BitConverter.ToUInt32(filebuf, 4);
               filelist[j].FileSize = BitConverter.ToUInt32(filebuf, 8);
               filelist[j].FileSpace = BitConverter.ToUInt32(filebuf, 12);
               filelist[j].FileType = 0;
           }

           //读取获取文件类型的信息
           fileexs = new FileExs[filesum];

           for (int i = 0; i < filesum; i++)
           {
               fileexs[i].Hdw = new byte[2];
               fileexs[i].Sst = new byte[4];
               fileexs[i].Nst = new byte[4];
               fileexs[i].Dss = new byte[4];
               fileexs[i].Sss = new byte[2];
               fileexs[i].Dsg = new byte[3];

               wdffile.Seek(filelist[i].FileOffset, SeekOrigin.Begin);
               wdffile.Read(fileexs[i].Hdw, 0, fileexs[i].Hdw.Length);//1-2
               wdffile.Seek(4, SeekOrigin.Current);//6
               wdffile.Read(fileexs[i].Sst, 0, fileexs[i].Sst.Length);//7-10
               wdffile.Seek(-2, SeekOrigin.Current);                  //8
               wdffile.Read(fileexs[i].Nst, 0, fileexs[i].Nst.Length);//9-12
               wdffile.Seek(filelist[i].FileOffset+filelist[i].FileSize-6, SeekOrigin.Begin);//最后7-10位
               wdffile.Read(fileexs[i].Dss, 0, fileexs[i].Dss.Length);
               wdffile.Seek(filelist[i].FileOffset + filelist[i].FileSize - 3, SeekOrigin.Begin);//最后4-5位
               wdffile.Read(fileexs[i].Dsg, 0, fileexs[i].Dsg.Length);
               wdffile.Seek(filelist[i].FileOffset  +4, SeekOrigin.Begin);//5-7
               wdffile.Read(fileexs[i].Sss, 0, fileexs[i].Sss.Length);

              filelist[i].FileType = GetFileType(fileexs[i]);
           }

          
           
       }

       public int GetFileType(FileExs esdate)
       {

        if (BitConverter.ToInt16(esdate.Hdw, 0) == 0x5053) return 1;                             //was/tcp文件  "PS"
        else if (BitConverter.ToInt16(esdate.Hdw, 0) ==0x4D42) return 6;						   //bmp文件  BM   42 4D
        else if (BitConverter.ToInt32(esdate.Sst,0) == 0x49464a10) return 3;                      //jpg文件  "FIFJ"
        else if (BitConverter.ToInt32(esdate.Dss,0) == 0x454c4946) return 4;                      //tga文件  "ELIF"
        else if (BitConverter.ToInt16(esdate.Hdw, 0) == 0x4952 && BitConverter.ToInt32(esdate.Nst,0) == 0x45564157) return 5;  //wav文件RIFF | WAVE  52 49 46 46  | 57 41 56 45
        else if ((esdate.Dsg[0] == 0x11) && (esdate.Dsg[1] == 0) && (esdate.Dsg[2] == 0) && ((BitConverter.ToInt16(esdate.Sss, 0) == 0x1000) || (BitConverter.ToInt16(esdate.Sss,0) == 0x0f00))) return 7;
        else if (BitConverter.ToInt16(esdate.Hdw, 0) == 0x00ff) return 2;			   //mp3文件
        else return 0;   
       }


        /// <summary>
        /// 读取具体文件的信息
        /// </summary>
        /// <param name="fileuid">文件的uid</param>
        public void ReadFileData(UInt32 fileuid)
        {
            for(int i=0;i<filesum;i++)
            {
              if(filelist[i].Uid==fileuid)
              {
                wdffile.Seek(filelist[i].FileOffset, SeekOrigin.Begin);
                filelist[i].FileData=new byte[filelist[i].FileSize];
                wdffile.Read(filelist[i].FileData,0,filelist[i].FileData.Length);
                break;
              }
            }
        }

       /// <summary>
       /// 释放资源
       /// </summary>
        public void Close()
        {
         wdffile.Close();
       
        }

       }













       //文件头定义
       /// <summary>
       /// 打开某WDF文件从0偏移读取该文件头， 50 46 44 57
       /// 对 FileHeader.id 判断是否是：‘WDFP’， 正确的WDF文件头四个字节一定是这个。
       /// 然后跳到 offset 所指的文件列表地址处准备读取文件列表信息。
       /// </summary>
       public struct WDFHeader
        {
            public  UInt32 Flag;                   // 文件标识符
            public  UInt32 FileSum;                // 文件数量
            public  UInt32 Offset;                 // 文件列表地址
        }

        //文件列表
      public  struct FileList
        {
            public UInt32 Uid;           //文件内部ID
            public Int32 FileType;      //文件类型
            public UInt32 FileSize;       //文件大小
            public UInt32 FileOffset;    //偏移量
            public UInt32 FileSpace;     //文件剩余空间
            public Byte[] FileData;
        }

      //文件类型定义
    //1 was/tcp/wap/tca    头两个字节        SP    53 50
    //2 mp3                头两个字节              FF / FF
    //3 jpg                第七个字节开始    JFIF  10 4A 46 49
    //4 TGA                倒数六字节        FILE  46 49 4C 45
    //5 WAV                头四个字节 | 第九个字节开始    RIFF | WAVE  52 49 46 46  | 57 41 56 45
    //6 BMP                头两个字节        BM   42 4D
    //7 lua脚本	     第三字节开始  00 00 00 10 或 00 00 00 0F   倒数三个字节 11 00 00 
    //0 未知
      public struct FileExs
      { 
        public Byte []Hdw;  //头两个字节
        public Byte []Sst;  //第七个字节开始的四个字节
        public Byte []Nst;  //第九个字节开始的四个字节
        public Byte []Dss;  //倒数六个字节开始的四个字节
        public Byte []Sss;	//第五个字节开始的二个字节
        public Byte []Dsg;//倒数三个字节
      
      }



    
}
