using System.Windows;

using HmPro.Scripting.Functions.Edit;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void Action1_Click(object sender, RoutedEventArgs e) //Loads the editor in action 1.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            ShowEditor(EditorReg.Action1);
            Action1Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action1;
        }
        private void Action2_Click(object sender, RoutedEventArgs e) //Loads the editor in action 2.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            ShowEditor(EditorReg.Action2);
            Action2Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action2;
        }
        private void Action3_Click(object sender, RoutedEventArgs e) //Loads the editor in action 3.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            LoadEditor(EditorReg.Action3);
            Action3Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action3;
        }
        private void Action4_Click(object sender, RoutedEventArgs e) //Loads the editor in action 4.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            ShowEditor(EditorReg.Action4);
            Action4Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action4;
        }
        private void Action5_Click(object sender, RoutedEventArgs e) //Loads the editor in action 5.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            ShowEditor(EditorReg.Action5);
            Action5Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action5;
        }
        private void Action6_Click(object sender, RoutedEventArgs e) //Loads the editor in action 6.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            ShowEditor(EditorReg.Action6);
            Action6Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action6;
        }
        private void Action7_Click(object sender, RoutedEventArgs e) //Loads the editor in action 7.
        {
            if (!IsLoaded)
            {
                Lock();
                HideAllEditors();
                ResetCloses();
                ResetActions();
            }

            HideAllEditors();
            ResetCloses();
            ShowEditor(EditorReg.Action7);
            Action7Close.Visibility = Visibility.Visible;

            CurrentAction = Actions.Action7;
        }

        private void Action1Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 1.
        {
            CloseEditor(Actions.Action1);
        }
        private void Action2Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 2.
        {
            CloseEditor(Actions.Action2);
        }
        private void Action3Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 3.
        {
            CloseEditor(Actions.Action3);
        }
        private void Action4Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 4.
        {
            CloseEditor(Actions.Action4);
        }
        private void Action5Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 5.
        {
            CloseEditor(Actions.Action5);
        }
        private void Action6Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 6.
        {
            CloseEditor(Actions.Action6);
        }
        private void Action7Close_Click(object sender, RoutedEventArgs e) //Closes the editor in action 7.
        {
            CloseEditor(Actions.Action7);
        }
    }
}
