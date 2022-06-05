using System.Windows;

using Phosphor;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void MaximizeCmd_Click(object sender, RoutedEventArgs e) //Sets the window state to maximize.
        {
            this.WindowState = WindowState.Maximized;
        }
        private void MinimizeCmd_Click(object sender, RoutedEventArgs e) //Sets the window state to minimized.
        {
            this.WindowState = WindowState.Minimized;
        }
        private void NormalCmd_Click(object sender, RoutedEventArgs e) //Sets the window state to normal.
        {
            this.WindowState = WindowState.Normal;
        }
        private void AutoSizeCmd_Click(object sender, RoutedEventArgs e) //If the window state is normal, it maximizes. If it is maximized, it sets it normal.
        {
            if (this.WindowState == WindowState.Normal)
                this.WindowState = WindowState.Maximized;
            else
                this.WindowState = WindowState.Normal;
        }

        private void WelcomeScreenCmd_Click(object sender, RoutedEventArgs e) //Loads the welcome screen.
        {
            Interaction.Welcome Wel = new Interaction.Welcome();
            Wel.ShowDialog();
            if (Wel.ShutDown) App.ThrowBeginExit(this);
        }
    }
}
