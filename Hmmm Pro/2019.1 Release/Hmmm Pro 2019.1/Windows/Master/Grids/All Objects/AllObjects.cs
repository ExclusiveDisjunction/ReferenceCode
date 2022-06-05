using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using HmPro.Scripting.Files;
using HmPro.Scripting;
using Lyseria;

using static HmPro.Scripting.Ins;
using static Lyseria.Fix;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void allAddChild_Click(object sender, RoutedEventArgs e) //Adds a child to the selected item of its corrisponding type.
        {
            TreeViewItem item = null;
            if (allObjectTree.SelectedItem != null && allObjectTree.SelectedItem is TreeViewItem)
                item = (TreeViewItem)allObjectTree.SelectedItem ?? null;

            if (item == null)
            {
                allObjTitle.Text = "*NoInfo*";
                allObjType.Text = "*NoInfo*";
                allType.Text = "*NoInfo*";
                allCreator.Text = "*NoInfo*";
                allDescription.Text = "*NoInfo*";
                allChildrenCount.Text = "*NoInfo*";
                return;
            }

            Session File = new Session();

            if ((string)item.Header == "Collections" || (string)item.Header == "Memes")
                return;
            if ((string)item.Header == "Favorite Memes" || (string)item.Header == "Legendary Memes")
            {
                Meme Item = (string)item.Header == "Favorite Memes" ? File.FavoriteMemes.Add(MemeTypes.Standard) : File.LegendaryMemes.Add(MemeTypes.Standard);
                baeExecute(Item);
                return;
            }

            string Header = "";
            if (item.Header is string)
                Header = (string)item.Header;
            Obj Selected = File.GetObject(Lyseria.Fix.FixToName(Header));

            switch (Selected.Type)
            {
                case ComponentTypes.Collection:
                {
                    Collection Collection = (Collection)Selected;

                    Meme New = Collection.Memes.Add(MemeTypes.Standard);
                    baeExecute(New);
                    break;
                }
                case ComponentTypes.MasterCollection:
                {
                    MasterCollection MsCollection = (MasterCollection)Selected;

                    MessageBoxResult Result = MessageBox.Show("Do you want to add a meme? (Collection will be added if not).", "Add child:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                    if (Result == MessageBoxResult.Cancel || Result == MessageBoxResult.None)
                        return;
                    else if (Result == MessageBoxResult.No)
                    {
                        Collection New = MsCollection.Collections.Add(CollectionTypes.Standard);
                        baeExecute(New);
                        return;
                    }
                    else if (Result == MessageBoxResult.Yes)
                    {
                        Meme New = MsCollection.Memes.Add(MemeTypes.Standard);
                        baeExecute(New);
                        return;
                    }
                    break;
                }
                case ComponentTypes.Meme:
                {
                    Meme Meme = (Meme)Selected;
                    if (Meme.ObjType != MemeTypes.Script)
                    {
                        MessageBox.Show("The meme cannot have children, as it is not a script meme.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                    Script New = Meme.Scripts.Add();
                    baeExecute(New);
                    break;
                }
                case ComponentTypes.Script:
                    MessageBox.Show("This object does not have logical children.", "Add child:", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
            }
        }
        private void allRemoveChild_Click(object sender, RoutedEventArgs e) //Removes the selected item from the session.
        {
            TreeViewItem item = (TreeViewItem)allObjectTree.SelectedItem ?? null;
            if (item == null || (string)item.Header == "Collections" || (string)item.Header == "Memes" || (string)item.Header == "Favorite Memes" || (string)item.Header == "Legendary Memes")
            {
                allRemove.IsEnabled = false;
                return;
            }
            Session File = new Session();
            Obj Obj = File.GetObject(FixToName((string)item.Header), true, true, true);

            if (Obj == null)
            {
                MessageBox.Show("The object cannot be removed.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                allRemove.IsEnabled = false;
                return;
            }

            Component Actual = Obj.ToType();

            switch (Actual.Type)
            {
                case ComponentTypes.Collection:
                    Collection Collection = (Collection)Actual;
                    if (Collection.FromMasterCollection)
                    {
                        foreach (MasterCollection collection in File.MasterCollections)
                        {
                            if (collection.Collections.Contains(Collection))
                            {
                                collection.Collections.Remove(Collection);
                                break;
                            }
                        }
                    }
                    else
                    {
                        File.Collections.Remove(Collection);
                        break;
                    }
                    break;
                case ComponentTypes.MasterCollection:
                    MasterCollection MCollection = (MasterCollection)Actual;
                    File.MasterCollections.Remove(MCollection);
                    return;
                case ComponentTypes.Meme:
                    Meme Meme = (Meme)Actual;
                    foreach (Collection collection in File.Collections)
                    {
                        if (collection.Memes.Contains(Meme))
                        {
                            collection.Memes.Remove(Meme);
                            break;
                        }
                    }
                    foreach (MasterCollection collection in File.MasterCollections)
                    {
                        if (collection.Memes.Contains(Meme))
                        {
                            collection.Memes.Remove(Meme);
                            break;
                        }
                    }
                    if (File.FavoriteMemes.Contains(Meme))
                    {
                        File.FavoriteMemes.Remove(Meme);
                        break;
                    }
                    if (File.LegendaryMemes.Contains(Meme))
                    {
                        File.LegendaryMemes.Remove(Meme);
                        break;
                    }
                    break;
                case ComponentTypes.Script:
                    Script Script = (Script)Actual;
                    foreach (Meme meme in File.Memes)
                    {
                        if (meme.ObjType == MemeTypes.Script && meme.Scripts.Contains(Script))
                        {
                            meme.Scripts.Remove(Script);
                            break;
                        }
                    }
                    break;
            }
            ShowEditor(Editors.AllObjects);
        }
        private void allRemoveAllChildren_Click(object sender, RoutedEventArgs e) //Removes all of the logical children from the selected item.
        {
            TreeViewItem item = null;
            if (allObjectTree.SelectedItem != null && allObjectTree.SelectedItem is TreeViewItem)
                item = (TreeViewItem)allObjectTree.SelectedItem;

            if (item == null)
            {
                MessageBox.Show("The action cannot continue, as the selected item does not exist in the current context.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                allRemoveAllChildren.IsEnabled = false;
                return;
            }

            Session File;
            try { File = new Session(); }
            catch
            {
                MessageBox.Show("The session cannot be loaded. Please try to open another session.", "Remove Children Failed:", MessageBoxButton.OK, MessageBoxImage.Error);
                CurrentLoaded = "";
                Lock();
                return;
            }

            string Header = "";
            if (item.Header is string)
                Header = (string)item.Header;

            if (Header == "Memes" || Header == "Collections")
                if (item.Parent != null && item.Parent != allObjectTree && item.Parent is TreeViewItem Parent)
                {
                    Header = (string)Parent.Header;
                    item = Parent;
                }
            Obj SelectObj = File.GetObject(FixToName(Header), Memes:true, Collections:true, MasterCollections:true);

           switch (SelectObj.Type)
           {
                case ComponentTypes.Collection:
                {
                    Collection Collection = (Collection)SelectObj;
                    foreach (Meme Meme in Collection.Memes)
                    {
                        Collection.Memes.Remove(Meme);
                    }
                    break;
                }
                case ComponentTypes.MasterCollection:
                {
                    MasterCollection Collection = (MasterCollection)SelectObj;
                    foreach (Meme Meme in Collection.Memes)
                    {
                        Collection.Memes.Remove(Meme);
                    }
                    foreach (Collection sCollection in Collection.Collections)
                    {
                        Collection.Collections.Remove(sCollection);
                    }
                    break;
                }
                case ComponentTypes.Meme:
                {
                    Meme Meme = (Meme)SelectObj;
                    if (Meme.ObjType != MemeTypes.Script)
                    {
                        MessageBox.Show("The current meme is not a script meme, the only type of meme with logical children.", "Remove all Children:", MessageBoxButton.OK, MessageBoxImage.Error);
                        allRemoveAllChildren.IsEnabled = false;
                        return;
                    }
                    else
                    {
                        foreach (Script Script in Meme.Scripts)
                        {
                            Meme.Scripts.Remove(Script);
                        }
                    }
                    break;
                }
                case ComponentTypes.Script:
                {
                    MessageBox.Show("Scripts do not have children, so there is nothing to remove.", "Remove all Children:", MessageBoxButton.OK, MessageBoxImage.Error);
                    allRemoveAllChildren.IsEnabled = false;
                    return;
                }
           }
        }
        private void allEditChild_Click(object sender, RoutedEventArgs e) //Loads the basic editor for the selected item.
        {
            TreeViewItem item = null;
            if (allObjectTree.SelectedItem != null && allObjectTree.SelectedItem is TreeViewItem)
                item = (TreeViewItem)allObjectTree.SelectedItem;

            if (item == null)
            {
                allObjTitle.Text = "*NoInfo*";
                allObjType.Text = "*NoInfo*";
                allType.Text = "*NoInfo*";
                allCreator.Text = "*NoInfo*";
                allDescription.Text = "*NoInfo*";
                allChildrenCount.Text = "*NoInfo*";
                return;
            }
            if ((string)item.Header == "Collections" || (string)item.Header == "Memes")
                return;

            Session File = new Session();

            string Header = "";
            if (item.Header is string)
                Header = (string)item.Header;
            Obj Selected = File.GetObject(FixToName(Header), Memes:true, Collections:true, MasterCollections:true);
            baeExecute(Selected.ToType());
        }
        private void allObjectTree_ItemSelected(object sender, RoutedEventArgs e) //Pulls up the information inside of the session from the key provided in the selected item's header.
        {
            if (allObjectTree.SelectedItem is null || !(allObjectTree.SelectedItem is TreeViewItem))
                return;

            TreeViewItem item = (TreeViewItem)allObjectTree.SelectedItem;
            if (item == null)
            {
                allObjTitle.Text = "*NoInfo*";
                allObjType.Text = "*NoInfo*";
                allType.Text = "*NoInfo*";
                allCreator.Text = "*NoInfo*";
                allDescription.Text = "*NoInfo*";
                allChildrenCount.Text = "*NoInfo*";
                return;
            }

            if ((string)item.Header == "Collections" || (string)item.Header == "Memes")
                return;

            Session File = new Session();

            if (item.Header is string FValue && FValue == "Favorite Memes")
            {
                allObjTitle.Text = "Favorite Memes";
                allObjType.Text = "*NoInfo*";
                allType.Text = "Session Component";
                allCreator.Text = "*NoInfo*";
                allDescription.Text = "This is the session's favorite memes.";
                allChildrenCount.Text = $"{File.FavoriteMemes.Count}";

                allAddChild.IsEnabled = true;
                allRemove.IsEnabled = false;
                allRemoveAllChildren.IsEnabled = File.FavoriteMemes.Count > 0 ? true : false;
                allEdit.IsEnabled = false;
            }
            else if (item.Header is string LValue && LValue == "Legendary Memes")
            {
                allObjTitle.Text = "Legendary Memes";
                allObjType.Text = "*NoInfo*";
                allType.Text = "Session Component";
                allCreator.Text = "*NoInfo*";
                allDescription.Text = "This is the session's legendary memes.";
                allChildrenCount.Text = $"{File.LegendaryMemes.Count}";

                allAddChild.IsEnabled = true;
                allRemove.IsEnabled = false;
                allRemoveAllChildren.IsEnabled = File.LegendaryMemes.Count > 0 ? true : false;
                allEdit.IsEnabled = false;
            }
            else
            {
                Obj obj = File.GetObject(FixToName((string)item.Header), Memes:true, Collections:true, MasterCollections:true);
                if (obj == null || obj.IsEmpty)
                {
                    allObjTitle.Text = "*NoInfo*";
                    allObjType.Text = "*NoInfo*";
                    allType.Text = "*NoInfo*";
                    allCreator.Text = "*NoInfo*";
                    allDescription.Text = "*NoInfo*";
                    allChildrenCount.Text = "*NoInfo*";

                    allAddChild.IsEnabled = false;
                    allRemove.IsEnabled = false;
                    allRemoveAllChildren.IsEnabled = false;
                    allEdit.IsEnabled = false;
                    return;
                }

                Component nObj = obj.ToType();
                switch (nObj.Type)
                {
                    case ComponentTypes.Collection:
                    {
                        Collection Collection = (Collection)nObj;
                        if (Collection == null || Collection.IsEmpty)
                        {
                            allType.Text = "!ERROR! 115";
                            allCreator.Text = "!ERROR! 115";
                            allDescription.Text = "!ERROR! 115";
                            allObjTitle.Text = "!ERROR! 115";
                            allObjType.Text = "!ERROR! 115";
                            allChildrenCount.Text = "!ERROR! 115";

                            allAddChild.IsEnabled = false;
                            allRemove.IsEnabled = true;
                            allRemoveAllChildren.IsEnabled = false;
                            allEdit.IsEnabled = false;
                            return;
                        }

                        allType.Text = "Collection";
                        allCreator.Text = "*NoInfo*";
                        allDescription.Text = "*NoInfo*";

                        allObjTitle.Text = Collection.Title;
                        allChildrenCount.Text = $"{Collection.Memes.Count}";

                        //If the type is master collection, then it has to return an error, as the type cannot be MasterCollection.
                        allObjType.Text = (Collection.ObjType == CollectionTypes.Master) ? "!ERROR! 113" : $"{Collection.ObjType} Collection";

                        allAddChild.IsEnabled = true;
                        allRemove.IsEnabled = true;
                        allRemoveAllChildren.IsEnabled = Collection.Memes.Count > 0 ? true : false;
                        allEdit.IsEnabled = true;
                        break;
                    }
                    case ComponentTypes.MasterCollection:
                    {
                        MasterCollection Collection = (MasterCollection)nObj ?? null;
                        if (Collection == null || Collection.IsEmpty)
                        {
                            allType.Text = "!ERROR! 115";
                            allCreator.Text = "!ERROR! 115";
                            allDescription.Text = "!ERROR! 115";
                            allObjTitle.Text = "!ERROR! 115";
                            allObjType.Text = "!ERROR! 115";
                            allChildrenCount.Text = "!ERROR! 115";

                            allAddChild.IsEnabled = false;
                            allRemove.IsEnabled = false;
                            allRemoveAllChildren.IsEnabled = false;
                            allEdit.IsEnabled = false;
                            return;
                        }

                        allType.Text = "Collection";
                        allCreator.Text = "*NoInfo*";
                        allDescription.Text = "*NoInfo*";

                        allObjTitle.Text = Collection.Title;
                        allChildrenCount.Text = $"{Collection.Memes.Count + Collection.Collections.Count}";
                        allObjType.Text = "Master Collection";

                        allAddChild.IsEnabled = true;
                        allRemove.IsEnabled = true;
                        allRemoveAllChildren.IsEnabled = Collection.HasMemes || Collection.HasCollections ? true : false;
                        allEdit.IsEnabled = true;
                        break;
                    }
                    case ComponentTypes.Meme:
                    {
                        Meme Meme = (Meme)nObj ?? null;
                        if (Meme == null || Meme.IsEmpty)
                        {
                            allType.Text = "!ERROR! 115";
                            allCreator.Text = "!ERROR! 115";
                            allDescription.Text = "!ERROR! 115";
                            allObjTitle.Text = "!ERROR! 115";
                            allObjType.Text = "!ERROR! 115";
                            allChildrenCount.Text = "!ERROR! 115";

                            allAddChild.IsEnabled = false;
                            allRemove.IsEnabled = false;
                            allRemoveAllChildren.IsEnabled = false;
                            allEdit.IsEnabled = false;
                            return;
                        }

                        allType.Text = "Meme";

                        allCreator.Text = Meme.Creator;
                        allDescription.Text = Meme.Description;
                        allObjTitle.Text = Meme.Title;
                        allObjType.Text = $"{Meme.ObjType} Meme";
                        allChildrenCount.Text = Meme.ObjType == MemeTypes.Script ? $"{Meme.Scripts.Count}" : "0";

                        allAddChild.IsEnabled = Meme.ObjType == MemeTypes.Script;
                        allRemove.IsEnabled = true;
                        allRemoveAllChildren.IsEnabled = Meme.ObjType == MemeTypes.Script && Meme.Scripts.Count > 0;
                        allEdit.IsEnabled = true;
                        break;
                    }
                    case ComponentTypes.Script:
                    {
                        Script Script = (Script)nObj ?? null;
                        if (Script == null || Script.IsEmpty)
                        {
                            allType.Text = "!ERROR! 115";
                            allCreator.Text = "!ERROR! 115";
                            allDescription.Text = "!ERROR! 115";
                            allObjTitle.Text = "!ERROR! 115";
                            allObjType.Text = "!ERROR! 115";
                            allChildrenCount.Text = "!ERROR! 115";

                            allAddChild.IsEnabled = false;
                            allRemove.IsEnabled = false;
                            allRemoveAllChildren.IsEnabled = false;
                            allEdit.IsEnabled = false;
                            return;
                        }

                        allType.Text = "Script";
                        allCreator.Text = Script.Parent.Creator;
                        allDescription.Text = Script.Parent.Scripts.ToString(false);
                        allObjTitle.Text = Script.Person ?? "!ERROR! 131";
                        allObjType.Text = "Script";
                        allChildrenCount.Text = "0";

                        allAddChild.IsEnabled = false;
                        allRemove.IsEnabled = true;
                        allRemoveAllChildren.IsEnabled = false;
                        allEdit.IsEnabled = true;
                        break;
                    }
                }
            }
        }

        private void AllObjects_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e) //Sets up the allObjectTree, wich shows all of the objects in the session.
        {
            allObjTitle.Text = "*NoInfo*";
            allObjType.Text = "*NoInfo*";
            allType.Text = "*NoInfo*";
            allCreator.Text = "*NoInfo*";
            allDescription.Text = "*NoInfo*";
            allChildrenCount.Text = "*NoInfo*";

            allAddChild.IsEnabled = false;
            allRemove.IsEnabled = false;
            allRemoveAllChildren.IsEnabled = false;
            allEdit.IsEnabled = false;

            if (AllObjects.Visibility == Visibility.Visible && Ins.IsLoaded)
            {
                allObjectTree.Items.Clear();

                Session File = new Session();
                SolidColorBrush White = new SolidColorBrush(Color.FromArgb(255, 255, 255, 255));

                TreeViewItem FavoriteMemes = new TreeViewItem
                {
                    Header = "Favorite Memes",
                    Name = "Favorite_Memes",
                    Foreground = White
                };
                TreeViewItem LegendaryMemes = new TreeViewItem
                {
                    Header = "Legendary Memes",
                    Name = "Legendary_Memes",
                    Foreground = White
                };
                allObjectTree.Items.Add(FavoriteMemes);
                allObjectTree.Items.Add(LegendaryMemes);

                foreach (Meme meme in File.FavoriteMemes)
                {
                    TreeViewItem item = new TreeViewItem
                    {
                        Header = meme.Title,
                        Name = meme.Name,
                        Foreground = White
                    };
                    FavoriteMemes.Items.Add(item);
                }
                foreach (Meme meme in File.LegendaryMemes)
                {
                    TreeViewItem item = new TreeViewItem
                    {
                        Header = meme.Title,
                        Name = meme.Name,
                        Foreground = White
                    };
                    LegendaryMemes.Items.Add(item);
                }

                foreach (Collection collection in File.Collections)
                {
                    TreeViewItem item = new TreeViewItem
                    {
                        Header = collection.Title,
                        Name = collection.Name,
                        Foreground = White
                    };
                    allObjectTree.Items.Add(item);
                    foreach (Meme meme in collection.Memes)
                    {
                        TreeViewItem Meme = new TreeViewItem
                        {
                            Header = meme.Title,
                            Name = meme.Name,
                            Foreground = White
                        };
                        item.Items.Add(Meme);
                    }
                }
                foreach (MasterCollection collection in File.MasterCollections)
                {
                    TreeViewItem item = new TreeViewItem
                    {
                        Header = collection.Title,
                        Name = collection.Name,
                        Foreground = White
                    };
                    allObjectTree.Items.Add(item);

                    TreeViewItem SubC = new TreeViewItem
                    {
                        Header = "Collections",
                        Foreground = White
                    };
                    TreeViewItem SubM = new TreeViewItem
                    {
                        Header = "Memes",
                        Foreground = White
                    };
                    item.Items.Add(SubC);
                    item.Items.Add(SubM);

                    foreach (Meme meme in collection.Memes)
                    {
                        TreeViewItem Meme = new TreeViewItem
                        {
                            Header = meme.Title,
                            Name = meme.Name,
                            Foreground = White
                        };
                        SubM.Items.Add(Meme);
                    }
                    foreach (Collection scollection in collection.Collections)
                    {
                        TreeViewItem Collection = new TreeViewItem
                        {
                            Header = scollection.Title,
                            Name = scollection.Name,
                            Foreground = White
                        };
                        SubC.Items.Add(Collection);
                    }
                }
            }
            else
            {
                allObjectTree.Items.Clear();
            }
        }
    }
}
