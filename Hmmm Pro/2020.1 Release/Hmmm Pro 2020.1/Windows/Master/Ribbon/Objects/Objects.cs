using System.Windows;
using HmPro.Windows.Editing;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void AddMemeCmd_Click(object sender, RoutedEventArgs e) //Loads the AddMeme editor.
        {
            LoadEditor(Editors.AddMeme);
        }
        private void AddCollectionCmd_Click(object sender, RoutedEventArgs e) //Load the AddCollection editor.
        {
            LoadEditor(Editors.AddCollection);
        }
        private void AddBinCmd_Click(object sender, RoutedEventArgs e)
        {
            LoadEditor(Editors.AddBin);
        }
        private void AddAttachmentCmd_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
