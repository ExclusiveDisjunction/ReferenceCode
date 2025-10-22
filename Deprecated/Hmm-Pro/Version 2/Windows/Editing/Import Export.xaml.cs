using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using System.Diagnostics.CodeAnalysis;

using HmPro.Files;
using Phosphor;
using HmPro.Edit;

namespace HmPro.Windows.Editing
{
    /// <summary>
    /// Interaction logic for ImportExport.xaml
    /// </summary>
    public partial class ImportExport : Window
    {
        private ImportExport()
        {
            InitializeComponent();
        }

        private ImportExportOpt DrawMode = ImportExportOpt.Import; //The mode that the user requested.
        [SuppressMessage("Code Quality", "IDE0052:Remove unread private members", Justification = "Not used in this version, archetecuture in place.")]
        private IHSession SelectedFile = null; //The session loaded to Import/Export From/To.

        public static void Execute() //Sets the ImportExportOpt to Import and shows the window.
        {
            ImportExport This = new ImportExport();
            This.ShowDialog();
        }
        public static void Execute(ImportExportOpt Option) //Used to set the specic ImportExportOpt.
        {
            ImportExport This = new ImportExport
            {
                DrawMode = Option
            };
            This.Show();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) //Draws the window from the selected index.
        {
            Ins.OpenWindows.Add(this);

            switch (DrawMode)
            {
                case ImportExportOpt.Export:
                    MainTab.SelectedIndex = 1;
                    break;
                case ImportExportOpt.Import:
                    MainTab.SelectedIndex = 0;
                    break;
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) //Removes the window from Ins.OpenWindows registry.
        {
            Ins.OpenWindows.Remove(this);
        }

        #region ImportCmds
        private void imChooseFile_Click(object sender, RoutedEventArgs e) //Selects a file from a file path.
        {
            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog
            {
                AddExtension = true,
                DefaultExt = ".teh",
                Filter = "Session Files|*.teh|Segment Files|*.tehseg",
                Multiselect = false,
                Title = "Select a file"
            };
            open.ShowDialog();
            string path = open.FileName;
            if (path == "")
                return;

            IHSession File;
            if (FileTools.FileExt(path, false) == "teh")
                File = new Session(path);
            else if (FileTools.FileExt(path, false) == "tehseg")
                File = new Segment(path);
            else
                return;

            SelectedFile = File;

            imPath.Text = path;
            Objects.Items.Clear();

            Objects.Items.Add(SessionBridge.GetAllObjects(path));
        }

        private enum imMode { AllObjects, AllMemes, AllCollections, AllCollectionsWoMemes }
        private void imStart_Click(object sender, RoutedEventArgs e) //Starts importing items from the selected session and the setting mode.
        {
            imMode Mode;
            if (imAll.IsChecked == true) Mode = imMode.AllObjects;
            else if (imMemes.IsChecked == true) Mode = imMode.AllMemes;
            else if (imCollections.IsChecked == true) Mode = imMode.AllCollections;
            else if (imCollectionsEXMeme.IsChecked == true) Mode = imMode.AllCollectionsWoMemes;
            else return;

            switch (Mode)
            {
                case imMode.AllObjects:
                {
                    if (Ins.LoadedSession is Segment && SelectedFile is Segment)
                    {
                        Segment Current = Ins.LoadedSession as Segment;
                        Segment Other = SelectedFile as Segment;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Meme item in Other.Memes)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                        foreach (Attachment item in Other.Files)
                        {

                        }
                        foreach (Script item in Other.Scripts)
                        {

                        }
                    }
                    else if (Ins.LoadedSession is Session && SelectedFile is Session)
                    {
                        Session Current = Ins.LoadedSession as Session;
                        Session Other = SelectedFile as Session;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Meme item in Other.FavoriteMemes)
                        {

                        }
                        foreach (Meme item in Other.LegendaryMemes)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                        foreach (Attachment item in Other.Files)
                        {

                        }
                    }

                    else if (Ins.LoadedSession is Session && SelectedFile is Segment)
                    {
                        Session Current = Ins.LoadedSession as Session;
                        Segment Other = SelectedFile as Segment;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                        foreach (Attachment item in Other.Files)
                        {

                        }
                    }
                    else if (Ins.LoadedSession is Segment && SelectedFile is Session)
                    {
                        Segment Current = Ins.LoadedSession as Segment;
                        Session Other = SelectedFile as Session;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Meme item in Other.FavoriteMemes)
                        {

                        }
                        foreach (Meme item in Other.LegendaryMemes)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                        foreach (Attachment item in Other.Files)
                        {

                        }
                    }                    
                    break;
                }
                case imMode.AllMemes:
                {
                    if (Ins.LoadedSession is Segment && SelectedFile is Segment)
                    {
                        Segment Current = Ins.LoadedSession as Segment;
                        Segment Other = SelectedFile as Segment;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Meme item in Other.Memes)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                    }
                    else if (Ins.LoadedSession is Session && SelectedFile is Session)
                    {
                        Session Current = Ins.LoadedSession as Session;
                        Session Other = SelectedFile as Session;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Meme item in Other.FavoriteMemes)
                        {

                        }
                        foreach (Meme item in Other.LegendaryMemes)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                    }

