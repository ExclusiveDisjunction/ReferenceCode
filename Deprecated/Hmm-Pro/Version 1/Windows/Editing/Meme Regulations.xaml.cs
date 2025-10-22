using System.Windows;

namespace HmPro.Windows.Editing
{
    /// <summary>
    /// Interaction logic for Meme_Regulations.xaml
    /// </summary>
    public partial class MemeRegulations : Window
    {
        public MemeRegulations()
        {
            InitializeComponent();
        }

        /*This class is not used in Hmmm Pro 2019.1, and no function commenting is supplied.*/

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Scripting.Ins.OpenWindows.Add(this);
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Scripting.Ins.OpenWindows.Remove(this);
        }
    }
}
