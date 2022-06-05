using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Phosphor;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void Open_Shortcut(object sender, Window Parent)
        {
            SessionControl.OpenFile(); //If it succeeds, then the Ins.Loaded will trigger will run.
            LoadEditor(Editors.CurrentFile);
        }
        private void NewFile_Shortcut(object sender, Window Parent)
        {
            SessionControl.NewFile();
            LoadEditor(Editors.CurrentFile);
        }

        private void SaveACopy_Shortcut(object sender, Window Parent)
        {
            SessionControl.SaveAsCopy();
        }
        private void SaveAs_Shortcut(object sender, Window Parent)
        {
            SessionControl.SaveAs();
            LoadEditor(Editors.CurrentFile);
        }

        private void CloseEditor_Shortcut(object sender, Window Parent)
        {
            CloseEditor(CurrentAction);
        }
        private void CloseFile_Shortcut(object sender, Window Parent)
        {
            SessionControl.CloseFile();
        }

        private void AppendAll_Shortcut(object sender, Window Parent)
        {
            SessionControl.AppendAll();
        }
        private void Append_Shortcut(object sender, Window Parent)
        {
            SessionControl.Append(CurrentAction, true);
        }

        private void Settings_Shortcut(object sender, Window Parent)
        {
            LoadEditor(Editors.Settings);
        }
        private void Export_Shortcut(object sender, Window Parent)
        {
            Editing.ImportExport.Execute(ImportExportOpt.Export);
        }
        private void Import_Shortcut(object sender, Window Parent)
        {
            Editing.ImportExport.Execute(ImportExportOpt.Import);
        }
        private void ConvertFile_Shortcut(object sender, Window Parent)
        {

        }
    }
}
