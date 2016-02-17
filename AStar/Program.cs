using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Threading;

namespace AStar
{
    static class Program
    {
        //环境量

        static int width;
        static int height;
        static Point a;
        static Point b;
        static int[,] map;
        static int[,] dirs;
        static int[,] visited;
        static int ans;//所发步数
        //end环境量

        static void Main1(string[] args)
        {
            //环境量
            width = 8;
            height = 8;
            dirs = new int[8, 2] { { 0, 1 }, { 1, 1 }, { 1, 0 }, { -1, -1 }, { -1, 0 }, { 0, -1 }, { -1, 1 }, { 1, -1 } };
            map = new int[width, height];
            visited = new int[width, height];

            a = new Point();
            a.x = 0; a.y = 0;
            b = new Point();
            b.x = 7; b.y = 7;
            map[2, 0] = 1;
            map[2, 1] = 1;
            map[2, 2] = 1;
            map[2, 3] = 1;
            map[2, 4] = 1;
            map[2, 5] = 1;
            map[2, 6] = 1;
            map[2, 7] = 0;

            map[4, 0] = 1;
            map[4, 1] = 1;
            map[4, 2] = 1;
            map[4, 3] = 1;
            map[4, 4] = 1;
            map[4, 5] = 1;
            map[4, 6] = 1;
            map[4, 7] = 0;

            map[6, 0] = 0;
            map[6, 1] = 1;
            map[6, 2] = 1;
            map[6, 3] = 1;
            map[6, 4] = 1;
            map[6, 5] = 1;
            map[6, 6] = 1;
            map[6, 7] = 1;

            AStar();


        }

        static void AStar()
        {
            queue que = new queue();
            que.Add(a);

            while (que.content.Count > 0)
            {
                var cPoint = que.Pop();
                Console.WriteLine("x:{0} y:{1}", cPoint.x, cPoint.y);
                visited[cPoint.x, cPoint.y] = 1;
                if (cPoint.x == b.x && cPoint.y == b.y)
                {
                    ans = cPoint.step;
                    break;
                }
                for (var i = 0; i < 8; i++)
                {
                    Point near = new Point();
                    near.x = cPoint.x + dirs[i, 0];
                    near.y = cPoint.y + dirs[i, 1];
                    if (_in(near) && visited[near.x, near.y] == 0 && map[near.x, near.y] == 0)
                    {
                        visited[near.x, near.y] = 1;
                        near.g = cPoint.g + 10;
                        near.h = Heuristic(near);
                        near.f = near.g + near.h;
                        near.step = cPoint.step + 1;
                        que.Add(near);
                    }

                }
            }
        }

        //判断knight是否在棋盘内
        static bool _in(Point a)
        {
            if (a.x < 0 || a.y < 0 || a.x >= width || a.y >= height)
                return false;
            return true;
        }

        //manhattan估价函数
        static int Heuristic(Point p)
        {
            return (Math.Abs(p.x - b.x) + Math.Abs(p.y - b.y)) * 10;
        }
    }

    public class Point
    {
        /// <summary>横轴</summary>
        public int x;
        /// <summary>纵轴</summary>
        public int y;
        /// <summary>步数</summary>
        public int step;
        /// <summary>起点到当前点的距离</summary>
        public int g;
        /// <summary>评估量</summary>
        public int h;
        /// <summary>花费</summary>
        public int f;
    }
    public class queue
    {
        public List<Point> content = new List<Point>();
        /// <summary>
        /// 顺序插入f值最小的在上面
        /// 倒序
        /// </summary>
        public void Add(Point p)
        {
            var max = content.Count - 1;
            if (max == -1)
            {
                content.Add(p);
                return;
            }
            var min = 0;
            var middle = 0;
            while (max >= min)
            {
                middle = (max + min) / 2;
                if (content[middle].f == p.f)
                {
                    break;
                }
                //在左半边
                else if (content[middle].f > p.f)
                    max = middle - 1;
                //在右半边
                else
                    min = middle + 1;
            }
            var miditem = content[middle];
            if (miditem.f < p.f)
            {
                content.Insert(middle + 1, p);
            }
            else
            {
                content.Insert(middle, p);
            }

        }

        /// <summary>取出最小一个</summary>
        public Point Pop()
        {
            Point p = content[0];
            content.RemoveAt(0);
            return p;
        }
    }


    static class P
    {
        static void Main()
        {
           var end= a(0, 1, 29);
           Console.Write(end);
        }
        static int a(int i, int k,int step)
        {
            --step;
            if (step == 0) return i + k;
            return a(k, i + k, step);
            
        }
    }
    class A
    {
        public virtual void F() { Console.WriteLine("A.F"); }
    }
    class B : A
    {
        public override void F() { Console.WriteLine("B.F"); }
    }
    class C : B
    {
        new public virtual void F() { Console.WriteLine("C.F"); }
    }
    class D : C
    {
        public override void F() { Console.WriteLine("D.F"); }
    }

}
