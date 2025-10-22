using System.Windows;
using HmPro.Windows.Editing;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void AddMemeCmd_Click(object sender, RoutedEventArgs e) //Loads the AddMeme editor.
        {
            LoadEditor(Editors.AddMeme);
        }
        private void ManageMemesCmd_Click(object sender, RoutedEventArgs e) //Loads the ManageMemes editor.
        {
            LoadEditor(Editors.ManageMemes);
        }
        private void ExportMemesCmd_Click(object sender, RoutedEventArgs e) //Loads Interaction.ImportExport() window.
        {
            ImportExport opt = new ImportExport();
            opt.Execute(ImportExportOpt.AllMemes);
        }

        private void AddCollectionCmd_Click(object sender, RoutedEventArgs e) //Load the AddCollection editor.
        {
            LoadEditor(Editors.AddCollection);
        }
        private void ManageCollectionsCmd_Click(object sender, RoutedEventArgs e) //Loads the ManageMemes editor.
        {
            LoadEditor(Editors.ManageCollections);
        }
        private void ExportCollectionsCmd_Click(object sender, RoutedEventArgs e) //Loads the Interaction.ImportExport() window.
        {
            ImportExport opt = new ImportExport();
            opt.Execute(ImportExportOpt.AllCollections);
        }

        private void MemeRegulationsCmd_Click(object sender, RoutedEventArgs e) //Loads the Interaction.MemeRegulations() window.
        {
            MemeRegulations reg = new MemeRegulations();
            reg.ShowDialog();
        }
    }
}
