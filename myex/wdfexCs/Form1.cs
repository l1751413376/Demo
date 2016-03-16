using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Media;

namespace 梦幻西游wdf文件读取
{
    public partial class Form1 : Form
    {


        string wdfpath;
        WdfClass wdf;

        SoundPlayer sp;
        public Form1()
        {
            InitializeComponent();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            
            OpenFileDialog of = new OpenFileDialog();
            of.DefaultExt = ".wdf";
          

            if (of.ShowDialog() == DialogResult.OK)
            {
                wdfpath = of.FileName;

                wdf = new WdfClass(wdfpath);
                wdf.ReadWdfFile();
               
                label1.Text =Convert.ToString(wdf.wdfhead.Flag,16)+"     sum="+wdf.wdfhead.FileSum.ToString();

                listBox1.Items.Clear();
                for (int i = 0; i < wdf.filesum; i++)
                { 
                    var item =wdf.filelist[i];
                    listBox1.Items.Add(string.Format("{0:X8}.{1}",item.Uid, WdfClass.FILETYPE[item.FileType]));
                }

                //wdf.ReadFileData(wdf.filelist[0].Uid);
                //FileStream f = File.Open(Convert.ToString((wdf.filelist[0].Uid), 16), FileMode.OpenOrCreate);
                //f.Write(wdf.filelist[0].FileData, 0, wdf.filelist[0].FileData.Length);
                //f.Flush();
                //f.Close();
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex != -1)
            {
                
                string s = listBox1.SelectedItem.ToString();
                label2.Text = s + "  文件类型：" + WdfClass.FILETYPE[wdf.filelist[listBox1.SelectedIndex].FileType];

                if (WdfClass.FILETYPE[wdf.filelist[listBox1.SelectedIndex].FileType] == "wav" || WdfClass.FILETYPE[wdf.filelist[listBox1.SelectedIndex].FileType] == "mp3")
                {
                  //wdf.ReadFileData(wdf.filelist[listBox1.SelectedIndex].Uid);
                  //sp=new SoundPlayer(new MemoryStream(wdf.filelist[listBox1.SelectedIndex].FileData));
                  //sp.Play();
                }
            }
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex != -1)
            {
                wdf.ReadFileData(wdf.filelist[listBox1.SelectedIndex].Uid);
                FileStream f = File.Open(listBox1.SelectedItem.ToString() + "." + WdfClass.FILETYPE[wdf.filelist[listBox1.SelectedIndex].FileType], FileMode.OpenOrCreate);
                f.Write(wdf.filelist[listBox1.SelectedIndex].FileData, 0, wdf.filelist[listBox1.SelectedIndex].FileData.Length);
                f.Flush();
                f.Close();
                MessageBox.Show("OK");
            }
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            foreach (var item in wdf.filelist)
            {
                wdf.ReadFileData(item.Uid);
                FileStream f = File.Open(string.Format("{0:X8}.{1}",item.Uid, WdfClass.FILETYPE[item.FileType]), FileMode.OpenOrCreate);
                f.Write(wdf.filelist[listBox1.SelectedIndex].FileData, 0, wdf.filelist[listBox1.SelectedIndex].FileData.Length);
                f.Flush();
                f.Close();
            }
        }
    }
}
