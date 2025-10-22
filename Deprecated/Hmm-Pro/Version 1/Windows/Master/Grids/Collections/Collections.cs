using System.Windows;
using System.Windows.Controls;

using HmPro.Scripting.Files;
using HmPro.Scripting;
using HmPro.Scripting.Functions.Edit;

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

            TreeViewItem Selected = (TreeViewItem)macObjectTree.SelectedItem;

            Session File = new Session();
            Obj Object = File.GetObject(Lyseria.Fix.FixToName((string)Selected.Header));

            if (!Object || (Object.Type != ComponentTypes.MasterCollection && Object.Type != ComponentTypes.Collection))
            {
                MessageBox.Show("There was an internal error.\nThe selected item was not Collection or Master Collection, the only allowed literals.\nPress OK and the list will be refreshed.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);

                CloseEditor(EditorReg.AssociatedAction(Editors.ManageCollections));
                LoadEditor(Editors.ManageCollections);

                macDeleteCurrent.IsEnabled = false;
                macEditCurrent.IsEnabled = false;
                return;
            }

            if (Selected.Parent == macObjectTree)
            {
                File.Collections.Remove((Collection)Object);
            }
            else
            {
                Obj Parent = File.GetObject(Lyseria.Fix.FixToName((string)((TreeViewItem)Selected.Parent).Header));
                if (Parent == null || !Parent)
                {
                    MessageBox.Show("The logical parent to remove from does not exit.\nPress OK and the list will be refreshed.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);

                    CloseEditor(EditorReg.AssociatedAction(Editors.ManageCollections));
                    LoadEditor(Editors.ManageCollections);

                    macDeleteCurrent.IsEnabled = false;
                    macEditCurrent.IsEnabled = false;
                    return;
                }
                MasterCollection NParent = (MasterCollection)Parent;
                NParent.Collections.Remove((Collection)Object);
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

            TreeViewItem Selected = (TreeViewItem)macObjectTree.SelectedItem;
            string Header = (string)Selected.Header;

            Session File = new Session();
            Obj Object = File.GetObject(Lyseria.Fix.FixToName(Header));

            if (Object == null || !Object || (Object.Type != ComponentTypes.Collection && Object.Type != ComponentTypes.MasterCollection))
            {
                MessageBox.Show("There was an internal error.\nThe selected item was not Collection or Master Collection, the only allowed literals.\nPress OK and the list will be refreshed.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);

                CloseEditor(EditorReg.AssociatedAction(Editors.ManageCollections));
                LoadEditor(Editors.ManageCollections);

                macDeleteCurrent.IsEnabled = false;
                macEditCurrent.IsEnabled = false;
                return;
            }

            CollectionBase Collection;
            if (Object.Type == ComponentTypes.Collection)
            {
                Collection = (Collection)Object;
            }
            else
            {
                Collection = (MasterCollection)Object;
            }

            macObjTitle.Text = Collection.Title;
            macObjType.Text = Collection.ObjType.ToString();
            macType.Text = Collection is MasterCollection ? "Master Collection" : "Collection";
        }
    }
}
