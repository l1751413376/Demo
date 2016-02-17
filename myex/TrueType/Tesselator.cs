using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpGL;
using System.Runtime.InteropServices;

namespace TrueType
{
    class Tesselator
    {
        public IntPtr Tessobj;
        OpenGL gl;

        public Tesselator()
        {

        }

        public void InitTess(OpenGL gls)
        {
            if (gl == null)
            {
                gl = gls;
                Tessobj = gl.NewTess();
                //注册回调函数
                SharpGL.Delegates.Tesselators.Vertex vert = new SharpGL.Delegates.Tesselators.Vertex(gl.Vertex3d);
                gl.TessCallback(Tessobj, OpenGL.GLU_TESS_VERTEX, vert);
                gl.TessCallback(Tessobj, OpenGL.GLU_TESS_BEGIN, new SharpGL.Delegates.Tesselators.Begin(gl.Begin));
                gl.TessCallback(Tessobj, OpenGL.GLU_TESS_END, new SharpGL.Delegates.Tesselators.End(gl.End));
                gl.TessCallback(Tessobj, OpenGL.GLU_TESS_ERROR, new SharpGL.Delegates.Tesselators.Error(Error));
                //设置环绕规则
                gl.TessProperty(Tessobj, OpenGL.GLU_TESS_WINDING_RULE, OpenGL.GLU_TESS_WINDING_ODD);
                double d = 0;
                gl.GetTessProperty(Tessobj, OpenGL.GLU_TESS_WINDING_RULE, ref d);
            }
        }

        #region CallBacks
        void Vertex(IntPtr data)
        {
            double[] dats = new double[3];
            Marshal.Copy(data, dats, 0, 3);
            gl.Vertex(dats[0], dats[1], dats[2]);
        }

        void Begin(uint mode)
        {
            gl.Begin(mode);
        }

        void Error(uint error)
        {
            string e = gl.ErrorString(error);
        }

        void EdgeFlag(bool flag)
        {

        }

        void End()
        {
            gl.End();
        }

        void Combine(double[] coords, IntPtr vertexData, float[] weight, double[] outData)
        {

        }

        void BeginData(uint mode, IntPtr userData)
        {

        }

        void EdgeFlagData(bool flag, IntPtr userData)
        {

        }

        void VertexData(IntPtr data, IntPtr userData)
        {

        }

        void EndData(IntPtr userData)
        {

        }

        void CombineData(double[] coords, IntPtr vertexData, float[] weight, double[] outData, IntPtr userData)
        {

        }

        void ErrorData(uint error, IntPtr userData)
        {

        }
        #endregion
    }
}
