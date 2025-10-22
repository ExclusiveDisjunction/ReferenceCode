using System.Windows;

using Lyseria;
using HmPro.Scripting;

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
            {
                this.WindowState = WindowState.Maximized;
            }
            else
            {
                this.WindowState = WindowState.Normal;
            }
        }

        private void WelcomeScreenCmd_Click(object sender, RoutedEventArgs e) //Loads the welcome screen.
        {
            WelcomeGrid.Visibility = Visibility.Visible;

            WelcomeGrid.Visibility = Visibility.Visible;
            ShowWelcome.IsChecked = Ins.ShowWelcome;

            if (Ins.GetRecent(RecentOptions.Rec1) != null)
            {
                Recent1.Visibility = Visibility.Visible;
                Session1.Content = Ins.RecentName(RecentOptions.Rec1);
                Session1Path.Content = Ins.GetRecent(RecentOptions.Rec1);
            }
            else
            {
                Recent1.Visibility = Visibility.Hidden;
            }

            if (Ins.GetRecent(RecentOptions.Rec2) != null)
            {
                Recent2.Visibility = Visibility.Visible;
                Session2.Content = Ins.RecentName(RecentOptions.Rec2);
                Session2Path.Content = Ins.GetRecent(RecentOptions.Rec2);
            }
            else
            {
                Recent2.Visibility = Visibility.Hidden;
            }

            if (Ins.GetRecent(RecentOptions.Rec3) != null)
            {
                Recent3.Visibility = Visibility.Visible;
                Session3.Content = Ins.RecentName(RecentOptions.Rec3);
                Session3Path.Content = Ins.GetRecent(RecentOptions.Rec3);
            }
            else
            {
                Recent3.Visibility = Visibility.Hidden;
            }

            if (Ins.GetRecent(RecentOptions.Rec4) != null)
            {
                Recent4.Visibility = Visibility.Visible;
                Session4.Content = Ins.RecentName(RecentOptions.Rec4);
                Session4Path.Content = Ins.GetRecent(RecentOptions.Rec4);
            }
            else
            {
                Recent4.Visibility = Visibility.Hidden;
            }

            if (Ins.GetRecent(RecentOptions.Rec5) != null)
            {
                Recent5.Visibility = Visibility.Visible;
                Session5.Content = Ins.RecentName(RecentOptions.Rec5);
                Session5Path.Content = Ins.GetRecent(RecentOptions.Rec5);
            }
            else
            {
                Recent5.Visibility = Visibility.Hidden;
            }
        }
    }
}
