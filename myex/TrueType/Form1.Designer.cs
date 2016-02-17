namespace TrueType
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.OpenGLControl = new SharpGL.OpenGLControl();
            ((System.ComponentModel.ISupportInitialize)(this.OpenGLControl)).BeginInit();
            this.SuspendLayout();
            // 
            // OpenGLControl
            // 
            this.OpenGLControl.BitDepth = 32;
            this.OpenGLControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.OpenGLControl.DrawFPS = true;
            this.OpenGLControl.FrameRate = 62.5F;
            this.OpenGLControl.Location = new System.Drawing.Point(0, 0);
            this.OpenGLControl.Name = "OpenGLControl";
            this.OpenGLControl.RenderContextType = SharpGL.RenderContextType.NativeWindow;
            this.OpenGLControl.Size = new System.Drawing.Size(784, 397);
            this.OpenGLControl.TabIndex = 0;
            this.OpenGLControl.Load += new System.EventHandler(this.OpenGLControl_Load);
            this.OpenGLControl.OpenGLDraw += new System.Windows.Forms.PaintEventHandler(this.OpenGLControl_OpenGLDraw);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 397);
            this.Controls.Add(this.OpenGLControl);
            this.Name = "Form1";
            this.Text = "数据查看器";
            ((System.ComponentModel.ISupportInitialize)(this.OpenGLControl)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private SharpGL.OpenGLControl OpenGLControl;

    }
}

