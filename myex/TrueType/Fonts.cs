using System;
using System.IO;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using TOpenGL;

namespace TrueType
{
    public class Fonts
    {
        static NURBSCurve nub;
        /// <summary> 表头
        /// /*此结构体为TrueType字体中的表的定义形式*/ 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class TableEntry
        {
            public char[] tag;          /*资源标记*/
            public uint checksum;       /*校验位*/
            public uint offset;         /*表在TrueType结构体中的偏移量*/
            public uint length;		    /*每个表的大小*/ 
            public TableEntry()
            {
                tag = new char[4];
                checksum = 0;
                offset = 0;
                length = 0;
            }
        };

        /// <summary> 表字典
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class TableDirctory
        {
            public uint sfntversion;     /*目前所用版本皆为1.0版本，故此值为0x10000*/
            public ushort numTables;    /*该字体中表的数目*/
            public ushort searchRange;
            public ushort entrySelector;
            public ushort rangeShift;
            public TableEntry[] entries;       /*保存着具体的表,i = numTables*/
        };

        /// <summary> 1.head表 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_head
        {
            public uint Table;                  /*版本号，一般为0x00010000*/
            public uint fontRevision;           /*由字体制造商设置*/
            public uint cheskSumAdjestment;     /*校验调整项*/
            public uint magicNumer;             /*一般设为0x5f0f3cf5*/
            public ushort flags;                  /*标志位*/
            public ushort unitsPerEm;             /*EM_square的大小，从16到16384*/
            public long created;                /*国际数据（8字节范围）*/
            public long modified;               /*国际数据（8字节范围）*/
            public short xMin;                   /*图元范围 */
            public short yMin;
            public short xMax;
            public short yMax;
            public ushort macStyle;
            public ushort lowestRecPPEM;          /*最小可读像素大小*/
            public short fontDirctionHint;       /*字体方向*/
            public short indexToLocFormat;       /*在位置表中图元索引的格式*/
            /*1为long，0为short*/
            public short glyphDataFormat;        /*图元数据格式，设为0*/
        }

        static Table_head ReadHeadTable(byte[] dats)
        {
            Table_head thead = new Table_head();
            thead.Table = (uint)((uint)dats[0] * (uint)0x1000000 + dats[1] * 0x10000 + dats[2] * 0x100 + dats[3]);
            thead.fontRevision = (uint)((uint)dats[4] * (uint)0x1000000 + dats[5] * 0x10000 + dats[6] * 0x100 + dats[7]);
            thead.cheskSumAdjestment = (uint)((uint)dats[8] * (uint)0x1000000 + dats[9] * 0x10000 + dats[10] * 0x100 + dats[11]);
            thead.magicNumer = (uint)((uint)dats[12] * (uint)0x1000000 + dats[13] * 0x10000 + dats[14] * 0x100 + dats[15]);
            thead.flags = (ushort)(dats[16] * 0x100 + dats[17]);
            thead.unitsPerEm = (ushort)(dats[18] * 0x100 + dats[19]);
            thead.created = (long)((long)dats[20] * 0x100000000000000 + (long)dats[21] * 0x100000000000 + (long)dats[22] * 0x1000000000 + (long)dats[23] * 0x10000000 + (long)dats[24] * 0x100000 + (long)dats[25] * 0x1000 + (long)dats[26] * 0x100 + (long)dats[27]);
            thead.modified = (long)((long)dats[28] * 0x100000000000000 + (long)dats[29] * 0x100000000000 + (long)dats[30] * 0x1000000000 + (long)dats[31] * 0x10000000 + (long)dats[32] * 0x100000 + (long)dats[33] * 0x1000 + (long)dats[34] * 0x100 + (long)dats[35]);
            thead.xMin = (short)(dats[36] * 0x100 + dats[37]);
            thead.yMin = (short)(dats[38] * 0x100 + dats[39]);
            thead.xMax = (short)(dats[40] * 0x100 + dats[41]);
            thead.yMax = (short)(dats[42] * 0x100 + dats[43]);
            thead.macStyle = (ushort)(dats[44] * 0x100 + dats[45]);
            thead.lowestRecPPEM = (ushort)(dats[46] * 0x100 + dats[47]);
            thead.fontDirctionHint = (short)(dats[48] * 0x100 + dats[49]);
            thead.indexToLocFormat = (short)(dats[50] * 0x100 + dats[51]);
            thead.glyphDataFormat = (short)(dats[52] * 0x100 + dats[53]);
            return thead;
        }

        /// <summary> 2.cmap表 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_cmap
        {
            public ushort version;
            public ushort numTables;
            public Table_cmap_name[] Tables;
        }

        /// <summary> 2.cmap表 name
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_cmap_name
        {
            public ushort platformID;
            public ushort encodingID;
            public uint offset;

            //格式代码 由OFFSET指向的数据
            public ushort format;
            public Table_cmap_smtd smtdTable;
            public Table_cmap_segcov segcovTable;
        }

