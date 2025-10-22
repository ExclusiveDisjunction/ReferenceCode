using System;
using System.Windows.Controls;
using System.Windows;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void LoadInAttachment(object sender, RoutedEventArgs e) //Opens a file dialog to select a file, and sets the value of one of the attachments, depending on the sender.
        {
            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "All Files | *.*",
                Multiselect = false,
                RestoreDirectory = true,
                Title = "Please select a file to attach to the meme."
            };

            open.ShowDialog();
            string path = open.FileName;

            if (path == Ins.CurrentLoaded)
            {
                MessageBox.Show("The attachment cannot address it's self. Please try again later.", "Attachments Invalid:", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (sender is Button button)
            {
                switch (button.Name)
                {
                    case "admAtt1Pick":
                        admAtt1.Text = path;
                        break;
                    case "admAtt2Pick":
                        admAtt2.Text = path;
                        break;
                    case "admAtt3Pick":
                        admAtt3.Text = path;
                        break;
                    case "admAtt4Pick":
                        admAtt4.Text = path;
                        break;
                    case "admAtt5Pick":
                        admAtt5.Text = path;
                        break;
                    default:
                        throw new ArgumentException("The sender is not known. This function is ment to be sent to an attachment picker.");
                }
            }
            else
            {
                throw new ArgumentException("The sender is not known. This function is ment to be sent to an attachment picker.");
            }
        }
    }
}
