using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Xml;

using Phosphor;
using HmPro.Files;
using HmPro.Events;
using HmPro.Windows;
using HmPro.Windows.Editing;
using HmPro.Registry;

namespace HmPro
{
    /// <summary>
    /// This class is used to manage the saving, appending, closing, and opening of files. 
    /// </summary>
    public static class SessionControl
    {
        public static bool SaveAsCopy()
        {
            if (!Ins.IsLoaded)
            {
                MessageBox.Show("Save As Copy cannot run if no session is loaded. Please open a file before running this command.", "Error:", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            string Filter = "";
            string WorkingType = FileTools.FileExt(Ins.CurrentLoaded, false);
            switch (WorkingType)
            {
                case "teh":
                    Filter = "Hmmm Pro Sessions|*.teh";
                    break;
                case "tehseg":
                    Filter = "Hmmm Pro Segments|*.tehseg";
                    break;
            }

            Microsoft.Win32.SaveFileDialog open = new Microsoft.Win32.SaveFileDialog()
            {
                Filter = Filter,
                RestoreDirectory = true,
                Title = "Select a path for the copy:"
            };

            open.ShowDialog();
            string Path = open.FileName;

            if (Path == "")
                return false;

            if (File.Exists(Path))
                File.Delete(Path);
            File.Copy(Ins.CurrentLoaded, Path);
            MessageBox.Show("The file was copied.", "Save As Copy:", MessageBoxButton.OK, MessageBoxImage.Information);


            //This section will set the sessions title property to the file's name (the value before the period). If the Session is not a .teh or .tehseg, no action commenses. 
            IHSession Thing;
            if (FileTools.FileExt(Path, false) == "teh")
                Thing = new Session(Path);
            else if (FileTools.FileExt(Path, false) == "tehseg")
                Thing = new Segment(Path);
            else
                return true; 
            Thing.Title = FileTools.FileName(Path);
            return true;
        }
        public static bool SaveAs() //Returns true for proper saving, false for cancel or errors.
        {
            string Filter = "";
            string WorkingType = FileTools.FileExt(Ins.CurrentLoaded, false);
            switch (WorkingType)
            {
                case "teh":
                    Filter = "Hmmm Pro Sessions|*.teh";
                    break;
                case "tehseg":
                    Filter = "Hmmm Pro Segments|*.tehseg";
                    break;
            }

            Microsoft.Win32.SaveFileDialog open = new Microsoft.Win32.SaveFileDialog()
            {
                Filter = Filter,
                RestoreDirectory = true,
                Title = "Select a location for saving the session."
            };

            open.ShowDialog();

            string path = open.FileName;

            if (path == "")
                return false;

            if (!Ins.IsLoaded)
            {
                MessageBox.Show("The session cannot be saved in a new location because the session no longer exists. This cannot be fixed (Was it moved or deleted?).Sorry For the Inconvience.", "Save As Error:", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (File.Exists(path))
                File.Delete(path);
            File.Copy(Ins.CurrentLoaded, path);
            Ins.CurrentLoaded = path;
            Ins.LastLoaded = path;

            Recent.AddRecent(path);

            IHSession Thing = Ins.LoadedSession;
            Thing.Title = FileTools.FileName(path);
            return true;
        }

        public static bool Append(Actions Action, bool RunExitActions = true) //Returns true if the specified action was appened, false for if it is a non-appendable type (such as CurrentFile), an error occurs, it gets canceled for if the action = none.
        {
            Session File = new Session(Ins.CurrentLoaded);
            Main Main = App.MainIns;

            Editors Open = Editors.None;
            switch (Action)
            {
                case Actions.Action1:
                    Open = EditorReg.Action1;
                    break;
                case Actions.Action2:
                    Open = EditorReg.Action2;
                    break;
                case Actions.Action3:
                    Open = EditorReg.Action3;
                    break;
                case Actions.Action4:
                    Open = EditorReg.Action4;
                    break;
                case Actions.Action5:
                    Open = EditorReg.Action5;
                    break;
                case Actions.Action6:
                    Open = EditorReg.Action6;
                    break;
                case Actions.Action7:
                    Open = EditorReg.Action7;
                    break;
                case Actions.None:
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
            }

            return Append(Open, RunExitActions);
        }
        public static bool Append(Editors Editor, bool RunExitActions = true)
        {
            IHSession File = Ins.LoadedSession;
            Main Main = App.MainIns;

            bool FileType; //False for Segment, True for Session
            FileType = File is Session ? true : false;

            switch (Editor)
            {
                case Editors.AddCollection:
                {
                    CollectionList CollectionList;
                    if (FileType)
                        CollectionList = ((Session)File).Collections;
                    else
                        CollectionList = ((Segment)File).Collections;

                    CollectionTypes Type = CollectionTypes.Standard;
                    switch (Main.adcType.Text)
                    {
                        case "Standard":
                            Type = CollectionTypes.Standard;
                            break;
                        case "Favorite":
                            Type = CollectionTypes.Favorite;
                            break;
                        case "Legendary":
                            Type = CollectionTypes.Legendary;
                            break;
                        case "Master":
                            Type = CollectionTypes.Master;
                            break;
                    }

                    if (Main.adcTitle.Text == "")
                    {
                        MessageBox.Show("The collection cannot be appended, as title cannot be blank.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return false;
                    }

                    if (Main.adcType.Text == "")
                    {
                        Main.adcType.Text = "Standard";
                    }
                    if (Main.adcLocation.Text == "")
                    {
                        Main.adcLocation.Text = "Session";
                    }

                    Retry:
                    switch (Main.adcLocation.Text)
                    {
                        case "Session":
                        {
                            Collection New = CollectionList.Add(Type);

                            if (New == null)
                            {
                                MessageBox.Show("The collection given is currently in the session, and duplicates are not allowed.", "Add Collection:", MessageBoxButton.OK, MessageBoxImage.Error);
                                return false;
                            }
                            New.Title = Main.adcTitle.Text;
                            break;
                        }
                        case "<Select Master Collection>":
                        {
                            if (Type == CollectionTypes.Master)
                            {
                                MessageBoxResult Result = MessageBox.Show("A master collection cannot be appended to another collection.\nDo you want to add this to session?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                                switch (Result)
                                {
                                    case MessageBoxResult.Cancel | MessageBoxResult.None | MessageBoxResult.No:
                                        MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                        return false;
                                    case MessageBoxResult.Yes:
                                        Main.adcLocation.Text = "Session";
                                        goto Retry;
                                }
                            }
                            ObjectSelectionArgs args = ObjectSelector.Execute(Memes: false, Collections: false, MasterCollections: true, Bins: false);
                            if (!args.Connected || args.Object == null || !(args.Object is Collection) || (args.Object as Collection).Type != CollectionTypes.Master)
                            {
                                MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                Main.ResetEditor(Editors.AddCollection);
                                return false;
                            }

                            Collection New = ((Collection)args.Object).Collections?.Add(Type);
                            New.Title = Main.adcTitle.Text;

                            Main.ResetEditor(Editors.AddCollection);
                            break;
                        }
                    }


                    break;
                }
                case Editors.AddMeme:
                {
                    MemeTypes Type = MemeTypes.Standard;
                    switch (Main.admType.Text)
                    {
                        case "Standard":
                            Type = MemeTypes.Standard;
                            break;
                        case "Attachment":
                            Type = MemeTypes.Attachment;
                            break;
                        case "Script":
                            Type = MemeTypes.Script;
                            break;
                    }

                    if (Main.admTitle.Text == "")
                    {
                        MessageBox.Show("The meme cannot be appended, as title cannot be blank.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return false;
                    }

                    if (Main.admType.Text == "") //This fixes any parameters that have not been ommited.
                    {
                        Main.admType.Text = "Standard";
                    }
                    if (Main.admLocation.Text == "")
                    {
                        Main.admLocation.Text = "<Select Collection>";
                    }

                    ///*
                    //* The location is special. It is decided by ConnectedEventArgs args (defined later), and setup in a special way.
                    // *  1. From Collection & Object associated; (true, CollectionBase)
                    // *  2. From Favorite Memes & null; (true, null)
                    //*  3. From Legendary Memes & null. (true, null)
                    //*  
                    //*  when Collection ommits true, then the object is added under the component's memes. Component must be CollectionBase.
                    //*  when Favortie Memes ommits true, then it adds the object to the Session.FavoriteMemes list.
                    //*  when Legendary Memes ommits true, then it adds the object to the Session.LegendaryMemes list.
                    //
                    DoubleList<bool, IComponent> Location = new DoubleList<bool, IComponent>();
                    if (Main.admLocation.Text == "<Select Collection>")
                    {
                        ObjectSelectionArgs args = ObjectSelector.Execute(Memes: false, Collections: true, MasterCollections: true, Bins: false);
                        if (!args.Connected)
                        {
                            MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                            Main.ResetEditor(Editors.AddMeme);
                            return false;
                        }
                        if (args.Object == null)
                        {
                            switch (args.Notes)
                            {
                                case "Favorite_Memes":
                                    Location.Add(false, null);
                                    Location.Add(true, null);
                                    Location.Add(false, null);
                                    break;
                                case "Legendary_Memes":
                                    Location.Add(false, null);
                                    Location.Add(false, null);
                                    Location.Add(true, null);
                                    break;
                                case "Memes":
                                    Location.Add(false, null);
                                    Location.Add(false, null);
                                    Location.Add(false, null);
                                    break;
                            }
                        }
                        else
                        {
                            Location.Add(true, args.Object);
                            Location.Add(false, null);
                            Location.Add(false, null);
                        }
                    }
                    else if (Main.admLocation.Text == "Favorite Memes")
                    {
                        Location.Add(false, null);
                        Location.Add(true, null);
                        Location.Add(false, null);
                    }
                    else if (Main.admLocation.Text == "Legendary Memes")
                    {
                        Location.Add(false, null);
                        Location.Add(false, null);
                        Location.Add(true, null);
                    }

                    List<bool> Options = Location.Ones;
                    List<IComponent> Components = Location.Twos;
                    Meme meme = null;

                    if (Options[0] == true && Options[1] == false && Options[2] == false)
                    {
                        if (Components[0] == null || !(Components[0] is Collection))
                        {
                            MessageBox.Show("There was an unresolvable internal error. Sorry for the inconvience.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                            return false;
                        }
                        
                        meme = ((Collection)Components[0]).Memes.Add(Type);
                    }
                    else if (Options[0] == false && Options[1] == true && Options[2] == false)
                    {
                        if (File is Session)
                            meme = (File as Session).FavoriteMemes.Add(Type);
                        else
                            meme = (File as Segment).Memes.Add(Type);  
                    }
                    else if (Options[0] == false && Options[1] == false && Options[2] == true)
                    {
                        if (File is Session)
                            meme = (File as Session).LegendaryMemes.Add(Type);
                        else
                            meme = (File as Segment).Memes.Add(Type);
                    }
                    else if (Options[0] == false && Options[1] == false && Options[2] == false)
                    {
                        if (File is Segment)
                        {
                            meme = (File as Segment).Memes.Add(Type);
                        }
                        else
                        {
                            MessageBox.Show("Memes cannot be appended to a Session directly. It must be added to a Collection, Favorite Memes, or Legendary Memes only.", "Append:", MessageBoxButton.OK, MessageBoxImage.Error);
                            return false;
                        }
                    }

                    if (meme == null)
                    {
                        MessageBox.Show("The meme could not be appended, the location was inconclusive.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return false;
                    }

                    meme.Title = Main.admTitle.Text;
                    meme.Creator = Main.admCreator.Text;

                    Main.ResetEditor(Editor);
                    break;
                }
                case Editors.AllObjects: //This is a view only thing, with pointers to other editors, such as 'Manage Memes' and 'Add Meme'. This does not contain information.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.BasicEditor:
                {
                    IComponent Obj = Main.baeObj;

                    switch (Obj.ObjectType)
                    {
                        case ComponentTypes.Collection:
                        {
                            Collection Collection = (Collection)Obj;

                            if (Collection.Type == CollectionTypes.Master)
                                Main.baeType.Text = "Master Collection";
                            string type = Main.baeType.Text.ToLower();

                            if (Main.baeTitle.Text == "")
                            {
                                MessageBox.Show("The item could not be modified.\nEnsure that the object has a title.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                return false;
                            }

                            Collection.Title = Main.baeTitle.Text;
                            Collection.Type = (type == "favorite collection") ? CollectionTypes.Favorite : (type == "legendary collection") ? CollectionTypes.Legendary : (type == "master collection") ? CollectionTypes.Master : CollectionTypes.Standard;
                            break;
                        }
                        case ComponentTypes.Meme:
                        {
                            Meme Meme = (Meme)Obj;

                            string title = Main.baeTitle.Text;
                            string type = Main.baeType.Text.ToLower();

                            if (title == "" || (type != "standard meme" && type != "script meme" && type != "attachment meme"))
                            {
                                MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & a correct type.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                return false;
                            }

                            Meme.Title = title;
                            Meme.Creator = Main.baeCreator.Text;
                            Meme.Description = Main.baeDescription.Text;
                            break;
                        }
                        case ComponentTypes.Bin:
                        {
                            break;
                        }
                    }

                    Main.CloseEditor(EditorReg.AssociatedAction(Editors.BasicEditor));
                    break;
                }
                case Editors.CurrentFile: //There is nothing to append here.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.ManageCollections: //Nothing to append.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.ManageMemes: //Nothing to append.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.None: //There is nothing to append here, as it is nothing.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.ReportBug: //Nothing to append.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    break;
                }
                case Editors.Settings: //There is nothing to append here, information is appened as it is modifyied.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    break;
                }
            }

            if (RunExitActions)
            {
                App.ThrowAppended(File, true);
                MessageBox.Show("Append Complete.", "Append Action:", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            return true;
        }
        public static bool AppendAll() //Appends all items. Returns true if all of the editors passed and appended, false for errors or canceling.
        {
            Session File = new Session(Ins.CurrentLoaded);
            Main Main = App.MainIns;

            EditorReg editors = new EditorReg();
            IEnumerable<Editors> Open = editors.GetOpenEditors();

            List<Editors> OpenT = new List<Editors>(Open);
            if (OpenT.Count == 0)
            {
                MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
            }

            foreach (Editors Current in Open)
            {
                if (!Append(Current, true))
                {
                    MessageBox.Show("Append All Failed.", "Append All", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }

            App.ThrowAppended(File, true);
            MessageBox.Show("Append All Complete.", "Append Action:", MessageBoxButton.OK, MessageBoxImage.Information);
            return true;
        }

        public static bool NewFile()
        {
            Microsoft.Win32.SaveFileDialog save = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Hmmm Pro Sessions |*.teh|Hmmm Pro Segments|*.tehseg",
                RestoreDirectory = true,
                Title = "Create a new file:"
            };

            save.ShowDialog();

            if (save.FileName == "")
            {
                MessageBox.Show("No file path was supplied.", "New File:", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (Ins.IsLoaded)
                CloseFile();

            if (File.Exists(save.FileName))
                File.Delete(save.FileName);

            if (FileTools.FileExt(save.FileName, false) == "teh")
                File.Copy(Ins.BasicSession, save.FileName);
            else if (FileTools.FileExt(save.FileName, false) == "tehseg")
                File.Copy(Ins.BasicSegment, save.FileName);

            Ins.CurrentLoaded = save.FileName;
            Ins.LastLoaded = save.FileName;
            Recent.AddRecent(save.FileName);

            IHSession Thing = Ins.LoadedSession;
            Thing.Title = FileTools.FileName(save.FileName);
            return true;
        }
        public static bool OpenFile()
        {
            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog()
            {
                Filter = "Hmmm Pro Sessions |*.teh|Hmmm Pro Segments|*.tehseg",
                Multiselect = false,
                RestoreDirectory = true,
                Title = "Select a session to open."
            };

            open.ShowDialog();

            string path = open.FileName;

            if (path == "")
            {
                MessageBox.Show("No file was selected.", "Open File:", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (Ins.IsLoaded)
                CloseFile();

            Ins.CurrentLoaded = path;
            Ins.LastLoaded = path;
            Recent.AddRecent(path);

            IHSession Thing = Ins.LoadedSession;
            Thing.Title = FileTools.FileName(path);
            return true;
        }
        public static bool OpenFile(string path)
        {
            if (path == "" || !File.Exists(path))
            {
                MessageBox.Show("No file was selected.", "Open File:", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (Ins.IsLoaded)
                CloseFile();

            Ins.CurrentLoaded = path;
            Ins.LastLoaded = path;
            Recent.AddRecent(path);

            IHSession Thing = Ins.LoadedSession;
            Thing.Title = FileTools.FileName(path);
            return true;
        }
        public static bool CloseFile()
        {
            if (!Ins.IsLoaded)
                return false;

            if (EditorReg.AppendableEditorRunning)
            {
                MessageBoxResult Result = MessageBox.Show("Do you want to append before closing?\n\nIf not, then all non-appended data will be lost.", "Close File:", MessageBoxButton.YesNoCancel, MessageBoxImage.Exclamation);

                switch (Result)
                {
                    case MessageBoxResult.Yes:
                        if (!AppendAll()) return false;
                        break;
                    case MessageBoxResult.No:
                        break;
                    case MessageBoxResult.Cancel:
                    case MessageBoxResult.None:
                        return false;
                }
            }

            Ins.CurrentLoaded = "";
            return true;
        }
    }
}
