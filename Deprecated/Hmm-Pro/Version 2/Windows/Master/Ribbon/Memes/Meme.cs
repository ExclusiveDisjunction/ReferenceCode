using System.Windows.Input;
using HmPro.Windows.Editing;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void AddMemeCmd_MouseDown(object sender, MouseButtonEventArgs e)//Loads the AddMeme editor.
        {
            LoadEditor(Editors.AddMeme);
        }
        private void ManageMemesCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the ManageMemes editor.
        {
            LoadEditor(Editors.ManageMemes);
        }
        private void ExportMemesCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads Interaction.ImportExport() window.
        {
           ImportExport opt = new ImportExport();
            opt.Execute(ImportExportOpt.AllMemes);
        }

        private void AddCollectionCmd_MouseDown(object sender, MouseButtonEventArgs e) //Load the AddCollection editor.
        {
            LoadEditor(Editors.AddCollection);
        }
        private void ManageCollectionsCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the ManageMemes editor.
        {
            LoadEditor(Editors.ManageCollections);
        }
        private void ExportCollectionsCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the Interaction.ImportExport() window.
        {
            ImportExport opt = new ImportExport();
            opt.Execute(ImportExportOpt.AllCollections);
        }

        private void MemeRegulationsCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the Interaction.MemeRegulations() window.
        {
            MemeRegulations reg = new MemeRegulations();
            reg.ShowDialog();
        }
    }
}
