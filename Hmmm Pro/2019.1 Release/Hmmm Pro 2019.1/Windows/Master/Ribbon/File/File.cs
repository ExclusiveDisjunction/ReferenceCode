/*
 Hmmm Pro - Main Splid.

 this file deals with all of the grids in the File Ribbon.
 */

using System.Windows;
using System.Windows.Input;
using System.IO;
using HmPro.Scripting.Files;
using HmPro.Scripting;
using HmPro.Windows.Editing;
using HmPro.Scripting.Functions.Edit;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void OpenCmd_MouseDown(object sender, MouseButtonEventArgs e) //Opens a session from using a Microsoft.Win32.OpenFileDialog for a path.
        {
            ProgressText.Text = "Opening a File...";

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
            OpenFromPath(path);
        }
        private void NewFileCmd_MouseDown(object sender, MouseButtonEventArgs e) //Creates a new session and then loads that session using OpenFromPath(string) method.
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
                ProgressText.Text = "Ready";
                return;
            }

            if (File.Exists(path))
            {
                File.Delete(path);
            }
            File.Copy(Ins.BasicTemplate, path);

            Session sess = new Session(path);

            string[] parts = path.Split('\\');
            string item = parts[parts.Length - 1].Remove(parts[parts.Length - 1].Length - ".teh".Length); //Removes the directory part and removes the .teh extension to get the Session's Title.
            sess.Title = item;

            this.Title = $"{sess.Title} - Hmmm Pro";

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

            ProgressText.Text = $"Created File: '{sess.Title}.teh'.";
            Lyseria.Timer.WaitSeconds(2);

            OpenFromPath(path);

            UnLock();
        }
        private void CloseFileCmd_MouseDown(object sender, MouseButtonEventArgs e) //Closes the current session.
        {
            if (EditorReg.AppendableEditorRunning)
            {
                MessageBox.Show("Please complete all editor actions before closing the file.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            foreach (Editors Editor in new EditorReg())
            {
                CloseEditor(EditorReg.AssociatedAction(Editor));
            }

            MessageBoxResult result = MessageBox.Show("Do you want to save?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
            switch (result)
            {
                case MessageBoxResult.Cancel | MessageBoxResult.None:
                    return;
                case MessageBoxResult.Yes:
                    Common.Save();

                    File.Delete(Ins.CurrentSession);
                    Ins.CurrentLoaded = "";
                    Lock();
                    break;
                case MessageBoxResult.No:
                    File.Delete(Ins.CurrentSession);
                    Ins.CurrentLoaded = "";
                    Lock();
                    break;
            }

            ResetEditor(Editors.CurrentFile);
            ResetActions();
        }
        
        private void SaveCmd_MouseDown(object sender, MouseButtonEventArgs e) //This uses the Common.Save() method.
        {
            Common.Save();
        }
        private void SaveAsCmd_MouseDown(object sender, MouseButtonEventArgs e) //This uses the Common.SaveAs method.
        {
            Common.SaveAs();
        }

        private void AppendCmd_MouseDown(object sender, MouseButtonEventArgs e) //This uses the Common.Append() method.
        {
            Common.Append(CurrentAction);
        }
        private void AppendAllCmd_MouseDown(object sender, MouseButtonEventArgs e) //This uses the Common.AppendAll() method.
        {
            Common.AppendAll();
        }
        
        private void ImportCmd_MouseDown(object sender, MouseButtonEventArgs e) //This opens the Windows.Interaction.ImportExport window, using it's Execute function for standard Importing.
        {
            ImportExport Opt = new ImportExport();
            Opt.Execute(ImportExportOpt.Import);
        }
        private void ExportCmd_MouseDown(object sender, MouseButtonEventArgs e) //This opens the Windows.Interaction.ImportExport window, using it's Execute function for standard Exporting.
        {
            ImportExport Opt = new ImportExport();
            Opt.Execute(ImportExportOpt.Export);
        }

        private void PropertiesCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the CurrentFile editor.
        {
            LoadEditor(Editors.CurrentFile);
        }
    }
}
