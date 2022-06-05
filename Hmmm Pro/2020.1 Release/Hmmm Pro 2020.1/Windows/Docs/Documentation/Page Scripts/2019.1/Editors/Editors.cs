using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HmPro.Windows.Docs
{
    public partial class Documentation
    {
        private void EditorsAndActions(object sender, RoutedEventArgs e)
        {
            string Title = itTitle.Text;
            if (Title == "Add Collection" || Title == "Add Meme" || Title == "All Objects" || Title == "Basic Editor" || Title == "Current File" || Title == "Manage Collections" || Title == "Manage Memes" || Title == "Settings")
            {
                return;
            }

            itTitle.Text = "Editors & Actions";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions.tehst");
        }

        private void AddCollection(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Add Collection";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Add Collection.tehst");
        }
        private void AddMeme(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Add Meme";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Add Meme.tehst");
        }
        private void AllObjects(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "All Objects";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\All Objects.tehst");
        }
        private void BasicEditor(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Basic Editor";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Basic Editor.tehst");
        }
        private void CurrentFile(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Current File";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Current File.tehst");
        }
        private void ManageCollections(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Manage Collections";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Manage Collections.tehst");
        }
        private void ManageMemes(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Manage Memes";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Manage Memes.tehst");
        }
        private void Settings(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Settings";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Editors and Actions\\Settings.tehst");
        }
    }
}
