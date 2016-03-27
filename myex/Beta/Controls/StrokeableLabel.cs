using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Beta.Controls
{
    public class StrokeableLabel : Label
    {
        public static readonly DependencyProperty TextProperty;
        public static readonly DependencyProperty StretchSizeProperty;
        public static readonly DependencyProperty FillProperty;
        public static readonly DependencyProperty StrokeProperty;
        public static readonly DependencyProperty StrokeThicknessProperty;
        static StrokeableLabel()
        {
            PropertyMetadata textMeta = new PropertyMetadata("", (DependencyObject d, DependencyPropertyChangedEventArgs e) =>
            {
                (d as StrokeableLabel).InvalidateVisual();
            });
            PropertyMetadata strchMeta = new PropertyMetadata(0);


            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(StrokeableLabel), textMeta);
            StretchSizeProperty = DependencyProperty.Register("StretchSize", typeof(double), typeof(StrokeableLabel));
            FillProperty = DependencyProperty.Register("Fill", typeof(Brush), typeof(StrokeableLabel));
            StrokeProperty = DependencyProperty.Register("Stroke", typeof(Brush), typeof(StrokeableLabel));
            StrokeThicknessProperty = DependencyProperty.Register("StrokeThickness", typeof(double), typeof(StrokeableLabel));
        }

        /// <summary>
        /// Create the outline geometry based on the formatted text.
        /// </summary>
        public void CreateText()
        {
            getformattedText(Text);
            //this.Content = Text;
        }

        PathGeometry pg = new PathGeometry();

        private void getformattedText(string str)
        {
            System.Windows.FontStyle fontStyle = FontStyles.Normal;
            FontWeight fontWeight = FontWeights.Normal;
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
                Brushes.Black // This brush does not matter since we use the geometry of the text. 
                );

            this.Width = formattedText.Width;
            this.Height = formattedText.Height;
            // Build the geometry object that represents the text.
            //pg.AddGeometry(formattedText.BuildGeometry(new System.Windows.Point(5, 5)));
            TextGeometry = formattedText.BuildGeometry(new Point(0, 0));
            // Build the geometry object that represents the text hightlight.
            if (Highlight == true)
            {
                TextHighLightGeometry = formattedText.BuildHighlightGeometry(new Point(0, 0));
            }
        }

        /// <summary>
        /// OnRender override draws the geometry of the text and optional highlight.
        /// </summary>
        /// <param name="drawingContext">Drawing context of the OutlineText control.</param>
        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);
            CreateText();
            // Draw the outline based on the properties that are set.
            drawingContext.DrawGeometry(Fill, new Pen(Stroke, StrokeThickness), TextGeometry);
            // Draw the text highlight based on the properties that are set.
            if (Highlight == true)
            {
                drawingContext.DrawGeometry(null, new Pen(Stroke, StrokeThickness), TextHighLightGeometry);
            }
        }

        /// <summary>
        /// 字符串的格式化几何对象
        /// </summary>
        public Geometry TextGeometry { get; private set; }

        /// <summary>
        /// 是否高亮（暂时不支持）
        /// </summary>
        public bool Highlight { get; set; }

        /// <summary>
        /// 高亮几何对象
        /// </summary>
        public Geometry TextHighLightGeometry { get; private set; }

        /// <summary>
        /// 字符间距
        /// </summary>
        public double StretchSize
        {
            get
            {
                return (double)GetValue(StretchSizeProperty);
            }
            set
            {
                SetValue(StretchSizeProperty, value);
            }
        }

        /// <summary>
        /// 字符串格式化对象的填充花刷
        /// </summary>
        public Brush Fill { get { return GetValue(FillProperty) as Brush; } set { SetValue(FillProperty, value); } }

        /// <summary>
        /// 边缘画刷
        /// </summary>
        public Brush Stroke { get { return GetValue(StrokeProperty) as Brush; } set { SetValue(StrokeProperty, value); } }

        /// <summary>
        /// 边缘宽度
        /// </summary>
        public double StrokeThickness { get { return (double)GetValue(StrokeThicknessProperty); } set { SetValue(StrokeThicknessProperty, value); } }

        /// <summary>
        /// 显示的文字
        /// </summary>
        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                SetValue(TextProperty, value);
            }
        }

        /*<Controls:StrokeableLabel Text="测试文本" Stroke="red" StrokeThickness="2"  FontWeight="Bold" FontSize="50">
                <Controls:StrokeableLabel.Fill>
                    <ImageBrush ImageSource="/StrokeableLabelTest;component/Images/20085385922474_2.jpg" />
                </Controls:StrokeableLabel.Fill>
            </Controls:StrokeableLabel>
            <Controls:StrokeableLabel Fill="Transparent" FontSize="50" FontWeight="Light" StrokeThickness="8" Text="测试文本" >
                <Controls:StrokeableLabel.Stroke>
                    <ImageBrush ImageSource="/StrokeableLabelTest;component/Images/05.jpg" />
                </Controls:StrokeableLabel.Stroke>
            </Controls:StrokeableLabel>*/

        /*<StackPanel Orientation="Vertical">
            <Controls:StrokeableLabel Text="测试文本" Fill="Yellow" Stroke="Black" StrokeThickness="0.3" FontWeight="Bold" FontSize="50"/>
            <Controls:StrokeableLabel Text="测试文本" Fill="Yellow" Stroke="Red" StrokeThickness="0.7" FontWeight="DemiBold" FontSize="50">
                <Controls:StrokeableLabel.Effect>
                    <DropShadowEffect Color="Black" BlurRadius="15" RenderingBias="Quality" Direction="290" ShadowDepth="5" Opacity="1"  />
                </Controls:StrokeableLabel.Effect>
            </Controls:StrokeableLabel>
            <Controls:StrokeableLabel Text="测试文本" Fill="White" StrokeThickness="2" FontWeight="Bold" FontSize="50">
                <Controls:StrokeableLabel.Stroke>
                    <LinearGradientBrush>
                        <LinearGradientBrush.GradientStops>
                            <GradientStop Color="Blue" Offset="0.2"/>
                            <GradientStop Color="Brown" Offset="0.3"/>
                            <GradientStop Color="PowderBlue" Offset="0.7"/>
                            <GradientStop Color="Red" Offset="1"/>
                        </LinearGradientBrush.GradientStops>
                    </LinearGradientBrush>
                </Controls:StrokeableLabel.Stroke>
            </Controls:StrokeableLabel>
            
        </StackPanel>*/
    }
}