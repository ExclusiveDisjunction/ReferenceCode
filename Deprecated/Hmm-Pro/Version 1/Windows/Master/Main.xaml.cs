using System.Windows;
using System.Windows.Input;

using HmPro.Scripting;
using HmPro.Scripting.Files;
using HmPro.Scripting.Events;
using Lyseria;

namespace HmPro.Windows
{
    /// <summary>
    /// Interaction logic for Windows\Master.xaml
    /// </summary>
    public partial class Main : Window
    {
        public Main() //Loads the class and connects the events for saving, appending, and Autosave.Tick.
        {
            InitializeComponent();

            //Links up events to their procedures. Autosave.Tick runs the autosave procedure, App.Saved changes the title from '*{Session.Title} - Hmmm Pro' to '{Session.Title} - Hmmm Pro'. Appended does the opposite of Saved.
            App.Saved += Window_Saved;
            App.Appended += Window_Appened;

            Ins.Loaded += Ins_Loaded;
            Ins.Unloaded += Ins_Unloaded;
        }

        #region WindowEvents

        private void Window_Loaded(object sender, RoutedEventArgs e) //Sets the WindowState of the window to Ins.LastState (set in shutdown), and sets up the welcome screen if Ins.ShowWelcome.
        {
            if (Ins.LastState == WindowState.Minimized)
            {
                this.WindowState = WindowState.Normal;
            }
            else
            {
                this.WindowState = Ins.LastState;
            }

            Lock();
            if (Ins.IsLoaded)
            {
                WelcomeGrid.Visibility = Visibility.Collapsed;
                Information.Visibility = Visibility.Collapsed;
                UnLock();

                LoadEditor(Editors.CurrentFile);
                ProgressText.Text = "Ready";
            }
            else if (Ins.ShowWelcome)
            {
                WelcomeGrid.Visibility = Visibility.Visible;
                ShowWelcome.IsChecked = Ins.ShowWelcome;

                if (Ins.GetRecent(RecentOptions.Rec1) != null)
                {
                    Recent1.Visibility = Visibility.Visible;
                    Session1.Content = Ins.RecentName(RecentOptions.Rec1);
                    Session1Path.Content = Ins.GetRecent(RecentOptions.Rec1);
                }
                else
                {
                    Recent1.Visibility = Visibility.Hidden;
                }

                if (Ins.GetRecent(RecentOptions.Rec2) != null)
                {
                    Recent2.Visibility = Visibility.Visible;
                    Session2.Content = Ins.RecentName(RecentOptions.Rec2);
                    Session2Path.Content = Ins.GetRecent(RecentOptions.Rec2);
                }
                else
                {
                    Recent2.Visibility = Visibility.Hidden;
                }

                if (Ins.GetRecent(RecentOptions.Rec3) != null)
                {
                    Recent3.Visibility = Visibility.Visible;
                    Session3.Content = Ins.RecentName(RecentOptions.Rec3);
                    Session3Path.Content = Ins.GetRecent(RecentOptions.Rec3);
                }
                else
                {
                    Recent3.Visibility = Visibility.Hidden;
                }

                if (Ins.GetRecent(RecentOptions.Rec4) != null)
                {
                    Recent4.Visibility = Visibility.Visible;
                    Session4.Content = Ins.RecentName(RecentOptions.Rec4);
                    Session4Path.Content = Ins.GetRecent(RecentOptions.Rec4);
                }
                else
                {
                    Recent4.Visibility = Visibility.Hidden;
                }

                if (Ins.GetRecent(RecentOptions.Rec5) != null)
                {
                    Recent5.Visibility = Visibility.Visible;
                    Session5.Content = Ins.RecentName(RecentOptions.Rec5);
                    Session5Path.Content = Ins.GetRecent(RecentOptions.Rec5);
                }
                else
                {
                    Recent5.Visibility = Visibility.Hidden;
                }
            }
            else
            {
                WelcomeGrid.Visibility = Visibility.Collapsed;
                Information.Visibility = Visibility.Visible;
                ProgressText.Text = "Waiting for User Project...";
            }

            HideAllEditors();
            ResetActions();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) //Sets the Ins.LastState (a window state) to this current state, allowing on startup to resize to it.
        {
            App.Appended -= Window_Appened;
            App.Saved -= Window_Saved;

            Ins.LastState = this.WindowState;
            Ins.OpenWindows.Remove(this);

            CloseAllEditors();
            foreach (Window thing in Ins.OpenWindows) { thing.Close(); }

            App.ExitApp();
        }
        private void Window_KeyDown(object sender, KeyEventArgs e) //This runs the HmPro.Scripting.Functions.KeyboardCtrl() method, which determines the keyboard's current state and executes the keyboard shortcut.
        {
            new Scripting.Functions.KeyboardCtrls(this);
        }

        #endregion
        #region AppEvents

        private void Window_Saved(object sender, SaveEventArgs e) //This changes the text of the title to show that the file was saved.
        {
            UpdateTitle(e.Session, false);
        }
        private void Window_Appened(object sender, AppendEventArgs e) //This changes the text of the title to show that the file was appended, and modifyied. (Adds an * before the file name).
        {
            UpdateTitle(e.Session, true);
        }
        private void Ins_Unloaded(object sender, System.EventArgs e) //Shows the information label in the middle of the window.
        {
            Information.Visibility = Visibility.Visible;
            ProgressText.Text = "Waiting for User Project...";
        }
        private void Ins_Loaded(object sender, System.EventArgs e) //Hides the information label in teh middle of the window.
        {
            Information.Visibility = Visibility.Collapsed;
            ProgressText.Text = "Ready";
        }

        #endregion
        #region EditorControl

        private void AppendAction(object sender, RoutedEventArgs e) //Used by all appendable editors, this calls the Common.Append(Main, Action) method.
        {
            Common.Append(CurrentAction);
        }
        private void CancelJob(object sender, RoutedEventArgs e) //This closes and resets the current editor. This is used by all appendable editors.
        {
            CloseEditor(CurrentAction);
        }

        #endregion

        public void UpdateTitle(Session Session, bool Edited) //Updates the title based off of a session's name and the Edited factor.
        {
            string New = Edited ? "*" : ""; //Shows an * when there is an edit in the file. This is set to '*' when appending, and to '' when saved.
            New += Session.Title;

            this.Title = New;
        }
        public override string ToString() //Returns the window's title.
        {
            return this.Title;
        }
    }
}
