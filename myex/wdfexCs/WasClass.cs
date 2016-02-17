using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace 梦幻西游wdf文件读取
{
   public class WasClass
    {

       public WasFileHead wasfiledhead;
       public Picture[] picture;
       public int directionsum;
       public int hz;



       FileStream fs;
       long fsoffset;

       /// <summary>
       /// 初始化
       /// </summary>
       /// <param name="FS">文件</param>
       /// <param name="Offset">was数据开始位置</param>
       public WasClass(FileStream FS,long Offset)
       {
           fs = FS;
           fsoffset = Offset;
       }


       public void ReadData()
       {

           wasfiledhead = new WasFileHead();
          
           byte []buf=new byte[2*8];
           fs.Seek(fsoffset, SeekOrigin.Begin);
           fs.Read(buf, 0, buf.Length);
           wasfiledhead.Flag = BitConverter.ToUInt16(buf, 0);
           wasfiledhead.HeadSize = BitConverter.ToUInt16(buf, 2);
           wasfiledhead.DirectionNum = BitConverter.ToUInt16(buf, 4);
           wasfiledhead.SpriteHz = BitConverter.ToUInt16(buf, 6);
           wasfiledhead.SpriteWidth = BitConverter.ToUInt16(buf, 8);
           wasfiledhead.SpriteHeight = BitConverter.ToUInt16(buf, 10);
           wasfiledhead.SpriteCenterX= BitConverter.ToUInt16(buf, 12);
           wasfiledhead.SpriteCenterY = BitConverter.ToUInt16(buf, 14);

           directionsum=(int)wasfiledhead.DirectionNum;
           hz=(int) wasfiledhead.SpriteHz;

           //取调色板信息
           wasfiledhead.ColorBoard = new byte[2 * 256];
           fs.Seek(fsoffset + wasfiledhead.HeadSize + 4, SeekOrigin.Begin);
           fs.Read(wasfiledhead.ColorBoard, 0, wasfiledhead.ColorBoard.Length);

           // 创建帧偏移表数组
           wasfiledhead.PictureOffseList = new UInt32[directionsum * hz];
           // 获取帧偏移表
           byte []buflist=new byte[4];
           for (int i = 0; i < directionsum * hz; i++)
           {
               fs.Read(buflist, 0, 4);
               wasfiledhead.PictureOffseList[i] = BitConverter.ToUInt32(buflist, 0);
           }

           

           // 获取图片数据指针的指针
           // 获取alpha数据指针的指针

           // 读取帧数据
           picture = new Picture[directionsum * hz];
           for (int j = 0; j < directionsum * hz; j++)
           { 
             // 获取帧数据头
               fs.Seek(wasfiledhead.PictureOffseList[j] + fsoffset+4+wasfiledhead.HeadSize, SeekOrigin.Begin);
               byte []pic = new byte[4 * 4];
               fs.Read(pic, 0, pic.Length);

               picture[j].PictureX = BitConverter.ToInt32(pic, 0);
               picture[j].PicyureY=BitConverter.ToInt32(pic,4);
               picture[j].Width=BitConverter.ToInt32(pic,8);
               picture[j].Height = BitConverter.ToInt32(pic, 12);
           }

       }




       /// <summary>
       /// was文件头
       /// </summary>
       public struct WasFileHead
       {
           public UInt16 Flag;              //文件标示 SP
           public UInt16 HeadSize;          //文件头大小
           public UInt16 DirectionNum;      //动画的方向数
           public UInt16 SpriteHz;          //每个方向的帧数
           public UInt16 SpriteWidth;       //动画的宽度
           public UInt16 SpriteHeight;      //动画的高度
           public UInt16 SpriteCenterX;     //动画的中心 X
           public UInt16 SpriteCenterY;     //动画的中心 Y
           public UInt32[] PictureOffseList; //帧偏移表数组
           public byte[] ColorBoard;    //调色板  每个2个字节
       }

       /// <summary>
       /// 数据
       /// </summary>
       public struct Picture
       {
           public int PictureX;  //图片中心 X
           public int PicyureY; //图片中心 Y
           public int Width;    //图片宽度
           public int Height;   //图片高度
           public byte[] PictureData;
          
        }

    }
}
