using System.Windows;
using System.IO;

using HmPro.Scripting.Files;
using HmPro.Scripting.Functions.Edit;

using HmPro.Scripting;

namespace HmPro.Windows
{
    public partial class Main 
    {
        public void OpenFromPath(string path) //Loads a session from a preditermined path. Used by New and Open methods.
        {
            ProgressText.Text = "Opening File...";

            if (path == "")
            {
                ProgressText.Text = "Ready";
                return;
            }

            if (!File.Exists(path))
            {
                MessageBox.Show("The path selected does not exists. Please try again later.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);

                ProgressText.Text = "Ready";
                return;
            }

            if (Ins.IsLoaded)
            {
                Common.Save(Stop:false);
                Lock();
            }

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

            try
            {
                Session sess = new Session();

                if (Lyseria.FileTools.FileName(path) != sess.Title)
                    sess.Title = Lyseria.FileTools.FileName(path);

                if (sess.Version == Lyseria.AppVersions.V_2019_1)
                {
                    ProgressText.Text = $"Session '{sess.Title}': lower version detected.";

                    MessageBoxResult result = MessageBox.Show("This session is made for Hmmm Pro 2019.1. If opened, some features cannot be used. Do you want to continue?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Warning);
                    switch (result)
                    {
                        case MessageBoxResult.Cancel:
                            Lock();

                            ProgressText.Text = "Job Canceled, canceled by user.";

                            File.Delete(Ins.CurrentSession);
                            return;
                        case MessageBoxResult.No:
                            Lock();

                            ProgressText.Text = "Job Canceled, canceled by user.";

                            File.Delete(Ins.CurrentSession);
                            return;
                        case MessageBoxResult.Yes:
                            MessageBox.Show("To update the file to 2019.2, goto File > Update File Definition", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                            break;
                    }
                }
                UpdateTitle(sess, false);

                ProgressText.Text = "Ready.";

                LoadEditor(Editors.CurrentFile);
            }
            catch
            {
                MessageBox.Show("This session cannot be opened. This could be corrupted, or is not a Hmmm Pro Session. For troubleshooting, goto Help > Troubleshoot Session", "Hmmm Pro:", MessageBoxButton.OK);
                File.Delete(Ins.CurrentSession);
                Lock();

                ProgressText.Text = "Ready";

                return;
            }

            Ins.SetRecent(path);
            UnLock();
        }

        public void Lock() //Locks the ribbon from use.
        {
            Ribbon.SelectedIndex = 0;

            MemesCmds.IsEnabled = false;
            EditCmds.IsEnabled = false;

            CloseFileCmd.IsEnabled = false;
            SaveCmd.IsEnabled = false;
            SaveAsCmd.IsEnabled = false;
            AppendCmd.IsEnabled = false;
            AppendAllCmd.IsEnabled = false;
            ImportCmd.IsEnabled = false;
            ExportCmd.IsEnabled = false;
            PropertiesCmd.IsEnabled = false;

            Navigation.IsEnabled = false;
        }
        public void UnLock() //Unlocks the ribbon from use.
        {
            Ribbon.SelectedIndex = 0;

            MemesCmds.IsEnabled = true;
            EditCmds.IsEnabled = true;

            CloseFileCmd.IsEnabled = true;
            SaveCmd.IsEnabled = true;
            SaveAsCmd.IsEnabled = true;
            AppendCmd.IsEnabled = true;
            AppendAllCmd.IsEnabled = true;
            ImportCmd.IsEnabled = true;
            ExportCmd.IsEnabled = true;
            PropertiesCmd.IsEnabled = true;

            Navigation.IsEnabled = true;
        }

        public bool IsActionRuning //Returns OpenEditors.IsRunning
        {
            get
            {
                return EditorReg.EditorRunning;
            }
        }
    }
}