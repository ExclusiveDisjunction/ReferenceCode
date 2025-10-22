using Lyseria;
using HmPro.Windows.Editing;
using HmPro.Scripting.Files;
using HmPro.Windows;

using System.Windows;
using System.IO;

namespace HmPro.Scripting.Functions
{
    public sealed class KeyboardCtrls
    {
        public KeyboardCtrls(Main Main) //Runs all keyboard shortcuts and uses Main for specific functions. Throws an error if Main is null.
        {
            this.Ms = Main ?? throw new System.ArgumentNullException("The Main provided was null. This is not allowed.");

            if (Keys.Ctrl && Keys.O)
            {
                Open();
                return;
            }
            if (Keys.Ctrl && Keys.N)
            {
                NewSession();
                return;
            }

            if (Ins.IsLoaded)
            {
                if (Keys.Ctrl && Keys.Shift && Keys.S)
                {
                    SaveAs();
                    return;
                }
                if (Keys.Ctrl && Keys.S)
                {
                    Save();
                    return;
                }

                if (Keys.Ctrl && Keys.Shift && Keys.A)
                {
                    AppendAll();
                    return;
                }
                if (Keys.Ctrl && Keys.A)
                {
                    Append();
                    return;
                }

                if (Keys.Ctrl && Keys.Shift && Keys.W)
                {
                    Close();
                    return;
                }
                if (Keys.Ctrl && Keys.W)
                {
                    CloseEditor();
                }

                if (Keys.Ctrl && Keys.A && Keys.M)
                {
                    AddMeme();
                    return;
                }
                if (Keys.Ctrl && Keys.M && Keys.E)
                {
                    ManageMemes();
                    return;
                }

                if (Keys.Ctrl && Keys.A && Keys.C)
                {
                    AddCollection();
                    return;
                }
                if (Keys.Ctrl && Keys.M && Keys.C)
                {
                    ManageCollections();
                    return;
                }

                if (Keys.Alt && Keys.S)
                {
                    OpenSettings();
                    return;
                }
                if (Keys.Alt && Keys.R)
                {
                    ReportBug();
                    return;
                }

                if (Keys.Alt && Keys.E && Keys.A && Keys.C)
                {
                    ExportAllCollections();
                    return;
                }
                if (Keys.Alt && Keys.E && Keys.A && Keys.M)
                {
                    ExportAllMemes();
                    return;
                }
                if (Keys.Alt && Keys.E && Keys.C)
                {
                    
                    return;
                }
                if (Keys.Alt && Keys.E)
                {
                    Export();
                    return;
                }
                if (Keys.Alt && Keys.I)
                {
                    Import();
                } 
            }
        }
        public KeyboardCtrls(Main Main, Ctrls Control) //Runs a keyboard shortcut control from normal ctrls. Throws an error if Main is null.
        {
            this.Ms = Main ?? throw new System.ArgumentNullException("The Main provided was null. This is not allowed.");
            switch (Control)
            {
                case Ctrls.AddCollection:
                    AddCollection();
                    break;
                case Ctrls.AddMeme:
                    AddMeme();
                    break;
                case Ctrls.Append:
                    Append();
                    break;
                case Ctrls.AppendAll:
                    AppendAll();
                    break;
                case Ctrls.Close:
                    Close();
                    break;
                case Ctrls.CloseEditor:
                    CloseEditor();
                    break;
                case Ctrls.ManageCollections:
                    ManageCollections();
                    break;
                case Ctrls.ManageMemes:
                    ManageMemes();
                    break;
                case Ctrls.NewSession:
                    NewSession();
                    break;
                case Ctrls.Open:
                    Open();
                    break;
                case Ctrls.OpenSettings:
                    OpenSettings();
                    break;
                case Ctrls.ReportBug:
                    ReportBug();
                    break;
                case Ctrls.Save:
                    Save();
                    break;
                case Ctrls.SaveAs:
                    SaveAs();
                    break;
            }
        }
        public KeyboardCtrls(Main Main, ImportExportOpt Control) //Runs a keyboard shortcut control from ImportExportOpt ctrls. Throws an error if Main is null.
        {
            this.Ms = Main ?? throw new System.ArgumentNullException("The Main provided was null. This is not allowed.");

            switch (Control)
            {
                case ImportExportOpt.AllCollections:
                    ExportAllCollections();
                    break;
                case ImportExportOpt.AllMemes:
                    ExportAllMemes();
                    break;
                case ImportExportOpt.Export:
                    Export();
                    break;
                case ImportExportOpt.ExportCurrent:
                    break;
                case ImportExportOpt.Import:
                    Import();
                    break;
            }
        }

        private Main Ms = null;

