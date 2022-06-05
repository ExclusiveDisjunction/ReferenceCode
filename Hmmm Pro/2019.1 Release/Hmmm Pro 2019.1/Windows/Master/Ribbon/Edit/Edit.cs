using System.Windows.Input;
using System.Windows;

using HmPro.Windows.Editing;
using HmPro.Scripting.Files;
using HmPro.Scripting.Events;

namespace HmPro.Windows
{
    public partial class Main
    {
        private void AllObjectsCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the all objects editor.
        {
            ShowEditor(Editors.AllObjects);
        }
        private void SettingsCmd_MouseDown(object sender, MouseButtonEventArgs e) //Loads the settings editor.
        {
            ShowEditor(Editors.Settings);
        }
        private void ScriptEditorCmd_MouseDown(object sender, MouseButtonEventArgs e) //Opens object selector, filtering for memes, and then opes the script editor window if the script is a script meme.
        {
            int ScriptMemeCount = 0;
            foreach (Meme meme in new Session().Memes)
            {
                if (meme.ObjType == MemeTypes.Script)
                {
                    ScriptMemeCount++;
                }
            }
            if (new Session().Memes.Count <= 0 || ScriptMemeCount == 0)
            {
                MessageBox.Show("There are no memes to edit. Please add a meme by going to Meme->Add Meme. For this feature to work, you need at least one script meme.", "Script Editor:", MessageBoxButton.OK, MessageBoxImage.Error);
            }

            int Tries = 0;
            Start:
            ConnectEventArgs Args = new ObjectSelector().Execute(Memes: true, Collections: false, MasterCollections: false, Scripts: false);
            if (Args.Connected == false || Args.Object == null || Args.Source == null)
            {
                MessageBoxResult Result = MessageBox.Show("No object was selected. Do you want to abort? (Cancel)", "Object Selector:", MessageBoxButton.YesNo, MessageBoxImage.Warning | MessageBoxImage.Question);
                switch (Result)
                {
                    case MessageBoxResult.Yes:
                    {
                        return;
                    }
                    case MessageBoxResult.No:
                    {
                        if (Tries >= 3)
                        {
                            MessageBox.Show("The job has failed three times. Please try again later.\nIf the problem persists, please contact me.", "Job Failed:", MessageBoxButton.OK, MessageBoxImage.Error);
                            return;
                        }
                        Tries++;
                        goto Start;
                    }
                    case MessageBoxResult.None:
                    {
                        return;
                    }
                }
            }
            if (Args.Object is Meme Meme && Meme.ObjType == MemeTypes.Script)
            {
                new ScriptEditor().Execute(Meme);
            }
            else
            {
                if (Tries >= 3)
                {
                    MessageBox.Show("The job has failed three times. Please try again later.\nIf the problem persists, please contact Lyseria Technologies @ Connorhcs17@outlook.com.", "Job Failed:", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                MessageBox.Show("The object selected is not a Meme, or is not a Script Meme. Press OK to try again.", "Incorrect Object:", MessageBoxButton.OK, MessageBoxImage.Warning);
                Tries++;
                goto Start;
            }
        }
    }
}
