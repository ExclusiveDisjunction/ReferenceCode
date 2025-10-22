using System.Windows;
using System.Windows.Input;

using HmPro.Scripting;

namespace HmPro.Windows
{
    public partial class Main
    {     
        private void OpenLastCmd_Click(object sender, RoutedEventArgs e) //Opens the last loaded session, using Ins.LastLoaded. Not used in this version.
        {
            if (System.IO.File.Exists(Ins.LastLoaded))
            {
                this.OpenFromPath(Ins.LastLoaded);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                this.OpenLastCmd.IsEnabled = false;
            }
        }
        private void CloseWelcome_Click(object sender, RoutedEventArgs e) //Closes the welcome grid.
        {
            WelcomeGrid.Visibility = Visibility.Collapsed;
        }

        private void NewFromTemplateCmd_Click(object sender, RoutedEventArgs e) //Makes a new file from a pre existing template. Being removed in the new version.
        {
            MessageBox.Show("Sorry, this feature is not included in Hmmm Pro 2019.1, and will be removed in the next release.", "Sorry:", MessageBoxButton.OK, MessageBoxImage.Error);
        }
        private void NewSessionWelcome_Click(object sender, RoutedEventArgs e) //Makes a new session.
        {
            NewSessionCmd_Click(sender, e);
            WelcomeGrid.Visibility = Visibility.Collapsed;
        }

        private void Recent1_Click(object sender, RoutedEventArgs e) //Loads the file in recent 1.
        {
            string Path = (string)Session1Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent1.Visibility = Visibility.Hidden;
                Session1.IsEnabled = false;
                Session1Path.IsEnabled = false;
            }
        }
        private void Recent2_Click(object sender, RoutedEventArgs e) //Loads the file in recent 2.
        {
            string Path = (string)Session2Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent2.Visibility = Visibility.Hidden;
                Session2.IsEnabled = false;
                Session2Path.IsEnabled = false;
            }
        }
        private void Recent3_Click(object sender, RoutedEventArgs e) //Loads the file in recent 3.
        {
            string Path = (string)Session3Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent3.Visibility = Visibility.Hidden;
                Session3.IsEnabled = false;
                Session3Path.IsEnabled = false;
            }
        }
        private void Recent4_Click(object sender, RoutedEventArgs e) //Loads the file in recent 4.
        {
            string Path = (string)Session4Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent4.Visibility = Visibility.Hidden;
                Session4.IsEnabled = false;
                Session4Path.IsEnabled = false;
            }
        }
        private void Recent5_Click(object sender, RoutedEventArgs e) //Loads the file in recent 5.
        {
            string Path = (string)Session5Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent5.Visibility = Visibility.Hidden;
                Session5.IsEnabled = false;
                Session5Path.IsEnabled = false;
            }
        }

        private void Recent1_MouseDown(object sender, MouseButtonEventArgs e)  //Loads the file in recent 1.
        {
            string Path = (string)Session1Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent1.Visibility = Visibility.Hidden;
                Session1.IsEnabled = false;
                Session1Path.IsEnabled = false;
            }
        }
        private void Recent2_MouseDown(object sender, MouseButtonEventArgs e)  //Loads the file in recent 2.
        {
            string Path = (string)Session2Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent2.Visibility = Visibility.Hidden;
                Session2.IsEnabled = false;
                Session2Path.IsEnabled = false;
            }
        }
        private void Recent3_MouseDown(object sender, MouseButtonEventArgs e)  //Loads the file in recent 3.
        {
            string Path = (string)Session3Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent3.Visibility = Visibility.Hidden;
                Session3.IsEnabled = false;
                Session3Path.IsEnabled = false;
            }
        }
        private void Recent4_MouseDown(object sender, MouseButtonEventArgs e)  //Loads the file in recent 4.
        {
            string Path = (string)Session4Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent4.Visibility = Visibility.Hidden;
                Session4.IsEnabled = false;
                Session4Path.IsEnabled = false;
            }
        }
        private void Recent5_MouseDown(object sender, MouseButtonEventArgs e)  //Loads the file in recent 5.
        {
            string Path = (string)Session5Path.Content;
            if (System.IO.File.Exists(Path))
            {
                OpenFromPath(Path);
                WelcomeGrid.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                Recent5.Visibility = Visibility.Hidden;
                Session5.IsEnabled = false;
                Session5Path.IsEnabled = false;
            }
        }

        private void ShowWelcome_Checked(object sedner, RoutedEventArgs e) //Sets Ins.ShowWelcome to true.
        {
            Ins.ShowWelcome = true;
        }
        private void ShowWelcome_Unchecked(object sender, RoutedEventArgs e) //Sets Ins.ShowWelcome to false.
        {
            Ins.ShowWelcome = false;
        }
    }
}