        private void Open() //Opens a file.
        {
            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog()
            {
                AddExtension = true,
                DefaultExt = ".teh",
                Filter = "Hmmm Pro Sessions | *.teh",
                Multiselect = false,
                RestoreDirectory = true,
                Title = "Select a session to open."
            };

            open.ShowDialog();

            string path = open.FileName;

            if (path == "")
            {
                return;
            }

            Ms.OpenFromPath(path);
        }
        private void NewSession() //Creates a new Session.
        {
            Microsoft.Win32.SaveFileDialog save = new Microsoft.Win32.SaveFileDialog()
            {
                AddExtension = true,
                DefaultExt = ".teh",
                Filter = "Hmmm Pro Sessions | *.teh",
                RestoreDirectory = true,
                Title = "Select a new location for the file."
            };

            save.ShowDialog();

            string path = save.FileName;

            if (path == "")
            {
                return;
            }

            if (File.Exists(path))
            {
                File.Delete(path);
            }
            File.Copy(Ins.BasicTemplate, path);

            Session sess = new Session(path);

            string[] parts = path.Split('\\');
            string item = parts[parts.Length - 1].Remove((parts[parts.Length - 1].Length - ".teh".Length));
            sess.Title = item;

            Ms.Title = $"{sess.Title} - Hmmm Pro";

            if (File.Exists(Ins.CurrentSession))
            {
                File.Delete(Ins.CurrentSession);
            }
            File.Copy(path, Ins.CurrentSession);

            Ins.CurrentLoaded = path;
            if (Ins.LastLoaded == "")
            {
                Ins.LastLoaded = path;
            }

            Ms.OpenFromPath(path);

            Ms.UnLock();
        }
        private void Save() //Runs Common.Save()
        {
            Common.Save();
        }
        private void SaveAs() //Runs Common.SaveAs()
        {
            Common.SaveAs();
        }
        private void Append() //Runs Common.Append()
        {
            Common.Append(Ms.CurrentAction);
        }
        private void AppendAll() //Runs Common.AppendAll()
        {
            Common.AppendAll();
        }
        private void Close() //Closes the currently loaded file.
        {
            if (Ms.IsActionRuning)
            {
                MessageBox.Show("Please complete all editor actions before closing the file.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBoxResult result = MessageBox.Show("Do you want to save?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
            switch (result)
            {
                case MessageBoxResult.Cancel | MessageBoxResult.None:
                    return;
                case MessageBoxResult.Yes:
                    Common.Save();

                    File.Delete(Ins.CurrentSession);
                    Ms.Lock();
                    break;
                case MessageBoxResult.No:
                    File.Delete(Ins.CurrentSession);
                    Ms.Lock();
                    break;
            }

            Ms.ResetEditor(Editors.CurrentFile);
            Ms.ResetActions();
        }
        private void CloseEditor() //Closes the curently open editor.
        {
            Ms.CloseEditor(Ms.CurrentAction);
        }
        private void AddMeme() //Loads the Add Meme editor.
        {
            Ms.LoadEditor(Editors.AddMeme);
        }
        private void ManageMemes() //Loads the Manage Memes editor.
        {
            Ms.LoadEditor(Editors.ManageCollections);
        }
        private void AddCollection() //Loads the Add Collections editor.
        {
            Ms.LoadEditor(Editors.AddCollection);
        }
        private void ManageCollections() //Loads the Manage Collections editor.
        {
            Ms.LoadEditor(Editors.ManageCollections);
        }
        private void OpenSettings() //Loads the Settings editor.
        {
            Ms.LoadEditor(Editors.Settings);
        }
        private void ReportBug() //Loads the ReportBug editor.
        {

        }

        private void ExportAllCollections() //Unused
        {
            new ImportExport().Execute(ImportExportOpt.AllCollections);
        }
        private void ExportAllMemes() //Unused
        {
            new ImportExport().Execute(ImportExportOpt.AllMemes);
        }
        private void Export() //Unused
        {
            new ImportExport().Execute(ImportExportOpt.Export);
        }
        private void Import() //Loads ImportExport using ImportExportOpt.Import.
        {
            new ImportExport().Execute(ImportExportOpt.Import);
        }
    }
}

namespace HmPro
{
    public enum ImportExportOpt //The options for availible keyboard shortcuts regarding ImportExport.
    {
        //Alt + E + C
        ExportCurrent,
        //Alt + E + A + C
        AllCollections,
        //Alt + E + A + M
        AllMemes,
        //Alt + E
        Export,
        //Alt + I
        Import
    }
    public enum Ctrls //All of the keyboard shortcuts in Hmmm Pro.
    {
        //Ctrl + O
        Open,
        //Ctrl + N
        NewSession,
        //Ctrl + S
        Save,
        //Ctrl + Shift + S
        SaveAs,
        //Ctrl + A
        Append,
        //Ctrl + Shift + A
        AppendAll,
        //Ctrl + Shift W
        Close,
        //Ctrl + W
        CloseEditor,
        //Ctrl + A + M
        AddMeme,
        //Ctrl + M + E
        ManageMemes,
        //Ctrl + A + C
        AddCollection,
        //Ctrl + M + C
        ManageCollections,
        //Alt + S
        OpenSettings,
        //Alt + R
        ReportBug
    }
}
