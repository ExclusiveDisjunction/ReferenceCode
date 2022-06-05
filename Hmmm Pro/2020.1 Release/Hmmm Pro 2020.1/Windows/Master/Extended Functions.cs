using System.Windows;
using HmPro.Registry;

namespace HmPro.Windows
{
    public partial class Main 
    {
        public void Lock() //Locks the ribbon from use.
        {
            this.Title = "Hmmm Pro 2020.1 - (Unloaded)";

            Ribbon.SelectedIndex = 0;

            ObjectsCmds.IsEnabled = false;
            EditCmds.IsEnabled = false;

            CloseFileCmd.IsEnabled = false;
            SaveAsCopyCmd.IsEnabled = false;
            SaveAsCmd.IsEnabled = false;
            AppendCmd.IsEnabled = false;
            AppendAllCmd.IsEnabled = false;
            ImportCmd.IsEnabled = false;
            ExportCmd.IsEnabled = false;
            PropertiesCmd.IsEnabled = false;

            Navigation.IsEnabled = false;

            CloseAllEditors();
            Information.Visibility = Visibility.Visible;
        }
        public void UnLock() //Unlocks the ribbon from use.
        {
            if (Ins.IsLoaded)
            {
                this.Title = $"Hmmm Pro 2020.1 - {Ins.LoadedSession.Title}";
            }

            Ribbon.SelectedIndex = 0;

            ObjectsCmds.IsEnabled = true;
            EditCmds.IsEnabled = true;

            CloseFileCmd.IsEnabled = true;
            SaveAsCopyCmd.IsEnabled = true;
            SaveAsCmd.IsEnabled = true;
            AppendCmd.IsEnabled = true;
            AppendAllCmd.IsEnabled = true;
            ImportCmd.IsEnabled = true;
            ExportCmd.IsEnabled = true;
            PropertiesCmd.IsEnabled = true;

            Navigation.IsEnabled = true;

            CloseAllEditors();

            Information.Visibility = Visibility.Collapsed;
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