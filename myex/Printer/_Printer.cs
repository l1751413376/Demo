using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Printing;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;

namespace Printer
{
    public class TestDocumentPaginator : DocumentPaginator
    {
        public int _pageCount;
        public Size _pageSize;


        #region 重写
        public override DocumentPage GetPage(int pageNumber)
        {
            var visual = new DrawingVisual();

            using (DrawingContext dc = visual.RenderOpen())
            {
                //设置要绘制的文本，文本字体，大小，颜色等
                FormattedText text = new FormattedText(string.Format("第{0}页", pageNumber + 1),
                                                     CultureInfo.CurrentCulture,
                                                     FlowDirection.LeftToRight,
                                                     new Typeface("宋体"),
                                                     30,
                                                     Brushes.Black);

                //文本的左上角位置
                Point leftpoint = new Point(0, 0);

                dc.DrawText(text, leftpoint);
                
            }

            return new DocumentPage(visual, _pageSize, new Rect(_pageSize), new Rect(_pageSize));
        }
        #endregion
        #region 构造
        public TestDocumentPaginator()
        {
            //这个数据可以根据你要打印的内容来计算
            _pageCount = 5;

            //我们使用A3纸张大小
            var pageMediaSize = LocalPrintServer.GetDefaultPrintQueue()
                              .GetPrintCapabilities()
                              .PageMediaSizeCapability
                              .FirstOrDefault(x => x.PageMediaSizeName == PageMediaSizeName.ISOA3);

            if (pageMediaSize != null)
            {
                _pageSize = new Size((double)pageMediaSize.Width, (double)pageMediaSize.Height);
            }
        }
        #endregion

        public override bool IsPageCountValid
        {
            get
            {
                return true;
            }
        }

        public override int PageCount
        {
            get
            {
                return _pageCount;
            }
        }

        public override Size PageSize
        {
            get;
            set;
        }

        public override IDocumentPaginatorSource Source
        {
            get
            {
                return null;
            }
        }
    }


    public class _Printer
    {
        private void LoadPrinterList()
        {
            var printServer = new PrintServer();

            //获取全部打印机
            var PrinterList = printServer.GetPrintQueues();

            //获取默认打印机
            var DefaultPrintQueue = LocalPrintServer.GetDefaultPrintQueue();

            //纸张
            var pageSizeCollection = DefaultPrintQueue.GetPrintCapabilities().PageMediaSizeCapability;
        }

        private void DrawSomething()
        {
            var visual = new DrawingVisual();

            using (DrawingContext dc = visual.RenderOpen())
            {
                dc.DrawRectangle(Brushes.Black, new Pen(Brushes.Black, 1), new Rect(0, 0, 100, 100));
            }
        }
    }

    public class DrawingCanvas : Canvas
    {
        #region 字段
        private List<Visual> _visuals = new List<Visual>();
        #endregion

        #region 公有方法

        public void AddVisual(Visual visual)
        {
            _visuals.Add(visual);

            base.AddLogicalChild(visual);
            base.AddVisualChild(visual);
        }

        public void RemoveVisual(Visual visual)
        {
            _visuals.Remove(visual);

            base.RemoveLogicalChild(visual);
            base.RemoveVisualChild(visual);
        }

        public void RemoveAll()
        {
            while (_visuals.Count != 0)
            {
                base.RemoveLogicalChild(_visuals[0]);
                base.RemoveVisualChild(_visuals[0]);

                _visuals.RemoveAt(0);
            }
        }

        #endregion

        #region 构造

        public DrawingCanvas()
        {
            Width = 200;
            Height = 200;
        }
        #endregion

        #region 重写
        protected override int VisualChildrenCount
        {
            get
            {
                return _visuals.Count;
            }
        }

        protected override Visual GetVisualChild(int index)
        {
            return _visuals[index];
        }
        #endregion
    }


}