        /// <summary> 2.cmap表 smtd
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_cmap_smtd
        {
            public ushort format; //Format number is set to 4. 
            public ushort length; //This is the length in bytes of the subtable. 
            public ushort language; //Please see “Note on the language field in 'cmap' subtables“ in this document. 
            public ushort segCount2; //2 x segCount. 
            public ushort searchRange; //2 x (2**floor(log2(segCount))) 
            public ushort entrySelector; //log2(searchRange/2) 
            public ushort rangeShift; //2 x segCount - searchRange 
            public ushort[] endCount; //End characterCode for each segment, last=0xFFFF. 
            public ushort reservedPad; //Set to 0. 
            public ushort[] startCount; //Start character code for each segment. 
            public short[] idDelta; //Delta for all character codes in segment. 
            public ushort[] idRangeOffset; //Offsets into glyphIdArray or 0 
            public ushort[] glyphIdArray; //Glyph index array (arbitrary length) 
        }

        /// <summary> 2.cmap表 segcov
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_cmap_segcov
        {
            public ushort format;// Subtable format; set to 12. 
            public ushort reserved;// Reserved; set to 0 
            public uint length;// Byte length of this subtable (including the header) 
            public uint language;// Please see “Note on the language field in 'cmap' subtables“ in this document. 
            public uint nGroups;// Number of groupings which follow 
            public Table_cmap_segcov_nGroup[] groups;
        }

        /// <summary> 2.cmap表 segcov_nGroup
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_cmap_segcov_nGroup
        {
            public uint startCharCode;// First character code in this group 
            public uint endCharCode;// Last character code in this group 
            public uint startGlyphID;// Glyph index corresponding to the starting character code
        }

        static Table_cmap ReadcmapTable(byte[] dats)
        {
            Table_cmap tcmap = new Table_cmap();
            tcmap.version = (ushort)(dats[0] * 0x100 + dats[1]);
            tcmap.numTables = (ushort)(dats[2] * 0x100 + dats[3]);
            tcmap.Tables = new Table_cmap_name[tcmap.numTables];
            int i = 0, bases = 0;
            for (i = 0; i < tcmap.numTables; i++)
            {
                bases = i * 8 + 4;
                tcmap.Tables[i] = new Table_cmap_name();
                tcmap.Tables[i].platformID = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                tcmap.Tables[i].encodingID = (ushort)(dats[bases + 2] * 0x100 + dats[bases + 3]);
                tcmap.Tables[i].offset = (uint)((uint)dats[bases + 4] * (uint)0x1000000 + dats[bases + 5] * 0x10000 + dats[bases + 6] * 0x100 + dats[bases + 7]);
                tcmap.Tables[i].format = (ushort)(dats[tcmap.Tables[i].offset] * 0x100 + dats[tcmap.Tables[i].offset + 1]);
            }
            foreach (Table_cmap_name tn in tcmap.Tables)
            {
                if (tn.format == 4)     //smtd表
                {
                    tn.smtdTable = new Table_cmap_smtd();
                    tn.smtdTable.format = tn.format;
                    tn.smtdTable.length = (ushort)(dats[tn.offset + 2] * 0x100 + dats[tn.offset + 3]);
                    tn.smtdTable.language = (ushort)(dats[tn.offset + 4] * 0x100 + dats[tn.offset + 5]);
                    tn.smtdTable.segCount2 = (ushort)(dats[tn.offset + 6] * 0x100 + dats[tn.offset + 7]);
                    tn.smtdTable.searchRange = (ushort)(dats[tn.offset + 8] * 0x100 + dats[tn.offset + 9]);
                    tn.smtdTable.entrySelector = (ushort)(dats[tn.offset + 10] * 0x100 + dats[tn.offset + 11]);
                    tn.smtdTable.rangeShift = (ushort)(dats[tn.offset + 12] * 0x100 + dats[tn.offset + 13]);
                    int segCount = tn.smtdTable.segCount2 / 2;
                    tn.smtdTable.endCount = new ushort[segCount];
                    bases = (int)tn.offset + 14;
                    for (i = 0; i < segCount; i++)
                    {
                        tn.smtdTable.endCount[i] = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                        bases += 2;
                    }
                    tn.smtdTable.reservedPad = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                    bases += 2;
                    tn.smtdTable.startCount = new ushort[segCount];
                    for (i = 0; i < segCount; i++)
                    {
                        tn.smtdTable.startCount[i] = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                        bases += 2;
                    }
                    tn.smtdTable.idDelta = new short[segCount];
                    for (i = 0; i < segCount; i++)
                    {
                        tn.smtdTable.idDelta[i] = (short)(dats[bases] * 0x100 + dats[bases + 1]);
                        bases += 2;
                    }
                    tn.smtdTable.idRangeOffset = new ushort[segCount];
                    for (i = 0; i < segCount; i++)
                    {
                        tn.smtdTable.idRangeOffset[i] = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                        bases += 2;
                    }
                    int glycount = (int)(tn.smtdTable.length + tn.offset - bases);
                    glycount = glycount / 2;
                    tn.smtdTable.glyphIdArray = new ushort[glycount];
                    for (i = 0; i < glycount; i++)
                    {
                        tn.smtdTable.glyphIdArray[i] = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                        bases += 2;
                    }
                }
                else if (tn.format == 12)
                {
                    tn.segcovTable = new Table_cmap_segcov();
                    tn.segcovTable.format = 12;
                    tn.segcovTable.reserved = (ushort)(dats[tn.offset + 2] * 0x100 + dats[tn.offset + 3]);
                    tn.segcovTable.length = (uint)((uint)dats[tn.offset + 4] * (uint)0x1000000 + dats[tn.offset + 5] * 0x10000 + dats[tn.offset + 6] * 0x100 + dats[tn.offset + 7]);
                    tn.segcovTable.language = (uint)((uint)dats[tn.offset + 8] * (uint)0x1000000 + dats[tn.offset + 9] * 0x10000 + dats[tn.offset + 10] * 0x100 + dats[tn.offset + 11]);
                    tn.segcovTable.nGroups = (uint)((uint)dats[tn.offset + 12] * (uint)0x1000000 + dats[tn.offset + 13] * 0x10000 + dats[tn.offset + 14] * 0x100 + dats[tn.offset + 15]);
                    tn.segcovTable.groups = new Table_cmap_segcov_nGroup[tn.segcovTable.nGroups];
                    bases = (int)tn.offset + 16;
                    for (i = 0; i < tn.segcovTable.nGroups; i++)
                    {
                        tn.segcovTable.groups[i] = new Table_cmap_segcov_nGroup();
                        tn.segcovTable.groups[i].startCharCode = (uint)((uint)dats[tn.offset] * (uint)0x1000000 + dats[tn.offset + 1] * 0x10000 + dats[tn.offset + 2] * 0x100 + dats[tn.offset + 3]);
                        tn.segcovTable.groups[i].endCharCode = (uint)((uint)dats[tn.offset + 4] * (uint)0x1000000 + dats[tn.offset + 5] * 0x10000 + dats[tn.offset + 6] * 0x100 + dats[tn.offset + 7]);
                        tn.segcovTable.groups[i].startGlyphID = (uint)((uint)dats[tn.offset + 8] * (uint)0x1000000 + dats[tn.offset + 9] * 0x10000 + dats[tn.offset + 10] * 0x100 + dats[tn.offset + 11]);
                        bases += 12;
                    }
                }
            }
            return tcmap;
        }

