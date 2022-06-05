using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HmPro.Windows.Docs
{
    public partial class Documentation
    {
        private void File(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Ribbon\\File.tehst");
        }
        private void Memes(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Ribbon\\Memes.tehst");
        }
        private void Edit(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Ribbon\\Edit.tehst");
        }
        private void Window(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Ribbon\\Window.tehst");
        }
        private void Help(object sender, RoutedEventArgs e)
        {
            itTitle.Text = "Help";
            itText.Text = System.IO.File.ReadAllText("doc\\2019.1\\Ribbon\\Help.tehst");
        }
    }
}