                    else if (Ins.LoadedSession is Session && SelectedFile is Segment)
                    {
                        Session Current = Ins.LoadedSession as Session;
                        Segment Other = SelectedFile as Segment;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                    }
                    else if (Ins.LoadedSession is Segment && SelectedFile is Session)
                    {
                        Segment Current = Ins.LoadedSession as Segment;
                        Session Other = SelectedFile as Session;

                        foreach (Collection item in Other.Collections)
                        {

                        }
                        foreach (Meme item in Other.FavoriteMemes)
                        {
                            Meme NewM = Current.Memes.Add(item.Type);
                            NewM.Title = item.Title;
                            NewM.Creator = item.Creator;
                            NewM.Description = item.Description;

                            if (item.Type == MemeTypes.Script)
                            {
                                foreach (Script Script in item.Scripts)
                                {
                                    Script NewS = NewM.Scripts.Add();
                                    NewS.Person = Script.Person;
                                    NewS.Position = Script.Position;
                                    NewS.Text = Script.Text;
                                }
                            }
                            else if (item.Type == MemeTypes.Attachment)
                            {
                                foreach (Attachment Attach in item.Files)
                                {
                                    Attachment NewF = NewM.Files.Add();
                                    NewF.Path = Attach.Path;
                                }
                            }
                        }
                        foreach (Meme item in Other.LegendaryMemes)
                        {
                            Meme NewM = Current.Memes.Add(item.Type);
                            NewM.Title = item.Title;
                            NewM.Creator = item.Creator;
                            NewM.Description = item.Description;

                            if (item.Type == MemeTypes.Script)
                            {
                                foreach (Script Script in item.Scripts)
                                {
                                    Script NewS = NewM.Scripts.Add();
                                    NewS.Person = Script.Person;
                                    NewS.Position = Script.Position;
                                    NewS.Text = Script.Text;
                                }
                            }
                            else if (item.Type == MemeTypes.Attachment)
                            {
                                foreach (Attachment Attach in item.Files)
                                {
                                    Attachment NewF = NewM.Files.Add();
                                    NewF.Path = Attach.Path;
                                }
                            }
                        }
                        foreach (Bin item in Other.Bins)
                        {

                        }
                    }
                    break;
                }
                case imMode.AllCollections:
                {
                    IHSession Current = Ins.LoadedSession as Segment;
                    IHSession Other = SelectedFile as Segment;

                    foreach (Collection item in Other.Collections)
                    {
                        Collection New = Current.Collections.Add(item.Type);
                        New.Title = item.Title;
                        New.Creator = item.Creator;
                        New.Description = item.Description;

                        if (item.Type == CollectionTypes.Master)
                        {
                            foreach (Collection sub in item.Collections)
                            {
                                Collection thing = New.Collections.Add(sub.Type);
                                thing.Title = sub.Title;
                                thing.Creator = sub.Creator;
                                thing.Description = sub.Description;

                                foreach (Meme meme in item.Memes)
                                {
                                    Meme NewM = New.Memes.Add(meme.Type);
                                    NewM.Title = meme.Title;
                                    NewM.Creator = meme.Creator;
                                    NewM.Description = meme.Description;

                                    if (meme.Type == MemeTypes.Script)
                                    {
                                        foreach (Script Script in meme.Scripts)
                                        {
                                            Script NewS = NewM.Scripts.Add();
                                            NewS.Person = Script.Person;
                                            NewS.Position = Script.Position;
                                            NewS.Text = Script.Text;
                                        }
                                    }
                                    else if (meme.Type == MemeTypes.Attachment)
                                    {
                                        foreach (Attachment Attach in meme.Files)
                                        {
                                            Attachment NewF = NewM.Files.Add();
                                            NewF.Path = Attach.Path;
                                        }
                                    }
                                }
                            }
                        }

                        foreach (Meme meme in item.Memes)
                        {
                            Meme NewM = New.Memes.Add(meme.Type);
                            NewM.Title = meme.Title;
                            NewM.Creator = meme.Creator;
                            NewM.Description = meme.Description;

                            if (meme.Type == MemeTypes.Script)
                            {
                                foreach (Script Script in meme.Scripts)
                                {
                                    Script NewS = NewM.Scripts.Add();
                                    NewS.Person = Script.Person;
                                    NewS.Position = Script.Position;
                                    NewS.Text = Script.Text;
                                }
                            }
                            else if (meme.Type == MemeTypes.Attachment)
                            {
                                foreach (Attachment Attach in meme.Files)
                                {
                                    Attachment NewF = NewM.Files.Add();
                                    NewF.Path = Attach.Path;
                                }
                            }
                        }
                    }
                    break;
                }
                case imMode.AllCollectionsWoMemes:
                {
                    IHSession Current = Ins.LoadedSession as Segment;
                    IHSession Other = SelectedFile as Segment;

                    foreach (Collection item in Other.Collections)
                    {
                        Collection New = Current.Collections.Add(item.Type);
                        New.Title = item.Title;
                        New.Creator = item.Creator;
                        New.Description = item.Description;

                        if (item.Type == CollectionTypes.Master)
                        {
                            foreach (Collection sub in item.Collections)
                            {
                                Collection thing = New.Collections.Add(sub.Type);
                                thing.Title = sub.Title;
                                thing.Creator = sub.Creator;
                                thing.Description = sub.Description;
                            }
                        }
                    }
                    break;
                }

            }
        }
        private void Cancel_Click(object sender, RoutedEventArgs e) //Closes the window.
        {
            this.Close();
        }

        #endregion
    }
}
