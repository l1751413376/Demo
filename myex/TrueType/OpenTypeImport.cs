using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;

namespace TrueType
{
    public class OpenTypeImport
    {

        #region 结构体

        struct FT_Library
        {
            byte[] Libs;
        }
        #endregion

        #region 枚举

        public enum Error_Code : int
        {
            FT_Err_Unknown_File_Format = 2,
            FT_Err_Invalid_Argument = 6,
        };

        public enum Load_Flags : int
        {
            FT_LOAD_DEFAULT = 0x00,
            FT_LOAD_NO_SCALE = 0x01,
            FT_LOAD_NO_HINTING = 0x02,
            FT_LOAD_RENDER = 0x04,
            FT_LOAD_NO_BITMAP = 0x08,
            FT_LOAD_VERTICAL_LAYOUT = 0x10,
            FT_LOAD_FORCE_AUTOHINT = 0x20,
            FT_LOAD_CROP_BITMAP = 0x40,
            FT_LOAD_PEDANTIC = 0x80,
            FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH = 0x0200,
            FT_LOAD_NO_RECURSE = 0x0400,
            FT_LOAD_IGNORE_TRANSFORM = 0x0800,
            FT_LOAD_MONOCHROME = 0x1000,
            FT_LOAD_LINEAR_DESIGN = 0x2000,
            FT_LOAD_NO_AUTOHINT = 0x8000,
        };

        #endregion

        #region API函数
        /// <summary>初始化FreeType。
        /// <para> 待初始化的Library。</para>
        /// <returns>初始化是否成功</returns>
        /// </summary>
        [DllImport("freetype.dll")]
        public static extern int FT_Init_FreeType(IntPtr library);

        /// <summary>初始化FreeType。
        /// <para> 使用的Library。</para>
        /// <para> 文件地址。</para>
        /// <para> 字体编号 一般为0。</para>
        /// <para> 以初始化的字体。</para>
        /// <returns>错误编码，0为正确。</returns>
        /// </summary>
        [DllImport("freetype.dll")]
        public static extern Error_Code FT_New_Face(IntPtr library, string filepathname, int face_index, IntPtr aface);

        /// <summary>设置编码格式。
        /// <para> 使用的face。</para>
        /// <para> 编码方式。</para>
        /// <returns>错误编码，0为正确。</returns>
        /// </summary>
        [DllImport("freetype.dll")]
        public static extern Error_Code FT_Select_Charmap(IntPtr face, int encoding);

        /// <summary>设置字体大小。
        /// <para> 使用的face。</para>
        /// <para> 文字宽度。</para>
        /// <para> 文字高度。</para>
        /// <para> Dpi Horz。</para>
        /// <para> Dpi Vert。</para>
        /// <returns>错误编码，0为正确。</returns>
        /// </summary>
        [DllImport("freetype.dll")]
        public static extern Error_Code FT_Set_Char_Size(IntPtr face, int char_width, int char_height, uint horz_resolution, uint vert_resolution);

        /// <summary>查询某个字所在的序号。
        /// <para> 使用的face。</para>
        /// <para> 查询的字的编码。</para>
        /// <returns>查询到的序号。</returns>
        /// </summary>
        [DllImport("freetype.dll")]
        public static extern uint FT_Get_Char_Index(IntPtr face, uint charcode);

        /// <summary>根据序号读取glyph表的图元信息。
        /// <para> 使用的face。</para>
        /// <para> 字的序号。</para>
        /// <para> 读取标志。</para>
        /// <returns>错误编码，0为正确。</returns>
        /// </summary>
        [DllImport("freetype.dll")]
        public static extern Error_Code FT_Load_Glyph(IntPtr face, uint glyph_index, Load_Flags load_flags);


        #endregion
    }
}
