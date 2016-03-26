using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Beta.Controls
{
    /// <summary>
    /// StrokeableLabel.xaml 的交互逻辑
    /// </summary>
    public partial class StrokeableLabel : Label
    {
        public StrokeableLabel()
        {
            InitializeComponent();
        }
        public Geometry TextGeometry;
        public bool Highlight;

        public Geometry TextHighLightGeometry;
        public Brush Fill;
        public Brush Stroke;
        public double StrokeThickness;

        private void getformattedText(string str)
        {
            // Create the formatted text based on the properties set.
            FormattedText formattedText = new FormattedText(
                str,
                CultureInfo.GetCultureInfo("en-us"),
                FlowDirection.LeftToRight,
                new Typeface(
                    FontFamily,
                    FontStyle,
                    FontWeight,
                    FontStretch),
                FontSize,
                System.Windows.Media.Brushes.Black // This brush does not matter since we use the geometry of the text. 
                );


            this.Width = formattedText.Width;
            this.Height = formattedText.Height;
            // Build the geometry object that represents the text.
            //pg.AddGeometry(formattedText.BuildGeometry(new System.Windows.Point(5, 5)));
            TextGeometry = formattedText.BuildGeometry(new System.Windows.Point(0, 0));
            // Build the geometry object that represents the text hightlight.
            if (Highlight == true)
            {
                TextHighLightGeometry = formattedText.BuildHighlightGeometry(new System.Windows.Point(0, 0));
            }
        }


        /// <summary>
        /// OnRender override draws the geometry of the text and optional highlight.
        /// </summary>
        /// <param name="drawingContext">Drawing context of the OutlineText control.</param>
        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);

            // Draw the outline based on the properties that are set.
            drawingContext.DrawGeometry(Fill, new System.Windows.Media.Pen(Stroke, StrokeThickness), TextGeometry);
            // Draw the text highlight based on the properties that are set.
            if (Highlight == true)
            {
                drawingContext.DrawGeometry(null, new System.Windows.Media.Pen(Stroke, StrokeThickness), TextHighLightGeometry);
            }
        }

    }
}
