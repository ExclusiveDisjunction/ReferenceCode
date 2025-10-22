using System.Windows;
using System.Windows.Input;
using System.Collections.Generic;

using HmPro.Files;
using HmPro.Events;
using Phosphor;
using Phosphor.Registries;

namespace HmPro.Windows
{
    /// <summary>
    /// Interaction logic for Windows\Master.xaml
    /// </summary>
    public partial class Main : Window
    {
        KeyboardRegistry KeyReg = null;

        public Main() //Loads the class and connects the events for saving, appending, and keyboard action
        {
            InitializeComponent();

            //Links up events to their procedures. Autosave.Tick runs the autosave procedure, App.Saved changes the title from '*{Session.Title} - Hmmm Pro' to '{Session.Title} - Hmmm Pro'. Appended does the opposite of Saved.
            App.Saved += Window_Saved;
            App.Appended += Window_Appened;

            Ins.Loaded += Ins_Loaded;
            Ins.Unloaded += Ins_Unloaded;

            List<KeyboardShortcut> Shortcuts = new List<KeyboardShortcut>()
            {
                new KeyboardShortcut(Key.O, true, false, false, Open_Shortcut), //Open File
                new KeyboardShortcut(Key.N, true, false, false, NewFile_Shortcut), //New File

                new KeyboardShortcut(new Key[2] { Key.C, Key.S}, true, false, false, SaveACopy_Shortcut), //Save a copy
                new KeyboardShortcut(Key.S, true, false, false, SaveAs_Shortcut), //Save As

                new KeyboardShortcut(Key.W, true, true, false, CloseEditor_Shortcut), //Close Editor
                new KeyboardShortcut(Key.W, true, false, false, CloseFile_Shortcut), //Close File
                
                new KeyboardShortcut(Key.A, true, true, false, AppendAll_Shortcut), //Append All
                new KeyboardShortcut(Key.A, true, false, false, Append_Shortcut), //Append

                new KeyboardShortcut(Key.S, false, false, true, Settings_Shortcut), //Settings
                new KeyboardShortcut(Key.E, false, false, true, Export_Shortcut), //Export
                new KeyboardShortcut(Key.I, false, false, true, Import_Shortcut), //Import
                new KeyboardShortcut(Key.C, false, false, true, ConvertFile_Shortcut) //Convert File
            };

            KeyReg = new KeyboardRegistry(Shortcuts, this);
        }
        ~Main() //Releases the events from the current window.
        {
            App.Saved -= Window_Saved;
            App.Appended -= Window_Appened;
            Ins.Loaded -= Ins_Loaded;
            Ins.Unloaded -= Ins_Unloaded;

            KeyReg?.Release();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) //Sets the WindowState of the window to Ins.LastState (set in shutdown), and sets up the welcome screen if Ins.ShowWelcome.
        {
            this.WindowState = Ins.LastState == WindowState.Minimized ? WindowState.Normal : Ins.LastState; //Sets the windows visual state to the last one to be set. If it is minimized, it sets it to normal.

            HideAllEditors();
            ResetActions();
            Lock();
            if (Ins.IsLoaded)
            {
                Information.Visibility = Visibility.Collapsed;
                UnLock();

                LoadEditor(Editors.CurrentFile);
                ProgressText.Text = "Ready";
            }
            else
            {
                Information.Visibility = Visibility.Visible;
                ProgressText.Text = "Waiting for User Project...";
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) //Sets the Ins.LastState (a window state) to this current state, allowing on startup to resize to it.
        {
            App.Appended -= Window_Appened;
            App.Saved -= Window_Saved;

            Ins.Loaded -= Ins_Loaded;
            Ins.Unloaded -= Ins_Unloaded;

            Ins.LastState = this.WindowState;
            Ins.OpenWindows.Remove(this);

            CloseAllEditors();
            App.ExitApp();
        }

        private void Window_Saved(object sender, SaveEventArgs e) //This changes the text of the title to show that the file was saved.
        {
            ProgressText.Text = $"Saved session on {System.DateTime.Now.ToShortDateString()} at {System.DateTime.Now.ToShortTimeString()}"; 
        }
        private void Window_Appened(object sender, AppendEventArgs e) //This changes the text of the title to show that the file was appended, and modifyied. (Adds an * before the file name).
        {
            ProgressText.Text = $"Appended data on {System.DateTime.Now.ToShortDateString()} at {System.DateTime.Now.ToShortTimeString()}"; 
        }
        private void Ins_Unloaded(object sender, System.EventArgs e) //Shows the information label in the middle of the window.
        {
            Lock();
            Information.Visibility = Visibility.Visible;
            ProgressText.Text = "Waiting for User Project...";
        }
        private void Ins_Loaded(object sender, System.EventArgs e) //Hides the information label in teh middle of the window.
        {
            UnLock();
            Information.Visibility = Visibility.Collapsed;
            ProgressText.Text = "Ready";

        }

        private void AppendAction(object sender, RoutedEventArgs e) //Used by all appendable editors, this calls the Common.Append(Main, Action) method.
        {
            SessionControl.Append(CurrentAction);
        }
        private void CancelJob(object sender, RoutedEventArgs e) //This closes and resets the current editor. This is used by all appendable editors.
        {
            CloseEditor(CurrentAction);
        }

        public override string ToString() //Returns the window's title.
        {
            return this.Title;
        }
    }
}
