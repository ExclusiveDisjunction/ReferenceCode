using System.Windows;

namespace HmPro.Windows.Docs
{
    public partial class Documentation
    {
        private void AppendAndAppendAll_Selected(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Append & Append All";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Actions\\Append and Append All.tehst");
        }
        private void SaveAndSaveAll_Selected(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Save & Save All";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Actions\\Save and Save All.tehst");
        }

        private void Importing_Selected(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Importing";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Actions\\Importing.tehst");
        }
        private void Exporting_Selected(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Exporting";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Actions\\Exporting.tehst");
        }
    }
}
