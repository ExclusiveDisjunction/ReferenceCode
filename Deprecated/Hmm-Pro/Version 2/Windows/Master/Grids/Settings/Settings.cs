using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void setShowWelcome_Checked(object sender, RoutedEventArgs e) //Sets Ins.ShowWelcome to True.
        {
            Ins.ShowWelcome = true;
        }
        private void setShowWelcome_Unchecked(object sender, RoutedEventArgs e) //Sets Ins.ShowWelcome to False.
        {
            Ins.ShowWelcome = false;
        }
        
        private void setLoadLast_Checked(object sender, RoutedEventArgs e) //Sets Ins.LoadLast to True.
        {
            Ins.LoadLast = true;
        }
        private void setLoadLast_Unchecked(object sender, RoutedEventArgs e) //Sets Ins.LoadLast to False.
        {
            Ins.LoadLast = false;
        }

        private void setSaveMemeCreator_Checked(object sender, RoutedEventArgs e) //Sets Ins.SaveMemeCreator to True.
        {
            Ins.SaveMemeCreator = true;
        }
        private void setSaveMemeCreator_Unchecked(object sender, RoutedEventArgs e) //Sets Ins.SaveMemeCreator to False.
        {
            Ins.SaveMemeCreator = false;
        }

        //private void setTour_Click(object sender, RoutedEventArgs e) //Loads the tour feature. In this version, it is not used.
        //{
        //    Interaction.Tour Tour = new Interaction.Tour();
        //    Tour.Execute();
        //}
        private void setResetRecents_Click(object sender, RoutedEventArgs e) //Resets all recents in the program and then notifies the user.
        {
            Recent.DeleteAllRecents();
            MessageBox.Show("All recents have been reset. To see results, you have to restart Hmmm Pro.", "Settings:", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
