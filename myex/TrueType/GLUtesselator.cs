using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpGL;
using System.Runtime.InteropServices;

namespace TrueType
{
    class GLUtesselator
    {
    }

    public class Context
    {
        OpenGL gl;
        TessellationSet sets;

        public Context(OpenGL gls)
        {
            gl = gls;
            sets = new TessellationSet();
            IntPtr tessellator = gl.NewTess();

            try
            {
                gl.TessCallback(tessellator, OpenGL.GLU_TESS_BEGIN_DATA, new SharpGL.Delegates.Tesselators.BeginData(tessBegin));
                gl.TessCallback(tessellator, OpenGL.GLU_TESS_END_DATA, new SharpGL.Delegates.Tesselators.EndData(tessEnd));
                gl.TessCallback(tessellator, OpenGL.GLU_TESS_VERTEX_DATA, new SharpGL.Delegates.Tesselators.VertexData(tessVertex));

                gl.TessBeginPolygon(tessellator, IntPtr.Zero);
                gl.TessBeginContour(tessellator);
                double[] dats = { 0.0, 20.0, 0.0 };
                gl.TessVertex(tessellator, dats, dats);
                dats[0] = 20.0;
                gl.TessVertex(tessellator, dats, dats);
                dats[1] = 0.0;
                gl.TessVertex(tessellator, dats, dats);
                dats[1] = 0.0;
                dats[2] = 0.0;
                gl.TessVertex(tessellator, dats, dats);
                gl.TessEndContour(tessellator);
                gl.TessEndPolygon(tessellator);
            }
            finally
            {
                gl.DeleteTess(tessellator);
            }
        }

        public class FloatPairVector
        {
            public float x, y;
        }

        public class Triangle
        {
            public int FirstVertexIndex;
            public int SecondVertexIndex;
            public int ThirdVertexIndex;

            public Triangle(int first, int second, int third)
            {
                FirstVertexIndex = first;
                SecondVertexIndex = second;
                ThirdVertexIndex = third;
            }
        }


        public class TessellationSet
        {
            public TessellationSet()
            {

            }

            ~TessellationSet()
            {

            }

            public FloatPairVector Vertices;

            public List<Triangle> Triangles = new List<Triangle>();

            public List<int[]> Indices = new List<int[]>();

            public int IntermediateVertexIndex1;
            public int IntermediateVertexIndex2;

            public uint GLType;
        }

        public void tessBegin(uint GLType, IntPtr p)
        {
            sets.GLType = GLType;
            sets.IntermediateVertexIndex1 = -1;
            sets.IntermediateVertexIndex2 = -1;
        }

        public void tessEnd(IntPtr p)
        {

        }

        public void tessVertex(IntPtr vertexData, IntPtr p)
        {
            double[] dats = new double[3];
            Marshal.Copy(vertexData, dats, 0, 3);

            switch(sets.GLType)
            {
                case OpenGL.GL_TRIANGLES:
                    {
                        if (sets.IntermediateVertexIndex1 == -1)
                        {
                            sets.IntermediateVertexIndex1 = 1;
                            sets.IntermediateVertexIndex2 = -1;
                        }
                        else if (sets.IntermediateVertexIndex2 == -1)
                        {
                            sets.IntermediateVertexIndex2 = 1;
                        }
                        else
                        {
                            sets.Triangles.Add(new Triangle(sets.IntermediateVertexIndex1, sets.IntermediateVertexIndex2, 1));
                            sets.IntermediateVertexIndex1 = -1;
                        }
                    }
                    break;
            }
        }
    }
}
