using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;

namespace TrueType
{
    class OpenType
    {
        unsafe public OpenType(char[] FFT_File)
        {
            IntPtr library = IntPtr.Zero;
            IntPtr face = IntPtr.Zero;

            library = Marshal.AllocHGlobal(192);

            int bError = OpenTypeImport.FT_Init_FreeType((IntPtr)(&library));

            if (bError != 0)
            {
                System.Windows.Forms.MessageBox.Show("Library初始化失败！");
            }

            string path = "C:\\MSYH.TTF";

            OpenTypeImport.Error_Code iError = OpenTypeImport.FT_New_Face(library, path, 0, (IntPtr)(&face));

            iError = OpenTypeImport.FT_Select_Charmap(face, 0x756e6963);

            iError = OpenTypeImport.FT_Set_Char_Size(face, 0, 16 * 64, 300, 300);

            UInt32 glyph_index = OpenTypeImport.FT_Get_Char_Index(face, 0x0034);

            iError = OpenTypeImport.FT_Load_Glyph(face, glyph_index, OpenTypeImport.Load_Flags.FT_LOAD_DEFAULT);

            return;
        }
    }
}
