using IronRuby.Builtins;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RubyDemo
{
    class Program
    {
        static void Main(string[] args)
        {
            RubyArray rmScripts = (RubyArray)Ruby.MarshalLoad(File.ReadAllBytes(@"D:\MyConfiguration\ly09614.TCENT\Desktop\Scripts.rvdata2"));

            foreach (RubyArray rmScript in rmScripts)
                InsertScript(-1, false, rmScript);
        }


        static void InsertScript(int index, bool open, params object[] args)
        {
            Script script;
            if (args.Length == 1)
            {
                RubyArray rmScript = (RubyArray)args[0];
                rmScript[0] = GetRandomSection();
                script = new Script(rmScript);
            }
            else if (args.Length == 2)
                script = new Script(GetRandomSection(), (string)args[0], (string)args[1]);
            else
                return;
        }

        /// <summary>
        /// Retrieves a non-repeatable random integer
        /// </summary>
        /// <returns>Random integer</returns>
        static int GetRandomSection()
        {
            Random random = new Random();
            return random.Next(int.MaxValue);
        }
    }

    public class Script
    {
        // Fields
        private bool _needSave = false;
        private int _section;
        private string _title;
        private string _text;
        private string _tabPageTitle;
        private RubyArray _rmScript;
        private TabPage _tabPage;

        public Script(RubyArray rmScript)
        {
            _rmScript = rmScript;
            _section = (int)_rmScript[0];
            _title = Ruby.ConvertString((MutableString)_rmScript[1]);
            _text = Ruby.ZlibInflate((MutableString)_rmScript[2]);
        }
        public Script(int section, string title, string text)
        {
            _rmScript = new RubyArray() { section, Ruby.ConvertString(title), Ruby.ZlibDeflate(text) };
            _section = section;
            _title = title;
        }

    }
}