        /// <summary> 3.glyf表 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class Table_glyf
        {
            public TableEntry entry;
            public byte[] dats;
        }

        static Table_glyf ReadGlyfTable(TableEntry te, byte[] dats)
        {
            Table_glyf tglyf = new Table_glyf();
            tglyf.entry = te;
            tglyf.dats = dats;
            return tglyf;
        }

        /// <summary> 4.loca表 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public class Table_loca_long
        {
            public uint[] offsets; //The actual local offset is stored. The value of n is numGlyphs + 1. The value for numGlyphs is found in the 'maxp' table.
        }

        public static Table_loca_long ReadLocalongTable(byte[] dats)
        {
            Table_loca_long tloca = new Table_loca_long();
            int length = dats.Length / 4;
            int bases = 0;
            tloca.offsets = new uint[length];
            for (int i = 0; i < length; i++)
            {
                tloca.offsets[i] = (uint)((uint)dats[bases + 0] * (uint)0x1000000 + dats[bases + 1] * 0x10000 + dats[bases + 2] * 0x100 + dats[bases + 3]);
                bases += 4;
            }
            return tloca;
        }

        static Table_head MainHead;
        static Table_cmap MainCmap;
        static Table_loca_long MainLocal;
        static Table_glyf MainGlyf;

        public Fonts(string Paths)
        {
            ReadFonts(Paths);
        }

        void ReadFonts(string Paths)
        {
            FileStream fs = new FileStream(Paths, FileMode.Open);
            byte[] dats = new byte[12];
            fs.Read(dats, 0, 12);
            TableDirctory td = new TableDirctory();
            td.sfntversion = (uint)((uint)dats[0] * (uint)0x1000000 + dats[1] * 0x10000 + dats[2] * 0x100 + dats[3]);
            td.numTables = (ushort)(dats[4] * 0x100 + dats[5]);
            td.searchRange = (ushort)(dats[6] * 0x100 + dats[7]);
            td.entrySelector = (ushort)(dats[8] * 0x100 + dats[9]);
            td.rangeShift = (ushort)(dats[10] * 0x100 + dats[11]);
            td.entries = new TableEntry[td.numTables];
            dats = new byte[td.numTables * 16];
            fs.Read(dats, 0, td.numTables * 16);
            int i = 0, bases = 0;
            for (i = 0; i < td.numTables; i++)
            {
                bases = i * 16;
                td.entries[i] = new TableEntry();
                td.entries[i].tag[0] = (char)dats[bases];
                td.entries[i].tag[1] = (char)dats[bases + 1];
                td.entries[i].tag[2] = (char)dats[bases + 2];
                td.entries[i].tag[3] = (char)dats[bases + 3];
                td.entries[i].checksum = (uint)((uint)dats[bases + 4] * (uint)0x1000000 + dats[bases + 5] * 0x10000 + dats[bases + 6] * 0x100 + dats[bases + 7]);
                td.entries[i].offset = (uint)((uint)dats[bases + 8] * (uint)0x1000000 + dats[bases + 9] * 0x10000 + dats[bases + 10] * 0x100 + dats[bases + 11]);
                td.entries[i].length = (uint)((uint)dats[bases + 12] * (uint)0x1000000 + dats[bases + 13] * 0x10000 + dats[bases + 14] * 0x100 + dats[bases + 15]);
            }
            //读取head表
            foreach (TableEntry te in td.entries)
            {
                if ((te.tag[0] == 'h') && (te.tag[1] == 'e') && (te.tag[2] == 'a') && (te.tag[3] == 'd'))
                {
                    dats = new byte[te.length];
                    fs.Seek(te.offset, SeekOrigin.Begin);
                    fs.Read(dats, 0, (int)te.length);
                    MainHead = ReadHeadTable(dats);
                }
            }
            //读取cmap表
            foreach (TableEntry te in td.entries)
            {
                if ((te.tag[0] == 'c') && (te.tag[1] == 'm') && (te.tag[2] == 'a') && (te.tag[3] == 'p'))
                {
                    dats = new byte[te.length];
                    fs.Seek(te.offset, SeekOrigin.Begin);
                    fs.Read(dats, 0, (int)te.length);
                    MainCmap = ReadcmapTable(dats);
                }
            }
            //读取loca表
            foreach (TableEntry te in td.entries)
            {
                if ((te.tag[0] == 'l') && (te.tag[1] == 'o') && (te.tag[2] == 'c') && (te.tag[3] == 'a'))
                {
                    dats = new byte[te.length];
                    fs.Seek(te.offset, SeekOrigin.Begin);
                    fs.Read(dats, 0, (int)te.length);
                    if (MainHead.indexToLocFormat == 1)
                    {
                        MainLocal = ReadLocalongTable(dats);
                    }
                }
            }
            //读取glyf表
            foreach (TableEntry te in td.entries)
            {
                if ((te.tag[0] == 'g') && (te.tag[1] == 'l') && (te.tag[2] == 'y') && (te.tag[3] == 'f'))
                {
                    dats = new byte[te.length];
                    fs.Seek(te.offset, SeekOrigin.Begin);
                    fs.Read(dats, 0, (int)te.length);
                    MainGlyf = ReadGlyfTable(te, dats);
                }
            }

            GetUnicodeChar("正");
        }

