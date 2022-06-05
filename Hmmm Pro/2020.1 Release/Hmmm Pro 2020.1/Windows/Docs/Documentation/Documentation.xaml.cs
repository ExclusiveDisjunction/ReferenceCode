using System.Windows;

namespace HmPro.Windows.Docs
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class Documentation : Window
    {
        public Documentation()
        {
            InitializeComponent();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Ins.OpenWindows.Remove(this);   
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.WindowState = Ins.LastState;
            Ins.OpenWindows.Add(this);
        }
    }
}
