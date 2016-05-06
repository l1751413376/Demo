using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Beta.Controls
{
    public class UnitProperties
    {
        /// <summary>
        /// 0当前,1最大值
        /// </summary>
        public Array HP=new Array();

        public Array LP = new Array();
    }
    public class Array
    {
        double[] Array1 = new double[16];
        public Array()
        {
            for (int i = 2; i < Array1.Length; i++)
            {
                Array1[i] = Double.NaN;
            }
        }
        public double this[int index]
        {
            get { return Array1[index]; }
            set { Array1[index] = value; }
        }
    }

    public class SkillProperties 
    {
        /// <summary>
        /// 类型 0-攻
        /// </summary>
        public int Type;

        public int Hit = 100;
    }
}