        static SimpleGly sgly;

        public void GetUnicodeChar(string Texts)
        {
            byte[] dats = ASCIIEncoding.Unicode.GetBytes(Texts);
            ushort unChar = (ushort)(dats[1] * 256 + dats[0]);
            int i = 0;
            int index = 0;
            int UCS_2 = 0;
            for (i = 0; i < MainCmap.Tables.Length; i++)
            {
                if ((MainCmap.Tables[i].platformID == 3) && (MainCmap.Tables[i].encodingID == 1))
                {
                    UCS_2 = i;
                }
            }
            for (i = 0; i < MainCmap.Tables[UCS_2].smtdTable.startCount.Length; i++)
            {
                if ((unChar >= MainCmap.Tables[UCS_2].smtdTable.startCount[i]) && (unChar <= MainCmap.Tables[UCS_2].smtdTable.endCount[i]))
                {
                    if (unChar < MainCmap.Tables[UCS_2].smtdTable.startCount[i])
                    {
                        index = 0;
                    }
                    else if (MainCmap.Tables[UCS_2].smtdTable.idRangeOffset[i] == 0)
                    {
                        index = unChar;// -MainCmap.Tables[0].smtdTable.idDelta[i];
                    }
                    else
                    {
                        int x = MainCmap.Tables[UCS_2].smtdTable.idRangeOffset[i] / 2 + (unChar - MainCmap.Tables[UCS_2].smtdTable.startCount[i]) + i;
                        if (x < MainCmap.Tables[UCS_2].smtdTable.idRangeOffset.Length)
                        {
                            index = MainCmap.Tables[UCS_2].smtdTable.idRangeOffset[x];
                        }
                        else
                        {
                            index = MainCmap.Tables[UCS_2].smtdTable.glyphIdArray[x - MainCmap.Tables[UCS_2].smtdTable.idRangeOffset.Length];
                        }
                    }
                    //获得初始文字序号
                    if (index != 0)
                    {
                        index = index + MainCmap.Tables[UCS_2].smtdTable.idDelta[i];
                    }
                    break;
                }
                else
                {
                    continue;
                }
            }
            //得到loca中的序号
            uint glyindex = MainLocal.offsets[index];

            short numberOfCountours = (short)(MainGlyf.dats[glyindex + 0] * 0x100 + MainGlyf.dats[glyindex + 1]);

            if (numberOfCountours > 0)
            {
                //简单图元
                sgly = new SimpleGly(MainGlyf.dats, (int)glyindex);
            }
            else
            {
                //复合图元
                CompositeGly cGly = new CompositeGly(MainGlyf.dats, (int)glyindex);
            }
            return;
        }
        /// <summary> 4.图元表 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class TableGly
        {
            public short numberOfCountours;      //轮廓线数目 复合图元为负数
            public short xMin;
            public short yMin;
            public short xMax;
            public short yMax;
        }

