using System.Windows.Input;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void HelpCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the documentation window.
        {
            new Docs.Documentation().Show();
        }
        private void AboutCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the About window.
        {
            new Interaction.About().ShowDialog();
        }
    }
}
