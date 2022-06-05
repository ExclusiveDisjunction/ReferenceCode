using System.Windows;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void HelpCmd_Click(object sender, RoutedEventArgs e) //Loads the documentation window.
        {
            new Docs.Documentation().Show();
        }
        private void AboutCmd_Click(object sender, RoutedEventArgs e) //Loads the About window.
        {
            new Interaction.About().ShowDialog();
        }
    }
}