        /// <summary> 5.简单图元 
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class SimpleGly
        {
            public ushort[] endPtsOfContours;           //图元数量
            public ushort instructionlength;            //图元指令长度
            public byte[] instruction;                  //instructionlength
            public byte[] flags;                        //variable size
            public short[] xCoordinates;                 //variable size
            public short[] yCoordinates;                 //variable size
            public TableGly gly;

            Tesselator tess = new Tesselator();

            public SimpleGly(byte[] dats, int index)
            {
                int bases = index;
                gly = new TableGly();
                gly.numberOfCountours = (short)(MainGlyf.dats[bases + 0] * 0x100 + MainGlyf.dats[bases + 1]);
                gly.xMin = (short)(MainGlyf.dats[bases + 2] * 0x100 + MainGlyf.dats[bases + 3]);
                gly.yMin = (short)(MainGlyf.dats[bases + 4] * 0x100 + MainGlyf.dats[bases + 5]);
                gly.xMax = (short)(MainGlyf.dats[bases + 6] * 0x100 + MainGlyf.dats[bases + 7]);
                gly.yMax = (short)(MainGlyf.dats[bases + 8] * 0x100 + MainGlyf.dats[bases + 9]);
                int i = 0;
                bases += 10;
                //简单图元
                endPtsOfContours = new ushort[gly.numberOfCountours];
                for (i = 0; i < gly.numberOfCountours; i++)
                {
                    endPtsOfContours[i] = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                    bases += 2;
                }
                instructionlength = (ushort)(dats[bases] * 0x100 + dats[bases + 1]);
                bases += 2;
                instruction = new byte[instructionlength];
                for (i = 0; i < instructionlength; i++)
                {
                    instruction[i] = dats[bases];
                    bases++;
                }
                ushort ptNumbers = (ushort)(endPtsOfContours[gly.numberOfCountours - 1] + 1);
                flags = new byte[ptNumbers];
                for (i = 0; i < ptNumbers; i++)
                {
                    if ((dats[bases] & 0x08) != 0)
                    {
                        //重复
                        int repeatNum = dats[bases + 1];
                        int j = 0;
                        for (; j < repeatNum; j++)
                        {
                            flags[i + j] = dats[bases];
                        }
                        bases += 2;
                        i += j;
                    }
                    else
                    {
                        flags[i] = dats[bases];
                        bases++;
                    }
                }

                xCoordinates = new short[ptNumbers];
                yCoordinates = new short[ptNumbers];
                short x = 0;
                short y = 0;
                short t = 0;
                byte[] tem = new byte[2];
                //解码X点
                for (i = 0; i < ptNumbers; i++)
                {
                    t = 0;
                    if ((flags[i] & 2) != 0)
                    {
                        t = dats[bases];
                        if ((flags[i] & 16) == 0)
                        {
                            t = (short)(-t);
                        }
                        bases++;
                    }
                    else if ((flags[i] & 16) == 0)
                    {
                        tem[0] = dats[bases + 1];
                        tem[1] = dats[bases];
                        t = BitConverter.ToInt16(tem, 0);
                        bases += 2;
                    }

                    x = (short)(x + t);
                    xCoordinates[i] = x;
                }
                //解码Y点
                for (i = 0; i < ptNumbers; i++)
                {
                    t = 0;
                    if ((flags[i] & 4) != 0)
                    {
                        t = dats[bases];
                        if ((flags[i] & 32) == 0)
                        {
                            t = (short)(-t);
                        }
                        bases++;
                    }
                    else if ((flags[i] & 32) == 0)
                    {
                        tem[0] = dats[bases + 1];
                        tem[1] = dats[bases];
                        t = BitConverter.ToInt16(tem, 0);
                        bases += 2;
                    }

                    y = (short)(y + t);
                    yCoordinates[i] = y;
                }
            }

            public int Draw(OpenGL gl, NURBSCurve nub, System.Drawing.Point position, int textheight)
            {
                int i = 0;
                int x = position.X;
                int y = position.Y;
                float scale = (gly.yMax - gly.yMin) / textheight;
                if (DrawPoints)
                {
                    gl.Color(1.0, 0.0, 0.0);
                    gl.Begin(OpenGL.GL_POINTS);
                    for (i = 0; i < xCoordinates.Length; i++)
                    {
                        gl.Vertex((double)xCoordinates[i] / scale + x, (double)yCoordinates[i] / scale + y);
                    }
                    gl.End();
                }

                Context ct = new Context(gl);

                double[] dats = new double[3];
                dats[2] = 0;
                tess.InitTess(gl);
                gl.TessBeginPolygon(tess.Tessobj, IntPtr.Zero);
                int j = 0;
                for (i = 0; i < 1; i++)
                {
                    gl.TessBeginContour(tess.Tessobj);
                    for (; j <= endPtsOfContours[i]; j++)
                    {
                        dats[0] = (double)xCoordinates[i] / scale + x;
                        dats[1] = (double)yCoordinates[i] / scale + y;
                        //gl.TessVertex(tess.Tessobj, dats, dats);
                    }
                    gl.TessEndContour(tess.Tessobj);
                }
                gl.TessEndPolygon(tess.Tessobj);

                float startX = xCoordinates[0] / scale + x;
                float startY = yCoordinates[0] / scale + y;

                float midX, midY;

                float[] Curve = new float[6];
                int nonLineNum = 0;

                i = 1;
                for (j = 0; j < endPtsOfContours.Length; j++)
                {
                    for (; i < endPtsOfContours[j] + 1; i++)
                    {
                        if ((flags[i] & 0x01) == 0)      //如果不在曲线上
                        {
                            nonLineNum++;   //记录不在曲线上的点的个数
                        }
                        else                                //在曲线上
                        {
                            if (nonLineNum == 0)        //直线
                            {
                                Curve = new float[6];
                                Curve[0] = xCoordinates[i - 1] / scale + x;
                                Curve[1] = yCoordinates[i - 1] / scale + y;
                                Curve[3] = xCoordinates[i] / scale + x;
                                Curve[4] = yCoordinates[i] / scale + y;
                            }
                            else if (nonLineNum == 1)   //3点B样条
                            {
                                Curve = new float[9];
                                Curve[0] = xCoordinates[i - 2] / scale + x;
                                Curve[1] = yCoordinates[i - 2] / scale + y;
                                Curve[3] = xCoordinates[i - 1] / scale + x;
                                Curve[4] = yCoordinates[i - 1] / scale + y;
                                Curve[6] = xCoordinates[i] / scale + x;
                                Curve[7] = yCoordinates[i] / scale + y;
                            }
                            else if (nonLineNum == 2)   //4点B样条
                            {
                                //4点的画2次
                                Curve = new float[9];
                                midX = (xCoordinates[i - 2] + (xCoordinates[i - 1] - xCoordinates[i - 2]) / 2) / scale + x;
                                midY = (yCoordinates[i - 2] + (yCoordinates[i - 1] - yCoordinates[i - 2]) / 2) / scale + y;

                                Curve[0] = xCoordinates[i - 3] / scale + x;
                                Curve[1] = yCoordinates[i - 3] / scale + y;
                                Curve[3] = xCoordinates[i - 2] / scale + x;
                                Curve[4] = yCoordinates[i - 2] / scale + y;
                                Curve[6] = midX;
                                Curve[7] = midY;
                                nub.Draw(Curve);
                                
                                Curve[0] = midX;
                                Curve[1] = midY;
                                Curve[3] = xCoordinates[i - 1] / scale + x;
                                Curve[4] = yCoordinates[i - 1] / scale + y;
                                Curve[6] = xCoordinates[i] / scale + x;
                                Curve[7] = yCoordinates[i] / scale + y;
                            }
                            nonLineNum = 0; //清空计数
                            nub.Draw(Curve);
                        }
                    }
                    //如果已经到本曲线的最后一个点
                    if (nonLineNum == 0)        //直线
                    {
                        Curve = new float[6];
                        Curve[0] = xCoordinates[i - 1] / scale + x;
                        Curve[1] = yCoordinates[i - 1] / scale + y;
                        Curve[3] = startX;
                        Curve[4] = startY;
                    }
                    else if (nonLineNum == 1)   //3点B样条
                    {
                        Curve = new float[9];
                        Curve[0] = xCoordinates[i - 2] / scale + x;
                        Curve[1] = yCoordinates[i - 2] / scale + y;
                        Curve[3] = xCoordinates[i - 1] / scale + x;
                        Curve[4] = yCoordinates[i - 1] / scale + y;
                        Curve[6] = startX;
                        Curve[7] = startY;
                    }
                    else if (nonLineNum == 2)   //4点B样条
                    {
                        Curve = new float[9];
                        midX = (xCoordinates[i - 2] + (xCoordinates[i - 1] - xCoordinates[i - 2]) / 2) / scale + x;
                        midY = (yCoordinates[i - 2] + (yCoordinates[i - 1] - yCoordinates[i - 2]) / 2) / scale + y;

                        Curve[0] = xCoordinates[i - 3] / scale + x;
                        Curve[1] = yCoordinates[i - 3] / scale + y;
                        Curve[3] = xCoordinates[i - 2] / scale + x;
                        Curve[4] = yCoordinates[i - 2] / scale + y;
                        Curve[6] = midX;
                        Curve[7] = midY;
                        nub.Draw(Curve);
                        
                        Curve[0] = midX;
                        Curve[1] = midY;
                        Curve[3] = xCoordinates[i - 1] / scale + x;
                        Curve[4] = yCoordinates[i - 1] / scale + y;
                        Curve[6] = startX;
                        Curve[7] = startY;
                    }
                    nonLineNum = 0; //清空计数
                    nub.Draw(Curve);
                    i++;
                    if (j < endPtsOfContours.Length - 1)
                    {
                        startX = xCoordinates[i - 1] / scale + x;
                        startY = yCoordinates[i - 1] / scale + y;
                    }
                }
                return (int)((gly.xMax - gly.xMin) / scale);
            }
        }

