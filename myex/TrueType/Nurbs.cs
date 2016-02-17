using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpGL;

namespace TOpenGL
{
    public class DPoint
    {
        public double X;
        public double Y;

        public DPoint()
        {
            X = 0;
            Y = 0;
        }
        public DPoint(double x, double y)
        {
            X = x;
            Y = y;
        }
    }

    //绘制B样条曲线、曲面 的基类
    public class Nurbs
    {
        public OpenGL gl;
        public IntPtr NurbsObject;
        public string name;
        protected bool drawPoints = true;
        protected bool drawLines = true;

        protected NurbsDisplayMode displayMode = NurbsDisplayMode.Fill;

        public enum NurbsDisplayMode : uint
        {
            OutlinePatch = OpenGL.GLU_OUTLINE_PATCH,
            OutlinePolygon = OpenGL.GLU_OUTLINE_POLYGON,
            Fill = OpenGL.GLU_FILL,
        }

        public Nurbs(OpenGL ogl)
        {
            gl = ogl;
            NurbsObject = gl.NewNurbsRenderer();
        }

        virtual public void Draw(float[] controlPoints)
        {
            //更改显示模式
            gl.NurbsProperty(NurbsObject, (int)OpenGL.GLU_DISPLAY_MODE, (float)displayMode);
            //更改采样精度
            gl.NurbsProperty(NurbsObject, (int)OpenGL.GLU_SAMPLING_TOLERANCE, 1);
        }
    }

    //绘制B样条曲线
    [Serializable()]
    public class NURBSCurve : Nurbs
    {
        float[] controlPoint = { 0.0f, 100.0f, 0.0f, 200.0f, 200.0f, 0.0f, 300.0f, 0.0f, 0.0f };

        public NURBSCurve(OpenGL gls)
            : base(gls)
        {
            base.gl = gls;
            name = "NURBS Curve";
        }

        public void Ddraw(DPoint d0, DPoint d1, DPoint d2)
        {
            controlPoint[0] = (float)d0.X;
            controlPoint[1] = (float)d0.Y;
            controlPoint[3] = (float)d1.X;
            controlPoint[4] = (float)d1.Y;
            controlPoint[6] = (float)d2.X;
            controlPoint[7] = (float)d2.Y;
            Draw(controlPoint);
        }

        //输入参数为 控制点矩阵 6个数为直线 9个数为3个控制点的B样条 12个数为4个控制点的B样条
        public override void Draw(float[] controlPoints)
        {
            base.Draw(controlPoints);


            if (controlPoints.Length == 9)
            {
                //	Begin drawing a NURBS curve.
                gl.BeginCurve(NurbsObject);
                //	Draw the curve.
                gl.NurbsCurve(NurbsObject,		//	The internal nurbs object.
                    6,					//	Number of knots.
                    knots,							//	The knots themselves.
                    3,								//	The size of a vertex.
                    controlPoints,	//	The control points.
                    3,			//	The order, i.e degree + 1.
                    OpenGL.GL_MAP1_VERTEX_3);			//	Type of data to generate.
                //	End the curve.
                gl.EndCurve(NurbsObject);
                
            }
            else if (controlPoints.Length == 12)
            {
                //	Begin drawing a NURBS curve.
                gl.BeginCurve(NurbsObject);
                //	Draw the curve.
                gl.NurbsCurve(NurbsObject,		//	The internal nurbs object.
                    8,					//	Number of knots.
                    knots1,							//	The knots themselves.
                    3,								//	The size of a vertex.
                    controlPoints,	//	The control points.
                    4,			//	The order, i.e degree + 1.
                    OpenGL.GL_MAP1_VERTEX_3);			//	Type of data to generate.
                //	End the curve.
                gl.EndCurve(NurbsObject);
            }
            else if (controlPoints.Length == 6)
            {
                gl.Begin(OpenGL.GL_LINES);
                gl.Vertex(controlPoints[0], controlPoints[1]);
                gl.Vertex(controlPoints[3], controlPoints[4]);
                gl.End();
            }

        }

        protected float[] knots = new float[] { 0, 0, 0, 1, 1, 1 };
        protected float[] knots1 = new float[] { 0, 0, 0, 0, 1, 1, 1, 1 };
    }
}
