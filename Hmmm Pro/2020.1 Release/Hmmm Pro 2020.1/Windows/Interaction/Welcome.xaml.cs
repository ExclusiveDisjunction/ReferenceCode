using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;
using HmPro.Files;
using Phosphor;

namespace HmPro.Windows.Interaction
{
    /// <summary>
    /// Interaction logic for Welcome.xaml
    /// </summary>
    public partial class Welcome : Window
    {
        public Welcome()
        {
            InitializeComponent();
        }

        public bool ShutDown = false;

        private void Window_Closed(object sender, EventArgs e)
        {
        }
        public void Window_ContentRendered(object sender, EventArgs e)
        {
            WelcomeGrid.Visibility = Visibility.Visible;
            Recalculate();
        }

        private void OpenFileCmd_Click(object sender, RoutedEventArgs e)
        {
            if (SessionControl.OpenFile()) this.Close();
        }
        private void OpenLastCmd_Click(object sender, RoutedEventArgs e) //Opens the last loaded session, using Ins.LastLoaded. Not used in this version.
        {
            if (System.IO.File.Exists(Ins.LastLoaded))
            {
                if (SessionControl.OpenFile(Ins.LastLoaded)) this.Close();
            }
            else
            {
                this.OpenLastCmd.IsEnabled = false;
            }
        }

        private void NewSessionWelcome_Click(object sender, RoutedEventArgs e) //Makes a new session.
        {
            if (SessionControl.NewFile()) this.Close();
        }

        private void RecentItem_Click(object sender, RoutedEventArgs e) //Loads the file in recent 5.
        {
            Button Item = sender as Button;
            string Path = "";

            if (Item == Recent1Bttn)
                Path = (string)Session1Path.Content;
            else if (Item == Recent2Bttn)
                Path = (string)Session2Path.Content;
            else if (Item == Recent3Bttn)
                Path = (string)Session3Path.Content;
            else if (Item == Recent4Bttn)
                Path = (string)Session4Path.Content;
            else if (Item == Recent5Bttn)
                Path = (string)Session5Path.Content;

            if (System.IO.File.Exists(Path))
            {
                Ins.CurrentLoaded = Path;
                Recent.AddRecent(Path);
                this.Close();
                return;
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");

                if (Item == Recent1Bttn)
                {
                    Recent.DeleteRecent(0);
                    Recalculate();
                }
                else if (Item == Recent2Bttn)
                {
                    Recent.DeleteRecent(1);
                    Recalculate();
                }
                else if (Item == Recent3Bttn)
                {
                    Recent.DeleteRecent(2);
                    Recalculate();
                }
                else if (Item == Recent4Bttn)
                {
                    Recent.DeleteRecent(3);
                    Recalculate();
                }
                else if (Item == Recent5Bttn)
                {
                    Recent.DeleteRecent(4);
                    Recalculate();
                }                
            }
        }
        private void RecentItem_MouseDown(object sender, MouseButtonEventArgs e)  //Loads the file in recent 1.
        {
            Grid Item = sender as Grid;
            string Path = "";

            if (Item == Recent1)
                Path = (string)Session1Path.Content;
            else if (Item == Recent2)
                Path = (string)Session2Path.Content;
            else if (Item == Recent3)
                Path = (string)Session3Path.Content;
            else if (Item == Recent4)
                Path = (string)Session4Path.Content;
            else if (Item == Recent5)
                Path = (string)Session5Path.Content;

            if (System.IO.File.Exists(Path))
            {
                Ins.CurrentLoaded = Path;
                this.Close();
            }
            else
            {
                MessageBox.Show($"Path '{Path}' does not exist on your computer. Please try again later.");
                if (Item == Recent1)
                {
                    Recent.DeleteRecent(1);
                    Recalculate();
                }
                else if (Item == Recent2)
                {
                    Recent.DeleteRecent(2);
                    Recalculate();
                }
                else if (Item == Recent3)
                {
                    Recent.DeleteRecent(3);
                    Recalculate();
                }
                else if (Item == Recent4)
                {
                    Recent.DeleteRecent(4);
                    Recalculate();
                }
                else if (Item == Recent5)
                {
                    Recent.DeleteRecent(4);
                    Recalculate();
                }
            }
        }

        private void Recalculate()
        {
            ShowWelcome.IsChecked = Ins.ShowWelcome;

            if (Recent.GetRecent(RecentOptions.Rec1) != null)
            {
                Recent1.Visibility = Visibility.Visible;
                Session1.Content = Recent.RecentName(RecentOptions.Rec1);
                Session1Path.Content = Recent.GetRecent(RecentOptions.Rec1);
            }
            else
            {
                Recent1.Visibility = Visibility.Hidden;
            }

            if (Recent.GetRecent(RecentOptions.Rec2) != null)
            {
                Recent2.Visibility = Visibility.Visible;
                Session2.Content = Recent.RecentName(RecentOptions.Rec2);
                Session2Path.Content = Recent.GetRecent(RecentOptions.Rec2);
            }
            else
            {
                Recent2.Visibility = Visibility.Hidden;
            }

            if (Recent.GetRecent(RecentOptions.Rec3) != null)
            {
                Recent3.Visibility = Visibility.Visible;
                Session3.Content = Recent.RecentName(RecentOptions.Rec3);
                Session3Path.Content = Recent.GetRecent(RecentOptions.Rec3);
            }
            else
            {
                Recent3.Visibility = Visibility.Hidden;
            }

            if (Recent.GetRecent(RecentOptions.Rec4) != null)
            {
                Recent4.Visibility = Visibility.Visible;
                Session4.Content = Recent.RecentName(RecentOptions.Rec4);
                Session4Path.Content = Recent.GetRecent(RecentOptions.Rec4);
            }
            else
            {
                Recent4.Visibility = Visibility.Hidden;
            }

            if (Recent.GetRecent(RecentOptions.Rec5) != null)
            {
                Recent5.Visibility = Visibility.Visible;
                Session5.Content = Recent.RecentName(RecentOptions.Rec5);
                Session5Path.Content = Recent.GetRecent(RecentOptions.Rec5);
            }
            else
            {
                Recent5.Visibility = Visibility.Hidden;
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

        private void ProcedeWithout_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void ShutDown_Click(object sender, RoutedEventArgs e)
        {
            this.ShutDown = true;
            this.Close();
        }
    }
}