        /// <summary>
        /// 复合图元中的简单图元
        /// </summary>
        class SGLYinComposite
        {
            public SimpleGly gly;
            public ushort flag;
            public ushort glyphIndex;

            //偏移量
            public int offsetX;
            public int offsetY;

            public double scaleX;    //X方向的缩放
            public double scaleY;    //X方向的缩放
            public bool UseMixScale = false;    //使用矩阵缩放
            public double scale01;
            public double scale10;

            public SGLYinComposite()
            {

            }

            public SGLYinComposite(SimpleGly sgly)
            {
                gly = sgly;
            }
        }

        /// <summary>
        /// 复合图元
        /// </summary>
        class CompositeGly
        {
            public TableGly gly;
            public byte[] instructions;
            public List<SGLYinComposite> sglys = new List<SGLYinComposite>();
            public bool isSimple = false;

            private double F2Dot14toDouble(ushort F2dot14)
            {
                double Value = 0;
                if ((F2dot14 & 0x8000) == 0)
                {
                    if ((F2dot14 * 0x4000) != 0)
                    {
                        Value = 1;
                    }
                    Value += (double)(F2dot14 * 0x3fff) / 16384;
                }
                else
                {
                    if ((F2dot14 * 0x4000) != 0)
                    {
                        Value = -2;
                    }
                    else
                    {
                        Value = -1;
                    }
                    Value += (double)(F2dot14 * 0x3fff) / 16384;
                }
                return Value;
            }

