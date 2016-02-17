using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using SharpGL;

namespace TrueType
{
    public partial class Form1 : Form
    {
        OpenGL gl;
        OpenType ot;
        Fonts ft;

        public Form1()
        {
            InitializeComponent();

            string s = AppDomain.CurrentDomain.BaseDirectory;
            string path = s + @"\MSYH.TTF";
            path = @"C:\\MSYH.TTF";
            FileInfo fi = new FileInfo(path);
            s = fi.FullName;
            char[] pWSName = new char[256];
            s.CopyTo(0, pWSName, 0, s.Length);
            ot = new OpenType(pWSName);
            //*/
        }

        Stopwatch sw = new Stopwatch();
        int count = 0;
        long total = 0;
        private void OpenGLControl_OpenGLDraw(object sender, PaintEventArgs e)
        {
            gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
            gl.MatrixMode(OpenGL.GL_PROJECTION);
            gl.LoadIdentity();
            gl.Ortho2D(0.0, 640, 0.0, 480);
            gl.Viewport(0, 0, 640, 480);
            gl.MatrixMode(OpenGL.GL_MODELVIEW);
            gl.LoadIdentity();
            gl.Color(1.0, 0.0, 1.0, 1.0);

            count++;
            sw.Reset();
            sw.Start();
            ft.Draw(gl);
            sw.Stop();
            total += sw.ElapsedTicks;
            if (count >= 100)
            {
                total += 1;
            }
        }

        private void OpenGLControl_Load(object sender, EventArgs e)
        {
            ft = new Fonts(".\\MSYH.TTF");
            ft.Items.Add(new DrawedString("O", new Point(150, 150), 120));
            gl = OpenGLControl.OpenGL;
            gl.ShadeModel(OpenGL.GL_SMOOTH);
            gl.ClearColor(0.7f, 0.8f, 0.8f, 1.0f);
            gl.ClearDepth(1.0f);

            gl.Enable(OpenGL.GL_DEPTH_TEST);
            gl.BlendFunc(OpenGL.GL_SRC_ALPHA, OpenGL.GL_ONE_MINUS_SRC_ALPHA);
            gl.Enable(OpenGL.GL_BLEND);
            gl.PointSize(8.0f);
            gl.Enable(OpenGL.GL_POINT_SMOOTH);
            gl.Hint(OpenGL.GL_POINT_SMOOTH_HINT, OpenGL.GL_FASTEST);

            gl.LineWidth(0.5f);
            gl.Enable(OpenGL.GL_LINE_SMOOTH);
            gl.Hint(OpenGL.GL_LINE_SMOOTH_HINT, OpenGL.GL_FASTEST);
        }
    }
}
