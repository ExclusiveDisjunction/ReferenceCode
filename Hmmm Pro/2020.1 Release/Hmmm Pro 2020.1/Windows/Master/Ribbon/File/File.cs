/*
 Hmmm Pro - Main Split.

 This file deals with all of the buttons in the File Ribbon. In a future release, this file will be remove, as the buttons will now be lables, and follow the MouseDown Procedure in File.cs
 */

using System.Windows;
using HmPro.Windows.Editing;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void OpenCmd_Click(object sender, RoutedEventArgs e) //Opens a session from using a Microsoft.Win22.OpenFileDialog for a path.
        {
            if (SessionControl.OpenFile()) LoadEditor(Editors.CurrentFile);
        }
        private void NewFileCmd_Click(object sender, RoutedEventArgs e)
        {
            if (SessionControl.NewFile()) LoadEditor(Editors.CurrentFile);
        }
        private void CloseFileCmd_Click(object sender, RoutedEventArgs e)
        {
            SessionControl.CloseFile();
        }

        private void ImportCmd_Click(object sender, RoutedEventArgs e) //This opens the Windows.Interaction.ImportExport window, using it's Execute function for standard Importing.
        {            
            ImportExport.Execute(ImportExportOpt.Import);
        }
        private void ExportCmd_Click(object sender, RoutedEventArgs e) //This opens the Windows.Interaction.ImportExport window, using it's Execute function for standard Exporting.
        {
            ImportExport.Execute(ImportExportOpt.Export);
        }

        private void SaveAsCopyCmd_Click(object sender, RoutedEventArgs e) //This uses the Common.SaveAsCopy() method.
        {
            SessionControl.SaveAsCopy();
        }
        private void SaveAsCmd_Click(object sender, RoutedEventArgs e) //This uses the Common.SaveAs() method.
        {
            if (SessionControl.SaveAs()) LoadEditor(Editors.CurrentFile);
        }

        private void AppendCmd_Click(object sender, RoutedEventArgs e) //Runs the Common.Append() method.
        {
            SessionControl.Append(CurrentAction);
        }
        private void AppendAllCmd_Click(object sender, RoutedEventArgs e) //Runs the Common.AppendAll() method.
        {
            SessionControl.AppendAll();
        }

        private void PropertiesCmd_Click(object sender, RoutedEventArgs e) //Loads the CurrentFile editor.
        {
            LoadEditor(Editors.CurrentFile);
        }
        private void ChangeFileTypeCmd_Click(object sender, RoutedEventArgs e)
        {
        }
    }
}