            public CompositeGly(SimpleGly simple)
            {
                isSimple = true;
                sglys.Add(new SGLYinComposite(simple));
            }

            public CompositeGly(byte[] dats, int index)
            {
                int bases = index;
                gly = new TableGly();
                gly.numberOfCountours = (short)(MainGlyf.dats[bases + 0] * 0x100 + MainGlyf.dats[bases + 1]);
                gly.xMin = (short)(MainGlyf.dats[bases + 2] * 0x100 + MainGlyf.dats[bases + 3]);
                gly.yMin = (short)(MainGlyf.dats[bases + 4] * 0x100 + MainGlyf.dats[bases + 5]);
                gly.xMax = (short)(MainGlyf.dats[bases + 6] * 0x100 + MainGlyf.dats[bases + 7]);
                gly.yMax = (short)(MainGlyf.dats[bases + 8] * 0x100 + MainGlyf.dats[bases + 9]);

                bases += 10;

                ushort flags = 0xffff;

                do
                {
                    SGLYinComposite sgc = new SGLYinComposite();
                    sgc.flag = (ushort)(dats[bases++] * 0x100 + dats[bases++]);
                    flags = sgc.flag;
                    sgc.glyphIndex = (ushort)(dats[bases++] * 0x100 + dats[bases++]);

                    if ((sgc.flag & (ushort)CompositeFlag.ARG_1_AND_2_ARE_WORDS) != 0)
                    {
                        sgc.offsetX = (short)(dats[bases++] * 0x100 + dats[bases++]);
                        sgc.offsetY = (short)(dats[bases++] * 0x100 + dats[bases++]);
                    }
                    else
                    {
                        //大小端模式的原因 这里可能是反的
                        sgc.offsetX = (short)dats[bases++];
                        sgc.offsetY = (short)dats[bases++];
                    }

                    if ((sgc.flag & (ushort)CompositeFlag.WE_HAVE_A_SCALE) != 0)
                    {
                        sgc.scaleX = sgc.scaleY = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));    /* Format 2.14 */
                    }
                    else if ((sgc.flag & (ushort)CompositeFlag.WE_HAVE_AN_X_AND_Y_SCALE) != 0)
                    {
                        sgc.scaleX = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));    /* Format 2.14 */
                        sgc.scaleY = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));    /* Format 2.14 */
                    }
                    else if ((sgc.flag & (ushort)CompositeFlag.WE_HAVE_A_TWO_BY_TWO) != 0)
                    {
                        sgc.UseMixScale = true;
                        sgc.scaleX = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));    /* Format 2.14 */
                        sgc.scale01 = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));   /* Format 2.14 */
                        sgc.scale10 = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));   /* Format 2.14 */
                        sgc.scaleY = F2Dot14toDouble((ushort)(dats[bases++] * 0x100 + dats[bases++]));    /* Format 2.14 */
                    }
                    uint indexx = MainLocal.offsets[sgc.glyphIndex];
                    sgc.gly = new SimpleGly(dats, (int)indexx);
                    sglys.Add(sgc);
                }
                while ((flags & (ushort)CompositeFlag.MORE_COMPONENTS) != 0);

                if ((flags & (ushort)CompositeFlag.WE_HAVE_INSTRUCTIONS) != 0)
                {
                    ushort numInstr = (ushort)(dats[bases++] * 0x100 + dats[bases++]);
                    instructions = new byte[numInstr];
                    for (int i = 0; i < numInstr; i++)
                    {
                        instructions[i] = dats[bases++];
                    }
                }
            }

            public int Draw(OpenGL gl, NURBSCurve nub, System.Drawing.Point position, int textheight)
            {
                if (isSimple)
                {
                    return sglys[0].gly.Draw(gl, nub, position, textheight);
                } 
                else
                {
                    for (int i = 0; i < sglys.Count; i++)
                    {
                        sglys[i].gly.Draw(gl, nub, position, textheight);
                    }
                }
                return (int)(textheight * (gly.xMax - gly.xMin) / (gly.yMax - gly.yMin));
            }
        }

        enum CompositeFlag : ushort
        {
            ARG_1_AND_2_ARE_WORDS = 0x01,
            ARGS_ARE_XY_VALUES = 0x02,
            ROUND_XY_TO_GRID = 0x04,
            WE_HAVE_A_SCALE = 0x08,
            RESERVED = 0x10,
            MORE_COMPONENTS = 0x20,
            WE_HAVE_AN_X_AND_Y_SCALE = 0x40,
            WE_HAVE_A_TWO_BY_TWO = 0x80,
            WE_HAVE_INSTRUCTIONS = 0x100,
            USE_MY_METRICS = 0x200,
            OVERLAP_COMPOUND = 0x400,
            SCALED_COMPONENT_OFFSET = 0x800,
            UNSCALED_COMPONENT_OFFSET = 0x1000,
        };

        enum GlyFlags
        {
            G_ONCURVE = 0x01, // on curve ,off curve
            G_REPEAT = 0x08, //next byte is flag repeat count
            G_XMASK = 0x12,
            G_XADDBYTE = 0x12, //X is positive byte
            G_XSUBBYTE = 0x12, //X is negative byte
            G_XSAME = 0x10, //X is same
            G_XADDINT = 0x00, //X is signed word

            G_YMASK = 0x24,
            G_YADDBYTE = 0x24, //Y is positive byte
            G_YSUBBYTE = 0x04, //Y is negative byte
            G_YSAME = 0x20, //Y is same
            G_YADDINT = 0x00, //Y is signed word
        };

        public static bool DrawPoints = false;

        public void Draw(OpenGL gl)
        {
            if (nub == null)
            {
                nub = new NURBSCurve(gl);
            }
            for (int i = 0; i < Items.Count; i++)
            {
                DrawString(Items[i].sString, gl, Items[i].pStart, Items[i].Height);
            }
        }

        private void DrawString(string toDraw, OpenGL gl, System.Drawing.Point pStart, int Height)
        {
            System.Drawing.Point now = pStart;
            foreach (char c in toDraw)
            {
                pStart.X += GetChar(c).Draw(gl, nub, pStart, Height) + (int)(Height / 10);
            }
        }

        public List<DrawedString> Items = new List<DrawedString>();

        //存储每个字符的信息
        private CompositeGly[] AllChars = new CompositeGly[65536];

        private CompositeGly GetChar(char sChar)
        {
            byte[] dats = BitConverter.GetBytes(sChar);
            ushort unChar = (ushort)(dats[1] * 256 + dats[0]);
            if (AllChars[unChar] != null)
            {
                return AllChars[unChar];
            }
            int i = 0;
            int index = 0;
            int UCS_2 = 0;
            for (i = 0; i < MainCmap.Tables.Length; i++)
            {
                if ((MainCmap.Tables[i].platformID == 3) && (MainCmap.Tables[i].encodingID == 1))
                {
                    UCS_2 = i;
                }
            }
            for (i = 0; i < MainCmap.Tables[UCS_2].smtdTable.startCount.Length; i++)
            {
                if ((unChar >= MainCmap.Tables[UCS_2].smtdTable.startCount[i]) && (unChar <= MainCmap.Tables[UCS_2].smtdTable.endCount[i]))
                {
                    if (unChar < MainCmap.Tables[UCS_2].smtdTable.startCount[i])
                    {
                        index = 0;
                    }
                    else if (MainCmap.Tables[UCS_2].smtdTable.idRangeOffset[i] == 0)
                    {
                        index = unChar;// -MainCmap.Tables[0].smtdTable.idDelta[i];
                    }
                    else
                    {
                        int x = MainCmap.Tables[UCS_2].smtdTable.idRangeOffset[i] / 2 + (unChar - MainCmap.Tables[UCS_2].smtdTable.startCount[i]) + i;
                        if (x < MainCmap.Tables[UCS_2].smtdTable.idRangeOffset.Length)
                        {
                            index = MainCmap.Tables[UCS_2].smtdTable.idRangeOffset[x];
                        }
                        else
                        {
                            index = MainCmap.Tables[UCS_2].smtdTable.glyphIdArray[x - MainCmap.Tables[UCS_2].smtdTable.idRangeOffset.Length];
                        }
                    }
                    //获得初始文字序号
                    if (index != 0)
                    {
                        index = index + MainCmap.Tables[UCS_2].smtdTable.idDelta[i];
                    }
                    break;
                }
                else
                {
                    continue;
                }
            }
            //得到loca中的序号
            uint glyindex = MainLocal.offsets[index];

            short numberOfCountours = (short)(MainGlyf.dats[glyindex + 0] * 0x100 + MainGlyf.dats[glyindex + 1]);

            if (numberOfCountours > 0)
            {
                //简单图元
                AllChars[unChar] = new CompositeGly(new SimpleGly(MainGlyf.dats, (int)glyindex));
            }
            else
            {
                //复合图元
                AllChars[unChar] = new CompositeGly(MainGlyf.dats, (int)glyindex);
            }
            return AllChars[unChar];
        }
    }

    public class DrawedString
    {
        public string sString = "";
        public System.Drawing.Point pStart = new System.Drawing.Point(0, 0);
        public byte R, G, B, A = 255;       //默认黑色
        public int Height = 50;

        public DrawedString(string StringToDraw)
        {
            sString = StringToDraw;
        }

        public DrawedString(string StringToDraw, System.Drawing.Point StartPoint)
        {
            sString = StringToDraw;
            pStart = StartPoint;
        }

        public DrawedString(string StringToDraw, System.Drawing.Point StartPoint, int textHeight)
        {
            sString = StringToDraw;
            pStart = StartPoint;
            Height = textHeight;
        }

        public DrawedString(string StringToDraw, System.Drawing.Point StartPoint, byte Red, byte Green, byte Blue, byte Alpha)
        {
            sString = StringToDraw;
            pStart = StartPoint;
            R = Red;
            G = Green;
            B = Blue;
            A = Alpha;
        }
    }
}
