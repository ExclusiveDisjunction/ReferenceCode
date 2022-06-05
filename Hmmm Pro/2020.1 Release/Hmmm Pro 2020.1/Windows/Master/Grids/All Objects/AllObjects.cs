using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using HmPro.Files;
using Phosphor;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void allAddChild_Click(object sender, RoutedEventArgs e) //Adds a child to the selected item of its corrisponding type.
        {
            
        }
        private void allRemoveChild_Click(object sender, RoutedEventArgs e) //Removes the selected item from the session.
        {
            
        }
        private void allRemoveAllChildren_Click(object sender, RoutedEventArgs e) //Removes all of the logical children from the selected item.
        {
            
        }
        private void allEditChild_Click(object sender, RoutedEventArgs e) //Loads the basic editor for the selected item.
        {
            
        }
        private void allObjectTree_ItemSelected(object sender, RoutedEventArgs e) //Pulls up the information inside of the session from the key provided in the selected item's header.
        {

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

                Session File = new Session(Ins.CurrentLoaded);
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
                //Add thing for master colelctions
            }
            else
            {
                allObjectTree.Items.Clear();
            }
        }
    }
}
