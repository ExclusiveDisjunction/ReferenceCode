using System.Windows;
using System.Windows.Controls;
using System.Collections.Generic;

using HmPro.Files;
using HmPro.Registry;
using Phosphor;
using HmPro.Edit;

namespace HmPro.Windows
{
    public partial class Main
    {
        public void HideAllEditors() //Hides all open editors. This does not reset them.
        {
            AddMeme.Visibility = Visibility.Collapsed;
            CurrentFile.Visibility = Visibility.Collapsed;
            AllObjects.Visibility = Visibility.Collapsed;
            AddCollection.Visibility = Visibility.Collapsed;
            ManageCollections.Visibility = Visibility.Collapsed;
            BasicEditor.Visibility = Visibility.Collapsed;
            Settings.Visibility = Visibility.Collapsed;
            ManageMemes.Visibility = Visibility.Collapsed;
        }
        public void CloseEditor(Actions Action) //Closes a single editor, resets it, and opens the last action. If the last action is Action1 or None, then it closes the action only.
        {
            CurrentAction = Actions.None;            

            switch (Action)
            {
                case Actions.Action1:
                    CloseGrid(EditorReg.Action1);
                    ResetEditor(EditorReg.Action1);
                    break;
                case Actions.Action2:
                    CloseGrid(EditorReg.Action2);
                    ResetEditor(EditorReg.Action2);
                    break;
                case Actions.Action3:
                    CloseGrid(EditorReg.Action3);
                    ResetEditor(EditorReg.Action3);
                    break;
                case Actions.Action4:
                    CloseGrid(EditorReg.Action4);
                    ResetEditor(EditorReg.Action4);
                    break;
                case Actions.Action5:
                    CloseGrid(EditorReg.Action5);
                    ResetEditor(EditorReg.Action5);
                    break;
                case Actions.Action6:
                    CloseGrid(EditorReg.Action6);
                    ResetEditor(EditorReg.Action6);
                    break;
                case Actions.Action7:
                    CloseGrid(EditorReg.Action7);
                    ResetEditor(EditorReg.Action7);
                    break;
                case Actions.None:
                    return;
            }
            Shift(Action);

            int Shifted;
            if ((Action == Actions.Action1 && !EditorReg.EditorRunning) || Action == Actions.None)
                Shifted = 7;
            else if (Action == Actions.Action1 && EditorReg.EditorRunning)
                Shifted = 0;
            else
                Shifted = (int)Action - 1;

            LoadEditor(EditorReg.AssociatedEditor((Actions)Shifted));            
        }
        private void CloseGrid(Editors Editor) //Sets the visibility of a single editor to collapsed.
        {
            switch (Editor)
            {
                case Editors.AddCollection:
                    AddCollection.Visibility = Visibility.Collapsed;
                    break;
                case Editors.AddMeme:
                    AddMeme.Visibility = Visibility.Collapsed;
                    break;
                case Editors.AllObjects:
                    AllObjects.Visibility = Visibility.Collapsed;
                    break;
                case Editors.BasicEditor:
                    BasicEditor.Visibility = Visibility.Collapsed;
                    break;
                case Editors.CurrentFile:
                    CurrentFile.Visibility = Visibility.Collapsed;
                    break;
                case Editors.ManageCollections:
                    ManageCollections.Visibility = Visibility.Collapsed;
                    break;
                case Editors.ManageMemes:
                    ManageMemes.Visibility = Visibility.Collapsed;
                    break;
                case Editors.None:
                    return;
                case Editors.ReportBug:
                    //ReportBug.Visibility = Visibility.Collapsed;
                    break;
                case Editors.Settings:
                    Settings.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        private void SetName(Editors Editor) //Sets an action name by determining the first open action for use.
        {
            string Value = "";
            bool IsNone = false;
            switch (Editor)
            {
                case Editors.AddCollection:
                    Value = "Add Collection";
                    break;
                case Editors.AddMeme:
                    Value = "Add Meme";
                    break;
                case Editors.AllObjects:
                    Value = "All Objects";
                    break;
                case Editors.BasicEditor:
                    Value = "Editor";
                    break;
                case Editors.CurrentFile:
                    Value = "Current File";
                    break;
                case Editors.ManageCollections:
                    Value = "Collections";
                    break;
                case Editors.ManageMemes:
                    Value = "Memes";
                    break;
                case Editors.None:
                    IsNone = true;
                    break;
                case Editors.ReportBug:
                    Value = "Report Bug";
                    break;
                case Editors.Settings:
                    Value = "Settings";
                    break;
            }

            if (IsNone)
            {
                return;
            }
            Actions Last = EditorReg.LastEmpty;

            EditorReg.Assign(EditorReg.LastEmpty, Editor);

            switch (Last)
            {
                case Actions.None:
                    return;
                case Actions.Action1:
                    Action1.Visibility = Visibility.Visible;
                    Action1B.Content = Value;
                    Action1Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action2:
                    Action2.Visibility = Visibility.Visible;
                    Action2B.Content = Value;
                    Action2Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action3:
                    Action3.Visibility = Visibility.Visible;
                    Action3B.Content = Value;
                    Action3Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action4:
                    Action4.Visibility = Visibility.Visible;
                    Action4B.Content = Value;
                    Action4Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action5:
                    Action5.Visibility = Visibility.Visible;
                    Action5B.Content = Value;
                    Action5Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action6:
                    Action6.Visibility = Visibility.Visible;
                    Action6B.Content = Value;
                    Action6Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action7:
                    Action7.Visibility = Visibility.Visible;
                    Action7B.Content = Value;
                    Action7Close.Visibility = Visibility.Visible;
                    break;
            }
        }
        private void Shift(Actions Shifted) //Shifts the OpenEditors, and moves the text based on an action to shift from. This is run when an editor is closed, and then keeps the editors in line without blank spots.
        {
            switch (Shifted)
            {
                case Actions.Action1:
                    EditorReg.Assign(Actions.Action1,EditorReg.Action2);
                    EditorReg.Assign(Actions.Action2, EditorReg.Action3);
                    EditorReg.Assign(Actions.Action3, EditorReg.Action4);
                    EditorReg.Assign(Actions.Action4, EditorReg.Action5);
                    EditorReg.Assign(Actions.Action5, EditorReg.Action6);
                    EditorReg.Assign(Actions.Action6, EditorReg.Action7);
                    EditorReg.CloseItem(Actions.Action7);

                    Action1B.Content = Action2B.Content;
                    Action2B.Content = Action3B.Content;
                    Action3B.Content = Action4B.Content;
                    Action4B.Content = Action5B.Content;
                    Action5B.Content = Action6B.Content;
                    Action6B.Content = Action7B.Content;
                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.Action2:
                    EditorReg.Assign(Actions.Action2, EditorReg.Action3);
                    EditorReg.Assign(Actions.Action3, EditorReg.Action4);
                    EditorReg.Assign(Actions.Action4, EditorReg.Action5);
                    EditorReg.Assign(Actions.Action5, EditorReg.Action6);
                    EditorReg.Assign(Actions.Action6, EditorReg.Action7);
                    EditorReg.CloseItem(Actions.Action7);

                    Action2B.Content = Action3B.Content;
                    Action3B.Content = Action4B.Content;
                    Action4B.Content = Action5B.Content;
                    Action5B.Content = Action6B.Content;
                    Action6B.Content = Action7B.Content;
                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.Action3:
                    EditorReg.Assign(Actions.Action3, EditorReg.Action4);
                    EditorReg.Assign(Actions.Action4, EditorReg.Action5);
                    EditorReg.Assign(Actions.Action5, EditorReg.Action6);
                    EditorReg.Assign(Actions.Action6, EditorReg.Action7);
                    EditorReg.CloseItem(Actions.Action7);

                    Action3B.Content = Action4B.Content;
                    Action4B.Content = Action5B.Content;
                    Action5B.Content = Action6B.Content;
                    Action6B.Content = Action7B.Content;
                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.Action4:
                    EditorReg.Assign(Actions.Action4, EditorReg.Action5);
                    EditorReg.Assign(Actions.Action5, EditorReg.Action6);
                    EditorReg.Assign(Actions.Action6, EditorReg.Action7);
                    EditorReg.CloseItem(Actions.Action7);

                    Action4B.Content = Action5B.Content;
                    Action5B.Content = Action6B.Content;
                    Action6B.Content = Action7B.Content;
                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.Action5:
                    EditorReg.Assign(Actions.Action5, EditorReg.Action6);
                    EditorReg.Assign(Actions.Action6, EditorReg.Action7);
                    EditorReg.CloseItem(Actions.Action7);

                    Action5B.Content = Action6B.Content;
                    Action6B.Content = Action7B.Content;
                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.Action6:
                    EditorReg.Assign(Actions.Action6, EditorReg.Action7);
                    EditorReg.CloseItem(EditorReg.Action7);

                    Action6B.Content = Action7B.Content;
                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.Action7:
                    EditorReg.CloseItem(EditorReg.Action7);

                    Action7B.Content = "Action";
                    Action7.Visibility = Visibility.Collapsed;
                    break;
                case Actions.None:
                    return;
            }

            if (EditorReg.Action1 == Editors.None)
            {
                Action1.Visibility = Visibility.Collapsed;
            }

            if (EditorReg.Action2 == Editors.None)
            {
                Action2.Visibility = Visibility.Collapsed;
            }

            if (EditorReg.Action3 == Editors.None)
            {
                Action3.Visibility = Visibility.Collapsed;
            }

            if (EditorReg.Action4 == Editors.None)
            {
                Action4.Visibility = Visibility.Collapsed;
            }

            if (EditorReg.Action5 == Editors.None)
            {
                Action5.Visibility = Visibility.Collapsed;
            }

            if (EditorReg.Action6 == Editors.None)
            {
                Action6.Visibility = Visibility.Collapsed;
            }

            if (EditorReg.Action7 == Editors.None)
            {
                Action7.Visibility = Visibility.Collapsed;
            }
        }

        public void LoadEditor(Editors Editor) //Loads an editor from the edtitors, and regigsters the editor and action pair.
        {
            if (Editor == Editors.None)
                return;

            if (!Ins.IsLoaded)
            {
                Lock();
                return;
            }

            HideAllEditors();
            if (EditorReg.SpecEditorRuning(Editor))
            {
                ShowEditor(Editor);
                return;
            }
            SetName(Editor);
            if (Editor != Editors.BasicEditor)
                ResetEditor(Editor);
            CurrentAction = EditorReg.AssociatedAction(Editor);
            switch (Editor)
            {
                case Editors.AddCollection:
                {
                    AddCollection.Visibility = Visibility.Visible;
                    adcTitle.Text = "";
                    break;
                }
                case Editors.AddMeme:
                {
                    AddMeme.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.AllObjects:
                {
                    AllObjects.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.BasicEditor:
                {
                    BasicEditor.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.CurrentFile:
                {
                    CurrentFile.Visibility = Visibility.Visible;

                    IHSession file = Ins.LoadedSession;

                    FileTitle.Text = file.Title;

                    FiMemesCount.Text = $"{file.CountOf(typeof(Meme))}";

                    //Begin memes
                    ICountable<MemeTypes> Memes = file;

                    FiAttachmentCount.Text = $"{(Memes is null ? 0 : Memes.CountOf(MemeTypes.Attachment))}";
                    FiScriptCount.Text = $"{(Memes is null ? 0 : Memes.CountOf(MemeTypes.Script))}";
                    FiStandardMemeCount.Text = $"{(Memes is null ? 0 : Memes.CountOf(MemeTypes.Standard))}";
                    //End Memes

                    //Begin Collections
                    FiCollectionCount.Text = $"{file.CountOf(typeof(Collection))}";

                    ICountable<CollectionTypes> Collections = file;

                    FiMasterCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Master))}";
                    FiFavoriteCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Favorite))}";
                    FiLegendaryCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Legendary))}";
                    FiStandardCollectionCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Standard))}";
                    //End Collections

                    FiBinsCount.Text = $"{file.CountOf(typeof(Bin))}";
                    FiFilesCount.Text = $"{file.CountOf(typeof(Attachment))}";

                    switch (file.Version)
                    {
                        case AppVersions.V_2019_1:
                            FiVersion.Text = "2019.1";
                            break;
                        case AppVersions.V_2019_2:
                            FiVersion.Text = "2019.2";
                            break;
                        case AppVersions.V_2020_1:
                            FiVersion.Text = "2020.1";
                            break;
                    }
                    FiLocation.Text = Ins.CurrentLoaded;
                    break;
                }
                case Editors.ManageCollections:
                {
                    ManageCollections.Visibility = Visibility.Visible;

                    macObjectTree.Items.Clear();
                    macObjectTree.Items.Add(SessionBridge.GetAllCollections(Ins.CurrentLoaded, false));

                    macObjTitle.Text = "*NoInfo*";
                    macObjType.Text = "*NoInfo*";
                    macType.Text = "*NoInfo*";

                    break;
                }
                case Editors.ManageMemes:
                {
                    ManageMemes.Visibility = Visibility.Visible;

                    mamObjectTree.Items.Clear();
                    mamObjectTree.Items.Add(SessionBridge.GetAllMemes(Ins.CurrentLoaded));
                    break;
                }
                case Editors.None:
                    return;
                case Editors.ReportBug:
                {
                    //ReportBug.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.Settings:
                {
                    Settings.Visibility = Visibility.Visible;
                    setShowWelcome.IsChecked = Ins.ShowWelcome;
                    setLoadLast.IsChecked = Ins.LoadLast;
                    setSaveMemeCreator.IsChecked = Ins.SaveMemeCreator;
                    break;
                }
            }
        }
        public void ShowEditor(Editors Editor) //Shows an open editor, and sets the current action to that assocated action.
        {
            if (Editor == Editors.None)
                return;

            if (!Ins.IsLoaded)
            {
                Lock();
                return;
            }

            if (!EditorReg.SpecEditorRuning(Editor))
            {
                LoadEditor(Editor);
                return;
            }

            CurrentAction = EditorReg.AssociatedAction(Editor);

            HideAllEditors();
            switch (CurrentAction)
            {
                case Actions.Action1:
                    Action1Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action2:
                    Action2Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action3:
                    Action3Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action4:
                    Action4Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action5:
                    Action5Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action6:
                    Action6Close.Visibility = Visibility.Visible;
                    break;
                case Actions.Action7:
                    Action7Close.Visibility = Visibility.Visible;
                    break;
            }

            switch (Editor)
            {
                case Editors.AddCollection:
                {
                    AddCollection.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.AddMeme:
                {
                    AddMeme.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.AllObjects:
                {
                    AllObjects.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.BasicEditor:
                {
                    BasicEditor.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.CurrentFile:
                {
                    CurrentFile.Visibility = Visibility.Visible;

                    IHSession file = Ins.LoadedSession;

                    FileTitle.Text = file.Title;

                    FiMemesCount.Text = $"{file.CountOf(typeof(Meme))}";

                    //Begin memes
                    ICountable<MemeTypes> Memes = file;

                    FiAttachmentCount.Text = $"{(Memes is null ? 0 : Memes.CountOf(MemeTypes.Attachment))}";
                    FiScriptCount.Text = $"{(Memes is null ? 0 : Memes.CountOf(MemeTypes.Script))}";
                    FiStandardMemeCount.Text = $"{(Memes is null ? 0 : Memes.CountOf(MemeTypes.Standard))}";
                    //End Memes

                    //Begin Collections
                    FiCollectionCount.Text = $"{file.CountOf(typeof(Collection))}";

                    ICountable<CollectionTypes> Collections = file;

                    FiMasterCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Master))}";
                    FiFavoriteCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Favorite))}";
                    FiLegendaryCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Legendary))}";
                    FiStandardCollectionCount.Text = $"{(Collections is null ? 0 : Collections.CountOf(CollectionTypes.Standard))}";
                    //End Collections

                    FiBinsCount.Text = $"{file.CountOf(typeof(Bin))}";
                    FiFilesCount.Text = $"{file.CountOf(typeof(Attachment))}";

                    switch (file.Version)
                    {
                        case AppVersions.V_2019_1:
                            FiVersion.Text = "2019.1";
                            break;
                        case AppVersions.V_2019_2:
                            FiVersion.Text = "2019.2";
                            break;
                        case AppVersions.V_2020_1:
                            FiVersion.Text = "2020.1";
                            break;
                    }
                    FiLocation.Text = Ins.CurrentLoaded;
                    break;
                }
                case Editors.ManageCollections:
                {
                    ManageCollections.Visibility = Visibility.Visible;

                    macObjectTree.Items.Clear();
                    macObjectTree.Items.Add(SessionBridge.GetAllCollections(Ins.CurrentLoaded, false));

                    macObjTitle.Text = "*NoInfo*";
                    macObjType.Text = "*NoInfo*";
                    macType.Text = "*NoInfo*";

                    break;
                }
                case Editors.ManageMemes:
                {
                    ManageMemes.Visibility = Visibility.Visible;

                    mamObjectTree.Items.Clear();
                    mamObjectTree.Items.Add(SessionBridge.GetAllMemes(Ins.CurrentLoaded));
                    break;
                }
                case Editors.None:
                    return;
                case Editors.ReportBug:
                {
                    //ReportBug.Visibility = Visibility.Visible;
                    break;
                }
                case Editors.Settings:
                {
                    Settings.Visibility = Visibility.Visible;
                    setShowWelcome.IsChecked = Ins.ShowWelcome;
                    setLoadLast.IsChecked = Ins.LoadLast;
                    setSaveMemeCreator.IsChecked = Ins.SaveMemeCreator;
                    break;
                }
            }
        }

        public void ResetEditor(Editors Editor) //Resets an editor's values. (Sets all of the editable feilds back to normal)
        {
            switch (Editor)
            {
                case Editors.AddCollection:
                    adcTitle.Text = "";
                    adcLocation.Text = "Session";
                    adcType.Text = "Standard";
                    break;
                case Editors.AddMeme:
                    if (!Ins.SaveMemeCreator)
                    {
                        admCreator.Text = "";
                    }

                    admAtt1.Text = "";
                    admAtt2.Text = "";
                    admAtt3.Text = "";
                    admAtt4.Text = "";
                    admAtt5.Text = "";

                    admDesc.Text = "";
                    admLocation.Text = "<Select Collection>";
                    admTitle.Text = "";
                    admType.Text = "Standard";

                    break;
                case Editors.AllObjects:

                    break;
                case Editors.BasicEditor:
                    baeTitle.Text = "";
                    baeType.Text = "";
                    baeCreator.Text = "";
                    baeDescription.Text = "";
                    break;
                case Editors.CurrentFile:
                    FileTitle.Text = "No Session";

                    FiMemesCount.Text = "0";
                    FiScriptCount.Text = "0";
                    FiAttachmentCount.Text = "0";
                    FiStandardMemeCount.Text = "0";

                    FiCollectionCount.Text = "0";
                    FiMasterCount.Text = "0";
                    FiLegendaryCount.Text = "0";
                    FiFavoriteCount.Text = "0";
                    FiStandardCollectionCount.Text = "0";

                    FiVersion.Text = "2019.2";
                    FiLocation.Text = "C:\\";
                    break;
                case Editors.ManageCollections:

                    break;
                case Editors.ManageMemes:

                    break;
                case Editors.None:
                    return;
                case Editors.ReportBug:

                    break;
                case Editors.Settings:

                    break;
            }
        }

        public void ResetActions() //Resets all actions (buttons in navigation) back to their origianal standing (collapsed, with "Action" as their content).
        {
            Action1.Visibility = Visibility.Collapsed;
            Action1B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action1);

            Action2.Visibility = Visibility.Collapsed;
            Action2B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action2);

            Action3.Visibility = Visibility.Collapsed;
            Action3B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action3);

            Action4.Visibility = Visibility.Collapsed;
            Action4B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action4);

            Action5.Visibility = Visibility.Collapsed;
            Action5B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action5);

            Action6.Visibility = Visibility.Collapsed;
            Action6B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action6);

            Action7.Visibility = Visibility.Collapsed;
            Action7B.Content = "Action";
            EditorReg.CloseItem(EditorReg.Action7);
        }
        public void ResetCloses() //Sets all of the Action's Close button's visibiltiy to collapsed.
        {
            Action1Close.Visibility = Visibility.Collapsed;
            Action2Close.Visibility = Visibility.Collapsed;
            Action3Close.Visibility = Visibility.Collapsed;
            Action4Close.Visibility = Visibility.Collapsed;
            Action5Close.Visibility = Visibility.Collapsed;
            Action6Close.Visibility = Visibility.Collapsed;
            Action7Close.Visibility = Visibility.Collapsed;
        }

        public bool EditorUsed(Editors Editor) //Determines if an editor is currently being used.
        {
            return EditorReg.SpecEditorRuning(Editor);
        }

        public void CloseAllEditors() //Closes, resets, and shuts down all editors, actions, and closes.
        {
            CloseEditor(Actions.Action1);
            CloseEditor(Actions.Action2);
            CloseEditor(Actions.Action3);
            CloseEditor(Actions.Action4);
            CloseEditor(Actions.Action5);
            CloseEditor(Actions.Action6);
            CloseEditor(Actions.Action7);

            ResetActions();
            ResetCloses();
        }

        public Actions CurrentAction; //The current action that is loaded.
    }
}
