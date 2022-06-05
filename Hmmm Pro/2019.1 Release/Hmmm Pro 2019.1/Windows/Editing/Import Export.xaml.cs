using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using System.Diagnostics.CodeAnalysis;

using HmPro.Scripting.Files;

namespace HmPro.Windows.Editing
{
    /// <summary>
    /// Interaction logic for ImportExport.xaml
    /// </summary>
    public partial class ImportExport : Window
    {
        public ImportExport()
        {
            InitializeComponent();
        }

        private ImportExportOpt DrawMode = ImportExportOpt.Import; //The mode that the user requested.
        [SuppressMessage("Code Quality", "IDE0052:Remove unread private members", Justification = "Not used in this version, archetecuture in place.")]
        private Component Selected = null; //The object used in ImportExportOpt.ExportCurrent.
        private Session SelectedSes = null; //The session loaded to Import/Export From/To.

        public void Execute() //Sets the ImportExportOpt to Import and shows the window.
        {
            this.Show();
        }
        public void Execute(ImportExportOpt Option) //Used to set the specic ImportExportOpt.
        {
            DrawMode = Option;
            this.Show();
        }
        public void Execute(Component Obj) //Used to export an object. Not used in this version.
        {
            if (Obj == null || Obj.IsEmpty)
            {
                throw new NotSupportedException("The Obj must have a literal value.");
            }

            DrawMode = ImportExportOpt.ExportCurrent;
            Selected = Obj;
            this.Show();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) //Draws the window from the selected index.
        {
            Scripting.Ins.OpenWindows.Add(this);

            switch (DrawMode)
            {
                case ImportExportOpt.AllCollections:
                {
                    MainTab.SelectedIndex = 1;
                    break;
                }
                case ImportExportOpt.AllMemes:
                {
                    MainTab.SelectedIndex = 1;
                    break;
                }
                case ImportExportOpt.Export:
                {
                    MainTab.SelectedIndex = 1;
                    break;
                }
                case ImportExportOpt.ExportCurrent:
                {
                    MainTab.SelectedIndex = 1;
                    break;
                }
                case ImportExportOpt.Import:
                {
                    MainTab.SelectedIndex = 0;
                    break;
                }
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) //Removes the window from Ins.OpenWindows registry.
        {
            Scripting.Ins.OpenWindows.Remove(this);
        }

        #region ImportCmds
        private void imChooseFile_Click(object sender, RoutedEventArgs e) //Selects a file from a file path.
        {
            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog
            {
                AddExtension = true,
                DefaultExt = ".teh",
                Filter = "Session Files | *.teh",
                Multiselect = false,
                Title = "Select a file"
            };
            open.ShowDialog();
            string path = open.FileName;
            if (path == "")
            {
                return;
            }

            Session File;
            try
            {
                File = new Session(path);
            }
            catch
            {
                MessageBox.Show("This file cannot be loaded, as it is not the correct session file type. Please try again later.", "File Select:", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            SelectedSes = File;

            imPath.Content = path;
            Objects.Items.Clear();

            SolidColorBrush White = new SolidColorBrush(Color.FromArgb(255, 255, 255, 255));

            TreeViewItem FMemesHeader = new TreeViewItem
            {
                Header = "Favorite Memes",
                Foreground = White
            };
            Objects.Items.Add(FMemesHeader);
            TreeViewItem LMemesHeader = new TreeViewItem
            {
                Header = "Legendary Memes",
                Foreground = White
            };
            Objects.Items.Add(LMemesHeader);

            foreach (Collection Collection in File.Collections.Sort())
            {
                TreeViewItem item = new TreeViewItem
                {
                    Header = Collection.Title,
                    Foreground = White
                };
                Objects.Items.Add(item);
                foreach (Meme Meme in Collection.Memes.Sort())
                {
                    TreeViewItem meme = new TreeViewItem
                    {
                        Header = Meme.Title,
                        Foreground = White
                    };
                    item.Items.Add(meme);
                }
            }
            foreach (MasterCollection Collection in File.MasterCollections.Sort())
            {
                TreeViewItem item = new TreeViewItem
                {
                    Header = Collection.Title,
                    Foreground = White
                };
                Objects.Items.Add(item);
                foreach (Collection collection in Collection.Collections.Sort())
                {
                    TreeViewItem coll = new TreeViewItem
                    {
                        Header = collection.Title,
                        Foreground = White
                    };
                    item.Items.Add(coll);
                }
                foreach (Meme Meme in Collection.Memes.Sort())
                {
                    TreeViewItem meme = new TreeViewItem
                    {
                        Header = Meme.Title,
                        Foreground = White
                    };
                    item.Items.Add(meme);
                }
            }
            foreach (Meme Meme in File.FavoriteMemes.Sort())
            {
                TreeViewItem meme = new TreeViewItem
                {
                    Header = Meme.Title,
                    Foreground = White
                };
                FMemesHeader.Items.Add(meme);
            }
            foreach (Meme Meme in File.LegendaryMemes.Sort())
            {
                TreeViewItem meme = new TreeViewItem
                {
                    Header = Meme.Title,
                    Foreground = White
                };
                LMemesHeader.Items.Add(meme);
            }
        }

        private void imStart_Click(object sender, RoutedEventArgs e) //Starts importing items from the selected session and the setting mode.
        {
            Session debFile;
            try
            {
                debFile = new Session((string)imPath.Content);
            }
            catch
            {
                MessageBox.Show("This file cannot be loaded, as it is not the correct session file type. Please try again later.", "File Select:", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            SelectedSes = debFile;

            if (imAll.IsChecked == true)
            {
                Session File = new Session();

                foreach (Collection Collection in SelectedSes.Collections)
                {
                    Collection New = File.Collections.Add(Collection.ObjType);
                    New.Title = Collection.Title;
                    foreach (Meme Meme in Collection.Memes)
                    {
                        Meme NewMeme = New.Memes.Add(Meme.ObjType);
                        NewMeme.Title = Meme.Title;
                        NewMeme.Creator = Meme.Creator;
                        NewMeme.Description = Meme.Description;
                        if (Meme.ObjType == MemeTypes.Attachment)
                        {
                            NewMeme.Attachments = Meme.Attachments;
                        }
                        else if (Meme.ObjType == MemeTypes.Script)
                        {
                            foreach (Script Script in Meme.Scripts)
                            {
                                Script NewScript = Meme.Scripts.Add();
                                NewScript.Person = Script.Person;
                                NewScript.Position = Script.Position;
                                NewScript.Text = Script.Text;
                            }
                        }
                    }
                }
                foreach (MasterCollection Collection in SelectedSes.MasterCollections)
                {
                    MasterCollection New = File.MasterCollections.Add();
                    New.Title = Collection.Title;
                    foreach (Collection Sub in Collection.Collections)
                    {
                        Collection NewCollection = New.Collections.Add(Sub.ObjType);
                        NewCollection.Title = Sub.Title;
                    }
                    foreach (Meme Meme in Collection.Memes)
                    {
                        Meme NewMeme = New.Memes.Add(Meme.ObjType);
                        NewMeme.Title = Meme.Title;
                        NewMeme.Creator = Meme.Creator;
                        NewMeme.Description = Meme.Description;
                        if (Meme.ObjType == MemeTypes.Attachment)
                        {
                            NewMeme.Attachments = Meme.Attachments;
                        }
                        else if (Meme.ObjType == MemeTypes.Script)
                        {
                            foreach (Script Script in Meme.Scripts)
                            {
                                Script NewScript = Meme.Scripts.Add();
                                NewScript.Person = Script.Person;
                                NewScript.Position = Script.Position;
                                NewScript.Text = Script.Text;
                            }
                        }
                    }
                }
                foreach (Meme Meme in SelectedSes.FavoriteMemes)
                {
                    Meme NewMeme = File.FavoriteMemes.Add(Meme.ObjType);
                    NewMeme.Title = Meme.Title;
                    NewMeme.Creator = Meme.Creator;
                    NewMeme.Description = Meme.Description;
                    if (Meme.ObjType == MemeTypes.Attachment)
                    {
                        NewMeme.Attachments = Meme.Attachments;
                    }
                    else if (Meme.ObjType == MemeTypes.Script)
                    {
                        foreach (Script Script in Meme.Scripts)
                        {
                            Script NewScript = Meme.Scripts.Add();
                            NewScript.Person = Script.Person;
                            NewScript.Position = Script.Position;
                            NewScript.Text = Script.Text;
                        }
                    }
                }
                foreach (Meme Meme in SelectedSes.LegendaryMemes)
                {
                    Meme NewMeme = File.LegendaryMemes.Add(Meme.ObjType);
                    NewMeme.Title = Meme.Title;
                    NewMeme.Creator = Meme.Creator;
                    NewMeme.Description = Meme.Description;
                    if (Meme.ObjType == MemeTypes.Attachment)
                    {
                        NewMeme.Attachments = Meme.Attachments;
                    }
                    else if (Meme.ObjType == MemeTypes.Script)
                    {
                        foreach (Script Script in Meme.Scripts)
                        {
                            Script NewScript = Meme.Scripts.Add();
                            NewScript.Person = Script.Person;
                            NewScript.Position = Script.Position;
                            NewScript.Text = Script.Text;
                        }
                    }
                }
            }
            else if (imMemes.IsChecked == true)
            {
                Session File = new Session();

                foreach (Meme Meme in SelectedSes.FavoriteMemes)
                {
                    Meme NewMeme = File.FavoriteMemes.Add(Meme.ObjType);
                    NewMeme.Title = Meme.Title;
                    NewMeme.Creator = Meme.Creator;
                    NewMeme.Description = Meme.Description;
                    if (Meme.ObjType == MemeTypes.Attachment)
                    {
                        NewMeme.Attachments = Meme.Attachments;
                    }
                    else if (Meme.ObjType == MemeTypes.Script)
                    {
                        foreach (Script Script in Meme.Scripts)
                        {
                            Script NewScript = Meme.Scripts.Add();
                            NewScript.Person = Script.Person;
                            NewScript.Position = Script.Position;
                            NewScript.Text = Script.Text;
                        }
                    }
                }
                foreach (Meme Meme in SelectedSes.LegendaryMemes)
                {
                    Meme NewMeme = File.LegendaryMemes.Add(Meme.ObjType);
                    NewMeme.Title = Meme.Title;
                    NewMeme.Creator = Meme.Creator;
                    NewMeme.Description = Meme.Description;
                    if (Meme.ObjType == MemeTypes.Attachment)
                    {
                        NewMeme.Attachments = Meme.Attachments;
                    }
                    else if (Meme.ObjType == MemeTypes.Script)
                    {
                        foreach (Script Script in Meme.Scripts)
                        {
                            Script NewScript = Meme.Scripts.Add();
                            NewScript.Person = Script.Person;
                            NewScript.Position = Script.Position;
                            NewScript.Text = Script.Text;
                        }
                    }
                }
            }
            else if (imCollections.IsChecked == true)
            {
                Session File = new Session();

                foreach (Collection Collection in SelectedSes.Collections)
                {
                    Collection New = File.Collections.Add(Collection.ObjType);
                    New.Title = Collection.Title;
                    foreach (Meme Meme in Collection.Memes)
                    {
                        Meme NewMeme = New.Memes.Add(Meme.ObjType);
                        NewMeme.Title = Meme.Title;
                        NewMeme.Creator = Meme.Creator;
                        NewMeme.Description = Meme.Description;
                        if (Meme.ObjType == MemeTypes.Attachment)
                        {
                            NewMeme.Attachments = Meme.Attachments;
                        }
                        else if (Meme.ObjType == MemeTypes.Script)
                        {
                            foreach (Script Script in Meme.Scripts)
                            {
                                Script NewScript = Meme.Scripts.Add();
                                NewScript.Person = Script.Person;
                                NewScript.Position = Script.Position;
                                NewScript.Text = Script.Text;
                            }
                        }
                    }
                }
                foreach (MasterCollection Collection in SelectedSes.MasterCollections)
                {
                    MasterCollection New = File.MasterCollections.Add();
                    New.Title = Collection.Title;
                    foreach (Collection Sub in Collection.Collections)
                    {
                        Collection NewCollection = New.Collections.Add(Sub.ObjType);
                        NewCollection.Title = Sub.Title;
                    }
                    foreach (Meme Meme in Collection.Memes)
                    {
                        Meme NewMeme = New.Memes.Add(Meme.ObjType);
                        NewMeme.Title = Meme.Title;
                        NewMeme.Creator = Meme.Creator;
                        NewMeme.Description = Meme.Description;
                        if (Meme.ObjType == MemeTypes.Attachment)
                        {
                            NewMeme.Attachments = Meme.Attachments;
                        }
                        else if (Meme.ObjType == MemeTypes.Script)
                        {
                            foreach (Script Script in Meme.Scripts)
                            {
                                Script NewScript = Meme.Scripts.Add();
                                NewScript.Person = Script.Person;
                                NewScript.Position = Script.Position;
                                NewScript.Text = Script.Text;
                            }
                        }
                    }
                }
            }
            else if (imCollectionsEXMeme.IsChecked == true)
            {
                Session File = new Session();

                foreach (Collection Collection in SelectedSes.Collections)
                {
                    Collection New = File.Collections.Add(Collection.ObjType);
                    New.Title = Collection.Title;
                }
                foreach (MasterCollection Collection in SelectedSes.MasterCollections)
                {
                    MasterCollection New = File.MasterCollections.Add();
                    New.Title = Collection.Title;
                    foreach (Collection Sub in Collection.Collections)
                    {
                        Collection NewCollection = New.Collections.Add(Sub.ObjType);
                        NewCollection.Title = Sub.Title;
                    }
                }
            }

            this.Close();
        }
        private void Cancel_Click(object sender, RoutedEventArgs e) //Closes the window.
        {
            this.Close();
        }

        #endregion
    }
}
