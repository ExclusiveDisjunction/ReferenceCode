using System.Windows;

namespace HmPro.Windows.Docs
{
    public partial class Documentation
    {
		private void Intro(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Intro";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Intro.tehst");
        }

        private void KeyboardShortcuts(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Keyboard Shortcuts";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Keyboard Shortcuts.tehst");
        }

        private void ScriptEditor(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Script Editor";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Script Editor.tehst");
        }
    }
}
