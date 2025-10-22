using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using HmPro.Files;
using Phosphor;

namespace HmPro.Windows.Editing
{
    /// <summary>
    /// Interaction logic for Duplicate_Selector.xaml
    /// </summary>
    public partial class DuplicateSelector : Window
    {
        private DuplicateSelector()
        {
            InitializeComponent();
        }

        private bool Submitted = false;
        private IHSession File;
        private IComponent Return = null;
        private List<TriplePair<RadioButton, IComponent, int>> MasterList;

        public static IComponent Execute(string Title)
        {
            DuplicateSelector Sel = new DuplicateSelector
            {
                File = Ins.LoadedSession,
                MasterList = new List<TriplePair<RadioButton, IComponent, int>>()
            };

            IComponent[] List = Sel.File.GetObject(Title, true, true, true);
            int i = 0;
            foreach (IComponent item in List)
            {
                string ContentString = $"{item.Title}, {item.ObjectType}, ";
                switch (item.ObjectType)
                {
                    case ComponentTypes.Bin:
                        ContentString += $"[No Type], {((item as Bin).Creator == "" ? "[No Creator]" : (item as Bin).Creator)}";
                        break;
                    case ComponentTypes.Collection:
                        ContentString += $"{(item as Collection).Type}, {((item as Collection).Creator == "" ? "[No Creator]" : (item as Collection).Creator)}";
                        break;
                    case ComponentTypes.File:
                        ContentString += "[No Type], [No Creator]";
                        break;
                    case ComponentTypes.Meme:
                        ContentString += $"{(item as Meme).Type}, {((item as Meme).Creator == "" ? "[No Creator]" : (item as Meme).Creator)}";
                        break;
                    case ComponentTypes.Script:
                        ContentString += "[No Type], [No Creator]";
                        break;
                }

                RadioButton Button = new RadioButton
                {
                    Content = ContentString,
                    Foreground = new SolidColorBrush(Color.FromRgb(0xFF, 0xFF, 0xFF))
                };
                Sel.Display.Children.Add(Button);

                Sel.MasterList.Add(new TriplePair<RadioButton, IComponent, int>(Button, item, i));
                i++;
            }

            Sel.ShowDialog();

            return Sel.Return;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Ins.OpenWindows.Add(this);
        }
        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            Ins.OpenWindows.Remove(this);

            if (!Submitted)
                Return = null;
        }

        private void Submit_Click(object sender, RoutedEventArgs e)
        {
            Submitted = true;

            int i = 0;
            foreach (UIElement item in Display.Children)
            {
                if (item is RadioButton)
                {
                    if ((item as RadioButton).IsChecked == true)
                        goto End;
                    else
                        i++;
                }
            }

            End:
            Return = MasterList[i].TwoValue;

            this.Close();
        }
        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            Submitted = false;
            Return = null;
            this.Close();
        }
    }
}
