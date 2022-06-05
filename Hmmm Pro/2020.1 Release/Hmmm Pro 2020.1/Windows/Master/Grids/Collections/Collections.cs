using System.Windows;
using System.Windows.Controls;

using HmPro.Files;
using HmPro.Edit;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void macAddNew_Click(object sender, RoutedEventArgs e) //Loads the Add Collection editor.
        {
            LoadEditor(Editors.AddCollection);
        }
        private void macDeleteCurrent_Click(object sender, RoutedEventArgs e) //Deletes the currently selected collection.
        {
            if (!Ins.IsLoaded)
            {
                CloseAllEditors();
                Lock();
                return;
            }

            if (macObjectTree.SelectedItem == null || !(macObjectTree.SelectedItem is TreeViewItem))
            {
                macDeleteCurrent.IsEnabled = false;
            }

            
        }
        private void macEditCurrent_Click(object sender, RoutedEventArgs e) //Loads the Basic Editor on the selected collection.
        {

        }

        private void macObjectTree_ItemSelected(object sender, RoutedEventArgs e) //Pulls up the information about a specific collection selected in the macObjectTree.
        {
            if (!Ins.IsLoaded)
            {
                CloseAllEditors();
                Lock();
                return;
            }

            if (macObjectTree.SelectedItem == null || !(macObjectTree.SelectedItem is TreeViewItem))
            {
                macDeleteCurrent.IsEnabled = false;
                macEditCurrent.IsEnabled = false;
                return;
            }

            
        }
    }
}
