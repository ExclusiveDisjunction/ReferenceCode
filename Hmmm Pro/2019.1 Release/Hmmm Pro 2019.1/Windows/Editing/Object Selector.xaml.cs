using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using HmPro.Scripting.Files;
using HmPro.Scripting.Events;

namespace HmPro.Windows.Editing
{
    /// <summary>
    /// Interaction logic for Collection Connector.xaml
    /// </summary>
    public partial class ObjectSelector : Window
    {
        public ObjectSelector()
        {
            InitializeComponent();
        }

        private event EventHandler<ConnectEventArgs> Connected; //The event raised when the user returns a response.
        private bool Waiting = true; //Used in event waiting, it makes the scripts wait for a response from the form. Used in while(bool) loops.
        private Component Selected; //The selected item from the tree.        
        private string Notes = null;

        private bool MastersOnly = false;
        private bool MemesOnly = false;

        private bool Scripts = false; //Determies if the startup should show scripts.
        private bool Collections = true; //Sets it that collections are visible.
        private bool MasterCollections = true; //Sets it that mastercollections are visible.
        private bool Memes = false; //Determines if statup should show memes.

        private void Window_Loaded(object sender, RoutedEventArgs e) //Runs on startup.
        {
            Scripting.Ins.OpenWindows.Add(this);
            if (MastersOnly || MemesOnly)
            {
                VisualTree.Items.Remove(FavMemes);
                VisualTree.Items.Remove(LegMemes);
            }

            Session Session = new Session();
            SolidColorBrush White = new SolidColorBrush(Color.FromArgb(255, 255, 255, 255));
            
            if (MemesOnly)
            {
                foreach (Meme Meme in Session.Memes)
                {
                    TreeViewItem item = new TreeViewItem
                    {
                        Header = Meme.Title,
                        Foreground = White
                    };
                    VisualTree.Items.Add(item);
                }
            }

            else
            {
                if (!MastersOnly)
                {
                    foreach (Collection collection in Session.Collections)
                    {
                        TreeViewItem item = new TreeViewItem
                        {
                            Header = collection.Title,
                            Foreground = White
                        };
                        VisualTree.Items.Add(item);

                        if (Memes)
                        {
                            foreach (Meme meme in collection.Memes)
                            {
                                TreeViewItem Meme = new TreeViewItem()
                                {
                                    Header = meme.Title,
                                    Foreground = White
                                };
                                item.Items.Add(Meme);

                                if (Scripts)
                                {
                                    int Index = 1;
                                    foreach (Script script in meme.Scripts)
                                    {
                                        TreeViewItem Script = new TreeViewItem()
                                        {
                                            Header = $"Script {Index}",
                                            Foreground = White
                                        };
                                        Meme.Items.Add(Script);
                                        Index++;
                                    }
                                }
                            }
                        }
                    }
                }
                foreach (MasterCollection collection in Session.MasterCollections)
                {
                    TreeViewItem item = new TreeViewItem()
                    {
                        Header = collection.Title,
                        Foreground = White
                    };
                    VisualTree.Items.Add(item);

                    if (!MastersOnly)
                    {
                        foreach (Collection sCollection in collection.Collections)
                        {
                            TreeViewItem Collection = new TreeViewItem()
                            {
                                Header = sCollection.Title,
                                Foreground = White
                            };
                            item.Items.Add(Collection);
                        }
                    }

                    if (Memes)
                    {
                        foreach (Meme meme in collection.Memes)
                        {
                            TreeViewItem Meme = new TreeViewItem()
                            {
                                Header = meme.Title,
                                Foreground = White
                            };
                            item.Items.Add(Meme);

                            if (Scripts)
                            {
                                int Index = 1;
                                foreach (Script script in meme.Scripts)
                                {
                                    TreeViewItem Script = new TreeViewItem()
                                    {
                                        Header = $"Script {Index}",
                                        Foreground = White
                                    };
                                    Meme.Items.Add(Script);
                                    Index++;
                                }
                            }
                        }
                    }
                }
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Scripting.Ins.OpenWindows.Remove(this);
        }

        private void VisualTree_Selected(object sender, RoutedEventArgs e) //Sets the information on the form using the selected item from the tree.
        {
            if (!(VisualTree.SelectedItem != null && VisualTree.SelectedItem is TreeViewItem item && item.Header is string))
            {
                ObjTitle.Text = "";
                ObjType.Text = "";
                this.Selected = null;
                this.Notes = "";
            }

            TreeViewItem selected = (TreeViewItem)VisualTree.SelectedItem;

            string Name = Lyseria.Fix.FixToName((string)selected.Header);

            if (Name == "Legendary_Memes" || Name == "Favorite_Memes")
            {
                ObjTitle.Text = Name.Replace('_', ' ');
                ObjType.Text = "No Type";
                this.Selected = null;
                this.Notes = Name;
            }
            else
            {
                Session Session = new Session();
                Obj NewSelected = Session.GetObject(Name, Memes, Collections, MasterCollections);

                if (NewSelected == null)
                {
                    MessageBox.Show("The selected item does not exist in the session. (Was it removed?)", "Retrive Object:", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                switch (NewSelected.Type)
                {
                    case ComponentTypes.Collection | ComponentTypes.MasterCollection:
                    {
                        ObjType.Text = "Collection";
                        break;
                    }
                    case ComponentTypes.Meme:
                    {
                        ObjType.Text = "Meme";
                        break;
                    }
                    case ComponentTypes.Script:
                    {
                        ObjType.Text = "Script";
                        break;
                    }
                }

                if (NewSelected.Type != ComponentTypes.Script)
                {
                    ObjTitle.Text = NewSelected.Title;
                }
                else
                {
                    ObjTitle.Text = "Script: No Title";
                }

                this.Selected = NewSelected.ToType();
                this.Notes = null;
            }
        }

        private void Connect_Click(object Sender, RoutedEventArgs e) //Throws the Selected event with the selected object. and sets Waiting = false;
        {
            Waiting = false;
            ConnectEventArgs E = new ConnectEventArgs(Selected, true, new Session(), Notes);
            this.Connected?.Invoke(this, E);
        }
        private void CancelJob_Click(object sender, RoutedEventArgs e) //Throws the Selected event with a null object and sets Waiting = false;
        {
            Waiting = false;
            ConnectEventArgs E = new ConnectEventArgs(null, false, null);
            this.Connected?.Invoke(this, E);
        }

        /// <summary>
        /// Used to run the entire form, and returns the user's response. It is assumed that only collections will be showed.
        /// </summary>
        /// <returns>The eventargs of the button's click.</returns>
        public ConnectEventArgs Execute() //Use this to run the form.
        {
            this.Show();

            ConnectEventArgs Retun = null;

            Connected += (s, e) =>
            {
                Retun = e;
            };

            while (Waiting)
            {
                System.Windows.Forms.Application.DoEvents();
            }

            this.Close();
            return Retun;
        }
        public ConnectEventArgs Execute(bool Memes = false, bool Collections=true, bool MasterCollections=true, bool Scripts = false)
        {
            this.MastersOnly = Memes == false && Collections == false && MasterCollections == true;            
            this.MemesOnly = Memes == true && Collections == false && MasterCollections == false;

            this.Collections = Collections;
            this.MasterCollections = MasterCollections;
            this.Memes = Memes;
            this.Scripts = Scripts;

            this.Show();

            ConnectEventArgs Retun = null;

            Connected += (s, e) =>
            {
                Retun = e;
            };

            while (Waiting)
            {
                System.Windows.Forms.Application.DoEvents();
            }

            this.Close();
            return Retun;
        }
    }
}
