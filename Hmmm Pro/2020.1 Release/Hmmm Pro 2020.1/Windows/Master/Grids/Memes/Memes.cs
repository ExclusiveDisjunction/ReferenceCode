using System.Windows;
using System.Windows.Controls;

using HmPro.Files;
using HmPro.Registry;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void mamObjectTree_ItemSelected(object sender, RoutedEventArgs e) //Pulls up the information about a meme in from the selected object in the mamObjectTree.
        {
            if (!Ins.IsLoaded)
            {
                CloseAllEditors();
                Lock();
                return;
            }

            if (mamObjectTree.SelectedItem == null || !(macObjectTree.SelectedItem is TreeViewItem))
            {
                mamEditCurrent.IsEnabled = false;
                return;
            }

            TreeViewItem Selected = (TreeViewItem)mamObjectTree.SelectedItem;
            string Header = (string)Selected.Header;

            Session File = new Session(Ins.CurrentLoaded);
            IComponent[] Objs = File.GetMeme(Header);
            IComponent Obj = Objs.Length == 0 ? null : Objs[0];

            if (Obj == null || Obj.IsEmpty || (Obj.ObjectType != ComponentTypes.Meme))
            {
                MessageBox.Show("There was an internal error.\nThe selected item was not Collection or Master Collection, the only allowed literals.\nPress OK and the list will be refreshed.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);

                CloseEditor(EditorReg.AssociatedAction(Editors.ManageCollections));
                LoadEditor(Editors.ManageCollections);

                mamEditCurrent.IsEnabled = false;
                return;
            }

            Meme Meme = (Meme)Obj;
            mamObjTitle.Text = Meme.Title;
            mamObjType.Text = "Meme";
            mamType.Text = Meme.Type == MemeTypes.Attachment ? "Attachment" : Meme.Type == MemeTypes.Script ? "Script" : "Standard";
            mamCreator.Text = Meme.Creator;
            mamDescription.Text = Meme.Description;
        }

        private void mamAddNew_Click(object sender, RoutedEventArgs e) //Loads the Add Meme editor.
        {
            LoadEditor(Editors.AddMeme);
        }
        private void mamEditCurrent_Click(object sender, RoutedEventArgs e) //Loads the basic editor on the selected object.
        {
            if (!Ins.IsLoaded)
            {
                CloseAllEditors();
                Lock();
                return;
            }

            if (mamObjectTree.SelectedItem == null || !(macObjectTree.SelectedItem is TreeViewItem))
            {
                mamEditCurrent.IsEnabled = false;
                return;
            }

            TreeViewItem Selected = (TreeViewItem)mamObjectTree.SelectedItem;
            string Header = (string)Selected.Header;

            Session File = new Session(Ins.CurrentLoaded);
            Meme[] Obj = File.GetMeme(Header);

            if (Obj == null || Obj[0].IsEmpty || (Obj[0].ObjectType != ComponentTypes.Meme))
            {
                MessageBox.Show("There was an internal error.\nThe selected item was not Collection or Master Collection, the only allowed literals.\nPress OK and the list will be refreshed.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);

                CloseEditor(EditorReg.AssociatedAction(Editors.ManageCollections));
                LoadEditor(Editors.ManageCollections);

                mamEditCurrent.IsEnabled = false;
                return;
            }

            baeExecute(Obj[0]);
        }
    }
}
